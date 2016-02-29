#include <gtk/gtk.h>
#include "struct.h"
#include "share.h"

GtkListStore *queryRentDetail(const char *date)
{
	GtkListStore *store;
	int number;
	BookType *type = head;
	BookInfo *book;
	RentInfo *rent;
	GtkTreeIter iter;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	while (type != NULL)
	{
		book = type->bookList;
		
		while (book != NULL)
		{
			rent = book->rentList;
			number = 0;
			
			if (datecmp(date, book->shelveDate) > 0)
			{
				while (rent != NULL)
				{
					if (datecmp(date, rent->rentDate) > 0 
					    && datecmp(date, rent->returnDate) < 0)
						number++;
					rent = rent->next;
				}
			}
			
			if (number > 0)
			{
				gtk_list_store_append(store, &iter);
				gtk_list_store_set(store, &iter, 0, book->bookName, 1, number, -1);
			}
			book = book->next;
		}
		
		type = type->next;
	}
	
	return store;
}

GtkListStore *queryRentTimes(int year, int month)
{
	GtkListStore *store;
	BookType *type = head;
	BookInfo *book;
	RentInfo *rent;
	GtkTreeIter iter, newiter;
	int number;
	char start[20];
	char end[20];
	int foundNext, value;
	gboolean findFirst;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	sprintf(start, "%d/%d/1", year, month);
	sprintf(end, "%d/%d/31", year, month);
	
	while (type != NULL)
	{
		book = type->bookList;
		
		while (book != NULL)
		{
			number = 0;
			rent = book->rentList;
			
			while (rent != NULL)
			{
				if (datecmp(start, rent->rentDate) <= 0 && datecmp(end, rent->rentDate) >= 0)
					number++;
				
				rent = rent->next;
			}
			
			findFirst = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
			
			if (!findFirst)
			{
				gtk_list_store_append(store, &iter);
				gtk_list_store_set(store, &iter, 0, book->bookName, 1, number, -1);
			}
			else
			{
				gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 1, &value, -1);
				foundNext = 1;
				
				while (number < value && (foundNext = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter)))
					gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 1, &value, -1);
				
				if (!foundNext)				//在最后插入
				{
					gtk_list_store_append(store, &newiter);
					gtk_list_store_set(store, &newiter, 0, book->bookName, 1, number, -1);
				}
				else
				{
					gtk_list_store_insert_before(store, &newiter, &iter);
					gtk_list_store_set(store, &newiter, 0, book->bookName, 1, number, -1);
				}
			}
			
			book = book->next;
		}
		type = type->next;
	}
	
	return store;
}

GtkListStore *queryExDays()
{
	GtkListStore *store;
	BookType *type = head;
	BookInfo *book;
	RentInfo *rent;
	GtkTreeIter newiter, iter;
	int foundNext, value, number;
	gboolean findFirst;
	
	store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_DOUBLE);
	
	while (type != NULL)
	{
		book = type->bookList;
		
		while (book != NULL)
		{
			rent = book->rentList;
			
			while (rent != NULL)
			{
				if (rent->additionalPay != 0)
				{
					findFirst = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
					
					if (!findFirst)
					{
						gtk_list_store_append(store, &newiter);
						gtk_list_store_set(store, &newiter, 0, rent->name, 1, book->bookName, 
						                   2, calcDays(rent->spReturnDate, rent->returnDate), 
										   3, rent->additionalPay, -1);
					}
					else
					{
						gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 2, &value, -1);
						foundNext = 1;
						number = calcDays(rent->spReturnDate, rent->returnDate);
						
						while (number < value && (foundNext = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter)))
							gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 2, &value, -1);
						
						if (!foundNext)				//在最后插入
						{
							gtk_list_store_append(store, &newiter);
							gtk_list_store_set(store, &newiter, 0, rent->name, 1, book->bookName, 
						                       2, calcDays(rent->spReturnDate, rent->returnDate), 
										       3, rent->additionalPay, -1);
						}
						else
						{
							gtk_list_store_insert_before(store, &newiter, &iter);
							gtk_list_store_set(store, &newiter, 0, rent->name, 1, book->bookName, 
						                       2, calcDays(rent->spReturnDate, rent->returnDate), 
										       3, rent->additionalPay, -1);
						}
					}
				}
				
				rent = rent->next;
			}
			
			book = book->next;
		}
		
		type = type->next;
	}
	
	return store;
}

GtkListStore *queryInOutNum(const char *start, const char *end)
{
	GtkListStore *store;
	BookType *type = head;
	BookInfo *book;
	RentInfo *rent;
	int inNum, outNum;
	GtkTreeIter iter;
	
	store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);
	while (type != NULL)
	{
		book = type->bookList;
		
		while (book != NULL)
		{
			rent = book->rentList;
			inNum = outNum = 0;
			
			while (rent != NULL)
			{
				if (datecmp(start, rent->rentDate) <= 0 && datecmp(end, rent->rentDate) >= 0)
					outNum++;
				if (datecmp(start, rent->returnDate) <= 0 && datecmp(end, rent->returnDate) >= 0)
					inNum++;
				
				rent = rent->next;
			}
			
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, 0, book->bookName, 1, outNum, 2, inNum, -1);
			
			book = book->next;
		}
		
		type = type->next;
	}
	
	return store;
}