#include "RentInfo.h"
#include "struct.h"
#include <gtk/gtk.h>
#include "share.h"
#include <string.h>
#include "BookType.h"
#include "BookInfo.h"

void freeRentList(RentInfo *list)
{
	RentInfo *temp;
	
	while (list != NULL)
	{
		temp = list->next;
		g_free(list);
		list = temp;
	}
}

RentInfo *viewRentInfo(char *title, RentInfo *data)
{
	GtkWidget *dialog, *contentArea, *vbox;
	GtkWidget *type, *bookNum, *name, *id, *rentDate, *keepDays;
	GtkWidget *spReturnDate, *returnDate, *additionalPay;
	RentInfo *retData = NULL;
	char text[100];
	const char *textBuf;
	
	dialog = gtk_dialog_new_with_buttons(title, GTK_WINDOW(mainWindow), GTK_DIALOG_MODAL,
										 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, 
										 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	gtk_window_set_default_size(GTK_WINDOW(dialog), 400, -1);
	
	contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	
	addEntry(vbox, "图书类别", type, 1);
	addEntry(vbox, "图书编号", bookNum, 11);
	addEntry(vbox, "   姓名   ", name, 30);
	addEntry(vbox, " 身份证  ", id, 19);
	addEntry(vbox, "租书日期", rentDate, 11);
	addEntry(vbox, "可租天数", keepDays, 10);
	addEntry(vbox, "应还日期", spReturnDate, 11);
	addEntry(vbox, "归还日期", returnDate, 11);
	addEntry(vbox, "   续费   ", additionalPay, 50);
	
	if (data != NULL)
	{
		sprintf(text, "%c\0", data->type);
		gtk_entry_set_text(GTK_ENTRY(type), text);
		gtk_entry_set_text(GTK_ENTRY(name), data->name);
		gtk_entry_set_text(GTK_ENTRY(bookNum), data->bookNum);
		gtk_entry_set_text(GTK_ENTRY(id), data->id);
		gtk_entry_set_text(GTK_ENTRY(rentDate), data->rentDate);
		gtk_entry_set_text(GTK_ENTRY(spReturnDate), data->spReturnDate);
		gtk_entry_set_text(GTK_ENTRY(returnDate), data->returnDate);
		sprintf(text, "%d\0", data->keepDays);
		gtk_entry_set_text(GTK_ENTRY(keepDays), text);
		sprintf(text, "%lf\0", data->additionalPay);
		gtk_entry_set_text(GTK_ENTRY(additionalPay), text);
		gtk_widget_set_sensitive(type, FALSE);
		gtk_widget_set_sensitive(bookNum, FALSE);
	}
	gtk_widget_set_sensitive(spReturnDate, FALSE);
	gtk_widget_set_sensitive(additionalPay, FALSE);
	
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
		retData = g_malloc(sizeof(RentInfo));
	
	if (data == NULL)
	{
		textBuf = gtk_entry_get_text(GTK_ENTRY(type));
		checkEntry(strlen(textBuf) == 0, dialog, "图书类别为空", lblRun);
		retData->type = textBuf[0];
		textBuf = gtk_entry_get_text(GTK_ENTRY(bookNum));
		checkEntry(strlen(textBuf) == 0, dialog, "图书编号为空", lblRun);
		strncpy(retData->bookNum, textBuf, 12);
	}
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(name));
	checkEntry(strlen(textBuf) == 0, dialog, "姓名为空", lblRun);
	checkEntry(strlen(textBuf) > 30, dialog, "姓名过长", lblRun);
	strncpy(retData->name, textBuf, 31);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(id));
	checkEntry(strlen(textBuf) == 0, dialog, "身份证号为空", lblRun);
	strncpy(retData->id, textBuf, 20);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(rentDate));
	checkEntry(strlen(textBuf) == 0 || !checkDate(textBuf),
	           dialog, "租书日期不正确", lblRun);
	strncpy(retData->rentDate, textBuf, 12);
			   
	textBuf = gtk_entry_get_text(GTK_ENTRY(keepDays));
	checkEntry(sscanf(textBuf, "%d", &retData->keepDays) == 0 || retData->keepDays <= 0,
	           dialog, "可租天数不正确", lblRun);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(returnDate));
	checkEntry(strlen(textBuf) == 0 || !checkDate(textBuf) 
	           || datecmp(retData->rentDate, textBuf) > 0,
	           dialog, "归还日期不正确", lblRun);
	strncpy(retData->returnDate, textBuf, 12);
	
	retData->next = NULL;
	retData->prev = NULL;
	gtk_widget_destroy(dialog);
	return retData;
}

void calcReturnDate(RentInfo *node)
{
	char *date;
	
	date = calcDate(node->rentDate, node->keepDays);
	strncpy(node->spReturnDate, date, 12);
	g_free(date);
}

void calcPay(BookType *type, BookInfo *book, RentInfo *node)
{
	node->additionalPay = type->payRate * type->vipDiscoutRate * book->rentPrice * calcDays(node->spReturnDate, node->returnDate);
}

void freeRentNode(BookInfo *parent, RentInfo *node)
{
	if (parent->rentList == node)
		parent->rentList = node->next;
	else
		node->prev->next = node->next;
	
	if (node->next != NULL)
		node->next->prev = node->prev;
	g_free(node);
}

void changeRentInfo(RentInfo *from, RentInfo *to)
{
	BookType *type;
	BookInfo *book;
	
	strcpy(from->name, to->name);
	strcpy(from->id, to->id);
	strcpy(from->rentDate, to->rentDate);
	from->keepDays = to->keepDays;
	strcpy(from->returnDate, to->returnDate);
	calcReturnDate(from);
	type = findBookTypeByNum(from->type);
	book = findBookInfoByNum(type->bookList, from->bookNum);
	calcPay(type, book, from);
}