#include <gtk/gtk.h>
#include "share.h"
#include <stdio.h>

static int daysOfMonth(int year, int month)
{
	switch(month)
	{
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
			if (year % 400 == 0 || year % 4 == 0 && year % 100 != 0)
				return 29;
			else
				return 28;
		default:
			return 31;
	}
}

void statusShow(char *info)
{
	gtk_statusbar_remove_all(GTK_STATUSBAR(statusBar), 0);
	gtk_statusbar_push(GTK_STATUSBAR(statusBar), 0, info);
}

void quit()
{
	freeTypeList(head);
	gtk_main_quit();
}

void messageBox(GtkWidget *parent, GtkMessageType type, char *title, char *message)
{
	GtkWidget *dialog;
	
	dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_MODAL, 
									type, GTK_BUTTONS_OK, message);
	gtk_window_set_title(GTK_WINDOW(dialog), title);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy (dialog);
}

void reloadModel()
{
	GtkTreeModel *model;
	GtkTreeStore *tree;
	GtkTreeIter iter, child, rentIter;
	BookType *typeList = head;
	BookInfo *infoList;
	RentInfo *rentList;
	
	tree = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);
	
	while (typeList != NULL)
	{
		gtk_tree_store_append(tree, &iter, NULL);
		gtk_tree_store_set(tree, &iter, 0, typeList->name, 1, (gpointer)typeList, -1);
		infoList = typeList->bookList;
		
		while (infoList != NULL)
		{
			gtk_tree_store_append(tree, &child, &iter);
			gtk_tree_store_set(tree, &child, 0, infoList->bookName, 1, (gpointer)infoList, -1);
			rentList = infoList->rentList;
			
			while (rentList != NULL)
			{
				gtk_tree_store_append(tree, &rentIter, &child);
				gtk_tree_store_set(tree, &rentIter, 0, rentList->name, 1, (gpointer)rentList, -1);
				rentList = rentList->next;
			}
			infoList = infoList->next;
		}
		typeList = typeList->next;
	}
	
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeView));
//	g_object_unref(model);														//引用数-1，析构model
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeView), GTK_TREE_MODEL(tree));
}

int checkDate(const char *date)
{
	int year, month, day;
	int readNum;
	
	readNum = sscanf(date, "%d/%d/%d", &year, &month, &day);
	return (readNum == 3 && year > 0 && month > 0 && day > 0 && day <= daysOfMonth(year, month));
}

char *calcDate(const char *date, int num)
{
	int year, month, day;
	int days;
	char *result;
	
	sscanf(date, "%d/%d/%d", &year, &month, &day);
	num += day;
	day = 0;
	
	while (num > 0)
	{
		days = daysOfMonth(year, month);
		if (num > days)
		{
			month++;
			num -= days;
		}
		else
		{
			day = num;
			num = 0;
		}
		if (month > 12)
		{
			year++;
			month = 1;
		}
	}
	
	result = g_malloc(11);
	sprintf(result, "%d/%d/%d\0", year, month, day);
	
	return result;
}

int calcDays(const char *start, const char *end)
{
	int year1, month1, day1;
	int year2, month2, day2;
	int yearcp, monthcp;
	int result = 0;
	int spareDays;
	
	sscanf(start, "%d/%d/%d", &year1, &month1, &day1);
	sscanf(end, "%d/%d/%d", &year2, &month2, &day2);
	
	yearcp = year1;
	monthcp = month1;
	
	while (year1 <= year2)
	{
		while ((year1 < year2 || month1 < month2) && month1 <= 12)
			result += daysOfMonth(year1, month1++);
		
		month1 = 1;
		year1++;
	}
	
	if (result == 0 && (yearcp > year2 || monthcp > month2 || day1 > day2))
		return 0;
	else
		result += (day2 - day1);
	
	return result;
}

static void showCalendar(GtkEntry *entry, GtkEntryIconPosition icon_pos, GdkEvent *event, gpointer data)
{
	GtkWidget *window, *calendar;
	char text[30];
	gint year, month, day;
	GtkAllocation allocation;
	GdkWindow *root, *tmp;
	GdkScreen *screen;
	GtkWidget *pop, *toplevel, *label;
	gint x, y, tx, ty;
	
	gtk_widget_get_allocation(GTK_WIDGET(entry), &allocation);
	toplevel = gtk_widget_get_toplevel(GTK_WIDGET(entry));
	screen = gtk_window_get_screen(GTK_WINDOW(toplevel));
	root = gdk_screen_get_root_window(screen);
	tmp = gtk_widget_get_window(GTK_WIDGET(entry));
	
	x = allocation.x;
	y = allocation.y;
	
	while (tmp != root)
	{
		gdk_window_get_position(tmp, &tx, &ty);
		x += tx;
		y += ty;
		tmp = gdk_window_get_parent(tmp);
	}
	
	x += allocation.width;
	y += allocation.height;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);
	gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(data));
	
	calendar = gtk_calendar_new();
	gtk_container_add(GTK_CONTAINER(window), calendar);
	g_signal_connect(calendar, "day-selected-double-click", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(window);
	
	gtk_widget_get_allocation(GTK_WIDGET(window), &allocation);
	x -= allocation.width;
	
	gtk_window_move(GTK_WINDOW(window), x, y);
	
	gtk_main();
	
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	month++;
	sprintf(text, "%d/%d/%d", year, month, day);
	gtk_entry_set_text(entry, text);
	gtk_widget_destroy(window);
	
}

GtkWidget *createDateEntry(GtkWidget *parent)
{
	GtkWidget *entry;
	
	entry = gtk_entry_new();
	gtk_entry_set_icon_from_stock(GTK_ENTRY(entry), GTK_ENTRY_ICON_SECONDARY, GTK_STOCK_EDIT);
	gtk_widget_set_hexpand(entry, TRUE);
	g_signal_connect(entry, "icon-press", G_CALLBACK(showCalendar), parent);
	
	return entry;
}

GtkWidget *createScrollWindow()
{
	GtkWidget *scrollWindow;
	
	scrollWindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_vexpand(scrollWindow, TRUE);
	gtk_widget_set_hexpand(scrollWindow, TRUE);
	return scrollWindow;
}

GtkWidget *createTreeView(int columnNum, gboolean showHeader)
{
	GtkWidget *treeView;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	int i;
	
	treeView = gtk_tree_view_new();
	renderer = gtk_cell_renderer_text_new();									//创建渲染器
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeView), showHeader);		//显示、隐藏列头
	
	for (i = 0; i < columnNum; i++)
	{
		column = gtk_tree_view_column_new();									//新建列
		gtk_tree_view_column_set_expand(column, TRUE);							//设置属性可拉伸
		gtk_tree_view_column_set_clickable(column, TRUE);
		gtk_tree_view_column_set_min_width(column, 60);
		gtk_tree_view_column_pack_start(column, renderer, TRUE);
		gtk_tree_view_column_add_attribute(column, renderer, "text", i);
		gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);			//添加一列
	}
	gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(treeView), TRUE);
	return treeView;
}

int datecmp(const char *date1, const char *date2)
{
	int year1, month1, day1;
	int year2, month2, day2;
	
	sscanf(date1, "%d/%d/%d", &year1, &month1, &day1);
	sscanf(date2, "%d/%d/%d", &year2, &month2, &day2);
	
	return (((year1 - year2) <<  9) + ((month1 - month2) << 5) + (day1 - day2));
}

void setColumnName(GtkWidget *treeView, int column, const char *name)
{
	GtkTreeViewColumn *col;
	
	col = gtk_tree_view_get_column(GTK_TREE_VIEW(treeView), column);
	gtk_tree_view_column_set_title(col, name);
}