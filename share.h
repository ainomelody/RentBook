#ifndef SHARE_H__

#define SHARE_H__

#include <gtk/gtk.h>
#include "struct.h"

#define addEntry(box, labelName, entryName, maxLength) { \
	GtkWidget *hbox, *label; \
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); \
	label = gtk_label_new(labelName); \
	entryName = gtk_entry_new(); \
	gtk_entry_set_max_length(GTK_ENTRY(entryName), maxLength); \
	gtk_widget_set_hexpand(entryName, TRUE); \
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, TRUE, 0); \
	gtk_box_pack_start(GTK_BOX(hbox), entryName, FALSE, TRUE, 0); \
	gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, TRUE, 0); \
}

#define checkEntry(condition, parent, tip, label) { \
	if (condition) \
	{ \
		messageBox(parent, GTK_MESSAGE_ERROR, "错误", tip); \
		goto label; \
	} \
}

typedef struct{
	GtkWidget *entry[2];
	GtkWidget *treeView;	
	GtkWidget *window;
}QueryStruct;

extern GtkWidget *mainWindow, *treeView, *statusBar;
extern BookType *head;

void statusShow(char *info);
void quit();
void messageBox(GtkWidget *parent, GtkMessageType type, char *title, char *message);
void reloadModel();
int checkDate(const char *date);
char *calcDate(const char *date, int num);
int calcDays(const char *start, const char *end);
GtkWidget *createDateEntry(GtkWidget *parent);
GtkWidget *createScrollWindow();
GtkWidget *createTreeView(int columnNum, gboolean showHeader);
int datecmp(const char *date1, const char *date2);
void setColumnName(GtkWidget *treeView, int column, const char *name);

#endif