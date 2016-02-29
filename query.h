#ifndef COUNT_H__

#define COUNT_H__

#include <gtk/gtk.h>

GtkListStore *queryRentDetail(const char *date);
GtkListStore *queryRentTimes(int year, int month);
GtkListStore *queryExDays();
GtkListStore *queryInOutNum(const char *start, const char *end);

#endif