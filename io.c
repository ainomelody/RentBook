#include "share.h"
#include "io.h"
#include "struct.h"
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>

static void loadRentInfo(RentInfo **list, FILE *fp)
{
	RentInfo *node, *tail = NULL;
	
	*list = NULL;
	node = g_malloc(sizeof(RentInfo));
	fread(node, sizeof(RentInfo), 1, fp);
	*list = tail = node;
	node->prev = NULL;
	
	while (node->next != NULL && !feof(fp))
	{
		node = g_malloc(sizeof(RentInfo));
		fread(node, sizeof(RentInfo), 1, fp);
		tail->next = node;
		node->prev = tail;
		tail = node;
	}
	
}

static void loadBookInfo(BookInfo **list, FILE *fp)
{
	BookInfo *node, *tail = NULL;
	
	*list = NULL;
	
	node = g_malloc(sizeof(BookInfo));
	fread(node, sizeof(BookInfo), 1, fp);
	*list = tail = node;
	node->prev = NULL;
	
	if (node->rentList != NULL)
		loadRentInfo(&node->rentList, fp);
	
	while (node->next != NULL && !feof(fp))
	{
		node = g_malloc(sizeof(BookInfo));
		fread(node, sizeof(BookInfo), 1, fp);
		tail->next = node;
		node->prev = tail;
		tail = node;
		
		if (node->rentList != NULL)
			loadRentInfo(&node->rentList, fp);
	}
}

void loadFromFile(char *filename)
{
	FILE *fp;
	BookType *node, *tail = NULL;
	
	fp = fopen(filename, "rb");
	
	if (fp == NULL)
	{
		char text[100];
		
		sprintf(text, "无法读取文件 %s\0", filename);
		messageBox(mainWindow, GTK_MESSAGE_ERROR, "错误", text);
		return;
	}
	
	freeTypeList(head);
	head = NULL;
	
	statusShow("正在读取...");
	
	node = g_malloc(sizeof(BookType));
	fread(node, sizeof(BookType), 1, fp);
	head = tail = node;
	node->prev = NULL;
	
	if (feof(fp))
	{
		statusShow("读取失败");
		g_free(node);
		head = NULL;
		return;
	}
	
	if (node->bookList != NULL)
		loadBookInfo(&node->bookList, fp);
	
	while (node->next != NULL && !feof(fp))
	{
		node = g_malloc(sizeof(BookType));
		fread(node, sizeof(BookType), 1, fp);
		tail->next = node;
		node->prev = tail;
		tail = node;
		
		if (node->bookList != NULL)
			loadBookInfo(&node->bookList, fp);
	}
	
	statusShow("完成");
	
}

void saveToFile(char *filename)
{
	FILE *fp;
	BookType *typeList;
	BookInfo *bookList;
	RentInfo *rentList;
	
	fp = fopen(filename, "wb");
	if (fp == NULL)							//对话框提示
	{
		char text[100];
		
		sprintf(text, "无法写入文件 %s\0", filename);
		messageBox(mainWindow, GTK_MESSAGE_ERROR, "错误", text);
										
		return;
	}
	
	statusShow("正在保存...");
	
	typeList = head;
	while (typeList != NULL)
	{
		fwrite(typeList, sizeof(BookType), 1, fp);
		bookList = typeList->bookList;
		
		while (bookList != NULL)
		{
			fwrite(bookList, sizeof(BookInfo), 1, fp);
			rentList = bookList->rentList;
			
			while (rentList != NULL)
			{
				fwrite(rentList, sizeof(RentInfo), 1, fp);
				rentList = rentList->next;
			}
			
			bookList = bookList->next;
		}
		
		typeList = typeList->next;
	}
	fclose(fp);
	
	statusShow("完成");
}