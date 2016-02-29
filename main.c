#include "struct.h"
#include "mainUI.h"
#include <gtk/gtk.h>
#include "share.h"
#include "BookType.h"
#include "BookInfo.h"
#include "RentInfo.h"

GtkWidget *mainWindow, *treeView, *statusBar;
BookType *head = NULL;

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	mainWindow = createMainUI();
	
	gtk_widget_show_all(mainWindow);
	gtk_main();
	return 0;
}