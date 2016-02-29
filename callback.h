#ifndef CALLBACK_H__

#define CALLBACK_H__

#include <gtk/gtk.h>

void menuLoadCall(GtkMenuItem *menu, gpointer data);
void menuSaveCall(GtkMenuItem *menu, gpointer data);
void menuAddBookTypeCall(GtkMenuItem *menu, gpointer data);
void menuAddBookInfoCall(GtkMenuItem *menu, gpointer data);
void menuAddRentInfoCall(GtkMenuItem *menu, gpointer data);
void menuDeleteCall(GtkMenuItem *menu, gpointer data);
void treeViewActive(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
void menuQueryRentDetailCall(GtkMenuItem *menu, gpointer user_data);
void queryRentDetailButtonCall(GtkButton *button, gpointer data);
void queryWindowDestroyCall(GtkWidget *window, gpointer data);
void menuQueryRentTimesCall();
void queryRentTimesButtonCall(GtkButton *button, gpointer data);
void menuQueryExDaysCall(GtkMenuItem *menu, gpointer user_data);
void queryInOutNumButtonCall(GtkButton *button, gpointer data);
void menuQueryInOutNumCall(GtkMenuItem *menu, gpointer user_data);

#endif