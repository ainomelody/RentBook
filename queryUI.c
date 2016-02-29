#include "queryUI.h"
#include "share.h"
#include <gtk/gtk.h>
#include "callback.h"
#include "query.h"

GtkWidget *rentDetailUI()
{
	GtkWidget *window, *vbox, *hbox, *label, *button, *entry;
	GtkWidget *scrollWindow, *treeView;
	QueryStruct *st;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_title(GTK_WINDOW(window), "租阅详情查询");
	gtk_widget_set_size_request(window, 400, 300);
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	
	label = gtk_label_new("日期");
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, TRUE, 0);
	
	entry = createDateEntry(window);
	gtk_widget_set_hexpand(entry, TRUE);
	gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, TRUE, 0);
	
	
	button = gtk_button_new_with_label("查询");
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, TRUE, 0);
	
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 1);
	
	scrollWindow = createScrollWindow();
	treeView = createTreeView(2, TRUE);
	setColumnName(treeView, 0, "图书名称");
	setColumnName(treeView, 1, "在借数量");
	
	st = g_malloc(sizeof(QueryStruct));
	st->entry[0] = entry;
	st->treeView = treeView;
	st->window = window;
	g_signal_connect(button, "clicked", G_CALLBACK(queryRentDetailButtonCall), st);
	
	gtk_container_add(GTK_CONTAINER(scrollWindow), treeView);
	gtk_box_pack_start(GTK_BOX(vbox), scrollWindow, FALSE, TRUE, 0);
	
	gtk_container_add(GTK_CONTAINER(window), vbox);
	g_signal_connect(window, "destroy", G_CALLBACK(queryWindowDestroyCall), st);
	
	gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(mainWindow));
	gtk_window_set_destroy_with_parent(GTK_WINDOW(window), TRUE);					//和mainWindow同时销毁
	
	return window;
}

GtkWidget *rentTimesUI()
{
	GtkWidget *window, *vbox, *hbox, *frame, *label, *year, *month;
	GtkWidget *button, *scrollWindow, *treeView;
	QueryStruct *st;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "租阅次数统计");
	gtk_widget_set_size_request(window, 500, 400);
	
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
	
	frame = gtk_frame_new("月份");
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
	gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, TRUE, 0);
	
	button = gtk_button_new_with_label("查询");
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, TRUE, 0);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_widget_set_hexpand(hbox, TRUE);
	gtk_container_add(GTK_CONTAINER(frame), hbox);
	
	year = gtk_entry_new();
	gtk_widget_set_hexpand(year, TRUE);
	gtk_box_pack_start(GTK_BOX(hbox), year, FALSE, TRUE, 0);
	
	label = gtk_label_new("年");
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, TRUE, 0);
	
	month = gtk_entry_new();
	gtk_widget_set_hexpand(month, TRUE);
	gtk_box_pack_start(GTK_BOX(hbox), month, FALSE, TRUE, 0);
	
	label = gtk_label_new("月");
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, TRUE, 0);
	
	scrollWindow = createScrollWindow();
	treeView = createTreeView(2, TRUE);
	setColumnName(treeView, 0, "图书名称");
	setColumnName(treeView, 1, "租阅次数");
	
	st = g_malloc(sizeof(QueryStruct));
	st->entry[0] = year;
	st->entry[1] = month;
	st->treeView = treeView;
	st->window = window;
	g_signal_connect(button, "clicked", G_CALLBACK(queryRentTimesButtonCall), st);
	
	gtk_container_add(GTK_CONTAINER(scrollWindow), treeView);
	gtk_box_pack_start(GTK_BOX(vbox), scrollWindow, FALSE, TRUE, 2);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	g_signal_connect(window, "destroy", G_CALLBACK(queryWindowDestroyCall), st);
	
	gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(mainWindow));
	gtk_window_set_destroy_with_parent(GTK_WINDOW(window), TRUE);
	
	return window;
}

GtkWidget *exDaysUI()
{
	GtkWidget *window, *scrollWindow, *treeView;
	GtkListStore *store;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "超期天数统计");
	gtk_widget_set_size_request(window, 400, 400);
	
	scrollWindow = createScrollWindow();
	gtk_container_add(GTK_CONTAINER(window), scrollWindow);
	
	treeView = createTreeView(4, TRUE);
	setColumnName(treeView, 0, "姓名");
	setColumnName(treeView, 1, "书名");
	setColumnName(treeView, 2, "超期天数");
	setColumnName(treeView, 3, "续费");
	gtk_container_add(GTK_CONTAINER(scrollWindow), treeView);
	
	store = queryExDays();
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeView), GTK_TREE_MODEL(store));
	gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(mainWindow));
	gtk_window_set_destroy_with_parent(GTK_WINDOW(window), TRUE);
	
	return window;
}

GtkWidget *inOutNumUI()
{
	GtkWidget *window, *vbox, *hbox;
	GtkWidget *start, *end, *button, *label;
	GtkWidget *scrollWindow, *treeView;
	QueryStruct *st;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "租出还入数查询");
	gtk_widget_set_size_request(window, 500, 400);
	
	
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	
	start = createDateEntry(window);
	gtk_box_pack_start(GTK_BOX(hbox), start, FALSE, TRUE, 0);
	
	label = gtk_label_new("-");
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, TRUE, 0);
	
	end = createDateEntry(window);
	gtk_box_pack_start(GTK_BOX(hbox), end, FALSE, TRUE, 0);
	
	button = gtk_button_new_with_label("查询");
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, TRUE, 0);
	
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
	
	scrollWindow = createScrollWindow();
	gtk_box_pack_start(GTK_BOX(vbox), scrollWindow, FALSE, TRUE, 0);
	
	treeView = createTreeView(3, TRUE);
	setColumnName(treeView, 0, "书名");
	setColumnName(treeView, 1, "租出数");
	setColumnName(treeView, 2, "还入数");
	gtk_container_add(GTK_CONTAINER(scrollWindow), treeView);
	
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	st = g_malloc(sizeof(QueryStruct));
	st->entry[0] = start;
	st->entry[1] = end;
	st->treeView = treeView;
	st->window = window;
	
	g_signal_connect(button, "clicked", G_CALLBACK(queryInOutNumButtonCall), st);
	g_signal_connect(window, "destroy", G_CALLBACK(queryWindowDestroyCall), st);
	
	return window;
}