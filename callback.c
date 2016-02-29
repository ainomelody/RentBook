#include "callback.h"
#include <gtk/gtk.h>
#include "share.h"
#include "io.h"
#include "BookType.h"
#include "BookInfo.h"
#include "RentInfo.h"
#include "mainUI.h"
#include "query.h"
#include "queryUI.h"

void menuLoadCall(GtkMenuItem *menu, gpointer data)
{
	GtkWidget *dialog;
	
	dialog = gtk_file_chooser_dialog_new("打开文件", GTK_WINDOW(mainWindow), 
										 GTK_FILE_CHOOSER_ACTION_OPEN,
										 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                         GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
	
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		loadFromFile(filename);
		g_free(filename);
	}
	gtk_widget_destroy(dialog);
	
	reloadModel();
}

void menuSaveCall(GtkMenuItem *menu, gpointer data)
{
	GtkWidget *dialog;

	dialog = gtk_file_chooser_dialog_new("保存文件", GTK_WINDOW(mainWindow),
										 GTK_FILE_CHOOSER_ACTION_SAVE,
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                         GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                         NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;

		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		saveToFile(filename);
		g_free (filename);
	}

	gtk_widget_destroy (dialog);
}

void menuAddBookTypeCall(GtkMenuItem *menu, gpointer data)
{
	BookType *node;
	
	node = viewBookType("添加图书类别", NULL);
	if (node != NULL)
	{
		GtkTreeModel *store;
		GtkTreeIter iter;
		
		addBookType(node);
		store = gtk_tree_view_get_model(GTK_TREE_VIEW(treeView));
		gtk_tree_store_prepend(GTK_TREE_STORE(store), &iter, NULL);
		gtk_tree_store_set(GTK_TREE_STORE(store), &iter, 0, node->name, 1, (gpointer)node, -1);
	}
}

void menuAddBookInfoCall(GtkMenuItem *menu, gpointer data)
{
	BookType *parent;
	BookInfo *node;
	
	node = viewBookInfo("添加图书信息", NULL);
	if (node != NULL)
	{
		parent = findBookTypeByNum(node->type);
		if (parent == NULL)
		{
			messageBox(mainWindow, GTK_MESSAGE_ERROR, "错误", "不存在的图书类别");
			g_free(node);
			return;
		}
		node->vipPrice = node->rentPrice * parent->vipDiscoutRate;
		addToBookList(parent, node);
		reloadModel();
	}
}

void menuAddRentInfoCall(GtkMenuItem *menu, gpointer data)
{
	RentInfo *node;
	BookType *type;
	BookInfo *book;
	
	node = viewRentInfo("添加租阅信息", NULL);
	if (node != NULL)
	{
		type = findBookTypeByNum(node->type);
		if (type == NULL)
		{
			messageBox(mainWindow, GTK_MESSAGE_ERROR, "错误", "不存在的图书类别");
			g_free(node);
			return;
		}
		
		book = findBookInfoByNum(type->bookList, node->bookNum);
		if (book == NULL)
		{
			messageBox(mainWindow, GTK_MESSAGE_ERROR, "错误", "图书编号错误");
			g_free(node);
			return;
		}
		if (datecmp(book->shelveDate, node->rentDate) > 0)
		{
			messageBox(mainWindow, GTK_MESSAGE_ERROR, "错误", "租书日期早于上架日期");
			g_free(node);
			return;
		}
		addToRentList(book, node);
		calcReturnDate(node);
		calcPay(type, book, node);
		reloadModel();
	}
}

void menuDeleteCall(GtkMenuItem *menu, gpointer user_data)
{
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter, parent;
	GtkTreePath *path;
	int depth, selected;
	gpointer data, pdata;
	
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeView));
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeView));
	selected = gtk_tree_selection_get_selected(selection, &model, &iter);
	if (!selected)
	{
		messageBox(mainWindow, GTK_MESSAGE_ERROR, "错误", "未选择要删除的结点");
		return;
	}
	
	path = gtk_tree_model_get_path(model, &iter);
	
	depth = gtk_tree_path_get_depth(path);
	gtk_tree_model_get(model, &iter, 1, &data, -1);
	gtk_tree_path_free(path);
	
	if (depth == 1)
		freeTypeNode((BookType *)data);
	else
	{
		gtk_tree_model_iter_parent(model, &parent, &iter);
		gtk_tree_model_get(model, &parent, 1, &pdata, -1);
		if (depth == 2)
			freeBookNode((BookType *)pdata, (BookInfo *)data);
		else
			freeRentNode((BookInfo *)pdata, (RentInfo*)data);
	}
	gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
	
}

void menuQueryRentDetailCall(GtkMenuItem *menu, gpointer user_data)
{
	GtkWidget *window;
	
	window = rentDetailUI();
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_show_all(window);
}

void treeViewActive(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	int depth;
	GtkTreeModel *store;
	GtkTreeIter iter;
	gpointer data;
	
	depth = gtk_tree_path_get_depth(path);
	store = gtk_tree_view_get_model(GTK_TREE_VIEW(treeView));
	gtk_tree_model_get_iter(store, &iter, path);
	gtk_tree_model_get(store, &iter, 1, &data, -1);
	
	if (depth == 1)
	{
		BookType *toChange;
		
		toChange = viewBookType("修改图书类别", (BookType *)data);
		if (toChange != NULL)
		{
			changeBookType((BookType *)data, toChange);
			gtk_tree_store_set(GTK_TREE_STORE(store), &iter, 0, toChange->name, -1);
			g_free(toChange);
		}
	}
	else if (depth == 2)
	{
		BookInfo *toChange;
		
		toChange = viewBookInfo("修改图书信息", (BookInfo *)data);
		if (toChange != NULL)
		{
			changeBookInfo((BookInfo *)data, toChange);
			gtk_tree_store_set(GTK_TREE_STORE(store), &iter, 0, toChange->bookName, -1);
			g_free(toChange);
		}
	}
	else
	{
		RentInfo *toChange;
		
		toChange = viewRentInfo("修改租借信息", (RentInfo *)data);
		if (toChange != NULL)
		{
			changeRentInfo((RentInfo *)data, toChange);
			gtk_tree_store_set(GTK_TREE_STORE(store), &iter, 0, toChange->name, -1);
			g_free(toChange);
		}
	}
}

void queryRentDetailButtonCall(GtkButton *button, gpointer data)
{
	QueryStruct *st = data;
	GtkListStore *store;
	const char *text;
	
	text = gtk_entry_get_text(GTK_ENTRY(st->entry[0]));
	if (!checkDate(text))
	{
		messageBox(st->window, GTK_MESSAGE_ERROR, "错误", "错误的日期格式");
		return;
	}
	store = queryRentDetail(text);
	gtk_tree_view_set_model(GTK_TREE_VIEW(st->treeView), GTK_TREE_MODEL(store));
}

void queryWindowDestroyCall(GtkWidget *window, gpointer data)
{	
	g_free(data);
}

void menuQueryRentTimesCall(GtkMenuItem *menu, gpointer user_data)
{
	GtkWidget *window;
	
	window = rentTimesUI();
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_show_all(window);
}

void queryRentTimesButtonCall(GtkButton *button, gpointer data)
{
	QueryStruct *st = data;
	GtkListStore *store;
	const char *text;
	int year, month;
	
	text = gtk_entry_get_text(GTK_ENTRY(st->entry[0]));
	if (!sscanf(text, "%d", &year))
	{
		messageBox(st->window, GTK_MESSAGE_ERROR, "错误", "错误的年份");
		return;
	}
	
	text = gtk_entry_get_text(GTK_ENTRY(st->entry[1]));
	if (!sscanf(text, "%d", &month))
	{
		messageBox(st->window, GTK_MESSAGE_ERROR, "错误", "错误的月份");
		return;
	}
	
	store = queryRentTimes(year, month);
	gtk_tree_view_set_model(GTK_TREE_VIEW(st->treeView), GTK_TREE_MODEL(store));
}

void menuQueryExDaysCall(GtkMenuItem *menu, gpointer user_data)
{
	GtkWidget *window;
	
	window = exDaysUI();
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_show_all(window);
}

void queryInOutNumButtonCall(GtkButton *button, gpointer data)
{
	QueryStruct *st = data;
	GtkListStore *store;
	const char *start, *end;
	
	start = gtk_entry_get_text(GTK_ENTRY(st->entry[0]));
	if (!checkDate(start))
	{
		messageBox(st->window, GTK_MESSAGE_ERROR, "错误", "起始日期错误");
		return;
	}
	
	end = gtk_entry_get_text(GTK_ENTRY(st->entry[1]));
	if (!checkDate(end))
	{
		messageBox(st->window, GTK_MESSAGE_ERROR, "错误", "终止日期错误");
		return;
	}
	
	store = queryInOutNum(start, end);
	gtk_tree_view_set_model(GTK_TREE_VIEW(st->treeView), GTK_TREE_MODEL(store));
}

void menuQueryInOutNumCall(GtkMenuItem *menu, gpointer user_data)
{
	GtkWidget *window;
	
	window = inOutNumUI();
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_show_all(window);
}