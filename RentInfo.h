#ifndef RENT_INFO_H__

#define RENT_INFO_H__

#include "struct.h"

void freeRentList(RentInfo *list);
RentInfo *viewRentInfo(char *title, RentInfo *data);
void calcReturnDate(RentInfo *node);
void calcPay(BookType *type, BookInfo *book, RentInfo *node);
void changeRentInfo(RentInfo *from, RentInfo *to);

#endif