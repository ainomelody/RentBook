#ifndef BOOK_TYPE_H__

#define BOOK_TYPE_H__

#include "struct.h"
#include <gtk/gtk.h>

BookType *viewBookType(char *title, BookType *data);
void addBookType(BookType *node);
GtkTreeStore *bookTypeModel();
void changeBookType(BookType *from, BookType *to);
void freeTypeList(BookType *list);
BookType *findBookTypeByNum(char num);
void addToBookList(BookType *type, BookInfo *node);
void freeTypeNode(BookType *node);
void calcVipPrice(BookType *node);
void typeCalcPay(BookType *type);

#endif