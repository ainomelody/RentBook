#include <gtk/gtk.h>
#include "struct.h"
#include "share.h"
#include <string.h>
#include "RentInfo.h"
#include "BookType.h"
#include "BookInfo.h"

BookInfo *viewBookInfo(char *title, BookInfo *data)
{
	GtkWidget *dialog, *contentArea, *vbox;
	GtkWidget *type, *bookName, *bookNum, *publisher, *author, *stock;
	GtkWidget *salePrice, *rentPrice, *vipPrice, *shelveDate, *place;
	BookInfo *retData = NULL;
	char text[100];
	const char *textBuf;
	
	dialog = gtk_dialog_new_with_buttons(title, GTK_WINDOW(mainWindow), GTK_DIALOG_MODAL,
										 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, 
										 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	
	contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	
	
	addEntry(vbox, "图书类别", type, 1);
	addEntry(vbox, "图书编号", bookNum, 9);
	addEntry(vbox, "图书名称", bookName, 30);
	addEntry(vbox, "   作者   ", author, 19);
	addEntry(vbox, " 出版社  ", publisher, 29);
	addEntry(vbox, "   售价   ", salePrice, 10);
	addEntry(vbox, "   租价   ", rentPrice, 10);
	addEntry(vbox, " 会员价  ", vipPrice, 10);
	addEntry(vbox, " 库存数  ", stock, 10);
	addEntry(vbox, "上架日期", shelveDate, 10);
	addEntry(vbox, "   架位   ", place, 4);
	
	if (data != NULL)
	{
		sprintf(text, "%c\0", data->type);
		gtk_entry_set_text(GTK_ENTRY(type), text);
		gtk_entry_set_text(GTK_ENTRY(bookNum), data->bookNum);
		gtk_entry_set_text(GTK_ENTRY(bookName), data->bookName);
		gtk_entry_set_text(GTK_ENTRY(publisher), data->publisher);
		gtk_entry_set_text(GTK_ENTRY(author), data->author);
		sprintf(text, "%d\0", data->stock);
		gtk_entry_set_text(GTK_ENTRY(stock), text);
		sprintf(text, "%lf\0", data->salePrice);
		gtk_entry_set_text(GTK_ENTRY(salePrice), text);
		sprintf(text, "%lf\0", data->rentPrice);
		gtk_entry_set_text(GTK_ENTRY(rentPrice), text);
		sprintf(text, "%lf\0", data->vipPrice);
		gtk_entry_set_text(GTK_ENTRY(vipPrice), text);
		gtk_entry_set_text(GTK_ENTRY(shelveDate), data->shelveDate);
		gtk_entry_set_text(GTK_ENTRY(place), data->place);
		gtk_widget_set_sensitive(type, FALSE);
		gtk_widget_set_sensitive(bookNum, FALSE);
	}
	
	gtk_widget_set_sensitive(vipPrice, FALSE);
	
	gtk_container_add(GTK_CONTAINER(contentArea), vbox);
	
	gtk_widget_show_all(dialog);
	
	lblRun:
	if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_ACCEPT)
	{
		gtk_widget_destroy(dialog);
		if (retData != NULL)
			g_free(retData);
		return NULL;
	}
	
	if (retData == NULL)
		retData = g_malloc(sizeof(BookInfo));
	
	if (data == NULL)
	{
		textBuf = gtk_entry_get_text(GTK_ENTRY(type));
		checkEntry(strlen(textBuf) == 0, dialog, "图书类别为空", lblRun);
		retData->type = textBuf[0];
		
		textBuf = gtk_entry_get_text(GTK_ENTRY(bookNum));
		checkEntry(strlen(textBuf) == 0, dialog, "图书编号为空", lblRun);
		checkEntry(strlen(textBuf) > 9, dialog, "图书编号过长", lblRun);
		strncpy(retData->bookNum, textBuf, 10);
	}
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(bookName));
	checkEntry(strlen(textBuf) == 0, dialog, "图书名称为空", lblRun);
	checkEntry(strlen(textBuf) > 30, dialog, "图书名称过长", lblRun);
	strncpy(retData->bookName, textBuf, 31);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(publisher));
	checkEntry(strlen(textBuf) == 0, dialog, "出版社为空", lblRun);
	checkEntry(strlen(textBuf) > 29, dialog, "出版社名称过长", lblRun);
	strncpy(retData->publisher, textBuf, 30);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(author));
	checkEntry(strlen(textBuf) == 0, dialog, "作者为空", lblRun);
	checkEntry(strlen(textBuf) > 19, dialog, "作者姓名过长", lblRun);
	strncpy(retData->author, textBuf, 20);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(stock));
	checkEntry(sscanf(textBuf, "%d", &retData->stock) == 0, dialog,
			   "库存数不正确", lblRun);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(salePrice));
	checkEntry(sscanf(textBuf, "%lf", &retData->salePrice) == 0, dialog,
	           "售价不正确", lblRun);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(rentPrice));
	checkEntry(sscanf(textBuf, "%lf", &retData->rentPrice) == 0, dialog,
	           "租价不正确", lblRun);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(shelveDate));
	checkEntry(strlen(textBuf) == 0 || !checkDate(textBuf), 
	           dialog, "上架日期不正确", lblRun);
	strncpy(retData->shelveDate, textBuf, 11);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(place));
	checkEntry(strlen(textBuf) == 0, dialog, "架位为空", lblRun);
	checkEntry(strlen(textBuf) > 4, dialog, "架位不正确", lblRun);
	strncpy(retData->place, textBuf, 5);
	
	retData->rentList = NULL;
	retData->prev = NULL;
	retData->next = NULL;
	gtk_widget_destroy(dialog);
	
	return retData;
}

void freeBookList(BookInfo *list)
{
	BookInfo *temp;
	
	while (list != NULL)
	{
		freeRentList(list->rentList);
		temp = list->next;
		g_free(list);
		list = temp;
	}
}

void changeBookInfo(BookInfo *from, BookInfo *to)
{
	double vipPayRate;
	
	vipPayRate = from->vipPrice / from->rentPrice;
	
	strncpy(from->bookName, to->bookName, 31);
	strncpy(from->publisher, to->publisher, 30);
	strncpy(from->author, to->author, 20);
	from->stock = to->stock;
	from->salePrice = to->salePrice;
	if (from->rentPrice != to->rentPrice)
	{
		from->rentPrice = to->rentPrice;
		bookCalcPay(from);
	}
	
	from->vipPrice = to->rentPrice * vipPayRate;
	strncpy(from->shelveDate, to->shelveDate, 11);
	strncpy(from->place, to->place, 5);
}

BookInfo *findBookInfoByNum(BookInfo *head, char *num)
{
	while (head != NULL)
	{
		if (!strcmp(head->bookNum, num))
			return head;
		
		head = head->next;
	}
	return NULL;
}

void addToRentList(BookInfo *parent, RentInfo *node)
{
	RentInfo *head = parent->rentList;
	
	parent->rentList = node;
	node->next = head;
	if (head != NULL)
		head->prev = node;
}

void bookCalcPay(BookInfo *book)
{
	RentInfo *node = book->rentList;
	BookType *parent;
	
	parent = findBookTypeByNum(book->type);
	while (node != NULL)
	{
		calcPay(parent, book, node);
		node = node->next;
	}
}

void freeBookNode(BookType *parent, BookInfo *book)
{
	if (parent->bookList == book)
		parent->bookList = book->next;
	else
		book->prev->next = book->next;
	
	if (book->next != NULL)
		book->next->prev = book->prev;
	
	freeRentList(book->rentList);
	g_free(book);
}