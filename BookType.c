#include "struct.h"
#include "share.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include "BookType.h"

BookType *viewBookType(char *title, BookType *data)
{
	GtkWidget *dialog, *contentArea, *vbox;
	char text[100];
	GtkWidget *number, *name, *payRate, *vipDiscoutRate;
	BookType *retData = NULL;
	const gchar *textBuf;
	
	dialog = gtk_dialog_new_with_buttons(title, GTK_WINDOW(mainWindow), GTK_DIALOG_MODAL,
										 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, 
										 GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	
	contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
	
	addEntry(vbox, "    编号     ", number, 1);
	addEntry(vbox, "    名称     ", name, 10);
	addEntry(vbox, "   续费率   ", payRate, 10);
	addEntry(vbox, "会员折扣率", vipDiscoutRate, 10);
	
	if (data != NULL)										//设置显示内容
	{
		sprintf(text, "%c\0", data->number);
		gtk_entry_set_text(GTK_ENTRY(number), text);
		gtk_entry_set_text(GTK_ENTRY(name), data->name);
		sprintf(text, "%lf\0", data->payRate);
		gtk_entry_set_text(GTK_ENTRY(payRate), text);
		sprintf(text, "%lf\0", data->vipDiscoutRate);
		gtk_entry_set_text(GTK_ENTRY(vipDiscoutRate), text);
		gtk_widget_set_sensitive(number, FALSE);
	}
	
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
		retData = g_malloc(sizeof(BookType));
	
	if (data == NULL)
	{
		textBuf = gtk_entry_get_text(GTK_ENTRY(number));
		checkEntry(strlen(textBuf) == 0, dialog, "编号为空", lblRun);
		retData->number = textBuf[0];
	}
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(name));
	checkEntry(strlen(textBuf) == 0, dialog, "名称为空", lblRun);
	checkEntry(strlen(textBuf) > 10, dialog, "名称过长", lblRun);

	strncpy(retData->name, textBuf, 11);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(payRate));
	checkEntry(sscanf(textBuf, "%lf", &retData->payRate) == 0 || retData->payRate >= 1, 
	           dialog, "请输入正确的续费率", lblRun);
	
	textBuf = gtk_entry_get_text(GTK_ENTRY(vipDiscoutRate));
	checkEntry(sscanf(textBuf, "%lf", &retData->vipDiscoutRate) == 0, dialog,
			   "请输入正确的会员折扣率", lblRun);
	
	retData->next = NULL;
	retData->prev = NULL;
	retData->bookList = NULL;
	
	gtk_widget_destroy(dialog);
	
	return retData;
}

void addBookType(BookType *node)
{
	node->next = head;
	node->prev = NULL;
	if (head != NULL)
		head->prev = node;
	head = node;
}

void changeBookType(BookType *from, BookType *to)
{
	strncpy(from->name, to->name, 11);
	if (from->payRate != to->payRate)
	{
		from->payRate = to->payRate;
		typeCalcPay(from);
	}
	
	
	if (from->vipDiscoutRate != to->vipDiscoutRate)
	{
		from->vipDiscoutRate = to->vipDiscoutRate;
		calcVipPrice(from);
		typeCalcPay(from);
	}
}

void freeTypeList(BookType *list)
{
	BookType *temp;
	
	while (list != NULL)
	{
		freeBookList(list->bookList);
		temp = list->next;
		g_free(list);
		list = temp;
	}
}

BookType *findBookTypeByNum(char num)
{
	BookType *node = head;
	
	while (node != NULL)
	{
		if (node->number == num)
			return node;
		
		node = node->next;
	}
	
	return NULL;
}

void addToBookList(BookType *type, BookInfo *node)
{
	BookInfo *head = type->bookList;
	
	type->bookList = node;
	node->next = head;
	if (head != NULL)
		head->prev = node;
}

void freeTypeNode(BookType *node)
{
	if (node->prev == NULL)
		head = node->next;
	else
		node->prev->next = node->next;
	
	if (node->next != NULL)
		node->next->prev = node->prev;
	
	freeBookList(node->bookList);
	g_free(node);
}

void calcVipPrice(BookType *node)
{
	BookInfo *info = node->bookList;
	
	while (info != NULL)
	{
		info->vipPrice = info->rentPrice * node->vipDiscoutRate;
		info = info->next;
	}
}

void typeCalcPay(BookType *type)
{
	BookInfo *book = type->bookList;
	RentInfo *rent;
	
	while (book != NULL)
	{
		rent = book->rentList;
		
		while (rent != NULL)
		{
			calcPay(type, book, rent);
			rent = rent->next;
		}
		book = book->next;
	}
}