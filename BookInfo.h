#ifndef BOOK_INFO_H__

#define BOOK_INFO_H__

#include "struct.h"

BookInfo *viewBookInfo(char *title, BookInfo *data);
void freeBookList(BookInfo *list);
void changeBookInfo(BookInfo *from, BookInfo *to);
BookInfo *findBookInfoByNum(BookInfo *head, char *num);
void addToRentList(BookInfo *parent, RentInfo *node);
void bookCalcPay(BookInfo *book);
void freeBookNode(BookType *parent, BookInfo *book);

#endif