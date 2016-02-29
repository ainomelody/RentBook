#include <gtk/gtk.h>
#include "mainUI.h"
#include "share.h"
#include "callback.h"

static GtkWidget *createMainWindow();
static GtkWidget *createMenu();

GtkWidget *createMainUI()
{
	GtkWidget *box, *menu, *scrollWindow, *window;
	GtkTreeStore *store;
	
	window = createMainWindow();
	g_signal_connect(window, "destroy", G_CALLBACK(quit), NULL);
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	
	menu = createMenu();
	gtk_box_pack_start(GTK_BOX(box), menu, FALSE, TRUE, 0);
	
	scrollWindow = createScrollWindow();
	gtk_box_pack_start(GTK_BOX(box), scrollWindow, TRUE, TRUE, 0);
	
	treeView = createTreeView(1, FALSE);
	store = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeView), GTK_TREE_MODEL(store));
	g_signal_connect(treeView, "row-activated", G_CALLBACK(treeViewActive), NULL);
	gtk_container_add(GTK_CONTAINER(scrollWindow), treeView);
	
	statusBar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(box), statusBar, FALSE, TRUE, 2);
	
	gtk_container_add(GTK_CONTAINER(window), box);
	
	return window;
}

static GtkWidget *createMainWindow()
{
	GtkWidget *window;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "租书管理系统");
	gtk_window_set_default_size(GTK_WINDOW(window), 700, 400);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	return window;
}

static GtkWidget *createMenu()
{
	GtkWidget *menuBar, *rootMenu, *subMenu, *menuItem, *subMenu2, *subMenuItem;
	
	menuBar = gtk_menu_bar_new();
	
	/*文件菜单*/
	rootMenu = gtk_menu_item_new_with_label("文件(_F)");						//新建菜单项
	gtk_menu_item_set_use_underline(GTK_MENU_ITEM(rootMenu), TRUE);				//添加下划线
	subMenu = gtk_menu_new();													//创建子菜单容器
	
	menuItem = gtk_menu_item_new_with_label("加载(_L)...");
	gtk_menu_item_set_use_underline(GTK_MENU_ITEM(menuItem), TRUE);
	g_signal_connect(menuItem, "activate", G_CALLBACK(menuLoadCall), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), menuItem);					//将子菜单项加入容器
	
	menuItem = gtk_menu_item_new_with_label("保存(_S)...");
	gtk_menu_item_set_use_underline(GTK_MENU_ITEM(menuItem), TRUE);
	g_signal_connect(menuItem, "activate", G_CALLBACK(menuSaveCall), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), menuItem);
	
	menuItem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), menuItem);
	
	menuItem = gtk_menu_item_new_with_label("退出(_X)");
	gtk_menu_item_set_use_underline(GTK_MENU_ITEM(menuItem), TRUE);
	g_signal_connect(menuItem, "activate", G_CALLBACK(quit), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), menuItem);
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootMenu), subMenu);				//设置子菜单
	gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), rootMenu);					//将菜单加入菜单栏
	
	
	
	rootMenu = gtk_menu_item_new_with_label("编辑(_E)");
	gtk_menu_item_set_use_underline(GTK_MENU_ITEM(rootMenu), TRUE);
	subMenu = gtk_menu_new();
	
	menuItem = gtk_menu_item_new_with_label("添加");
	subMenu2 = gtk_menu_new();
	
	subMenuItem = gtk_menu_item_new_with_label("图书类别 ...");
	g_signal_connect(subMenuItem, "activate", G_CALLBACK(menuAddBookTypeCall), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu2), subMenuItem);
	
	subMenuItem = gtk_menu_item_new_with_label("图书 ...");
	g_signal_connect(subMenuItem, "activate", G_CALLBACK(menuAddBookInfoCall), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu2), subMenuItem);
	
	subMenuItem = gtk_menu_item_new_with_label("租阅信息 ...");
	g_signal_connect(subMenuItem, "activate", G_CALLBACK(menuAddRentInfoCall), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu2), subMenuItem);
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), subMenu2);
	
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), menuItem);
	
	menuItem = gtk_menu_item_new_with_label("删除");
	g_signal_connect(menuItem, "activate", G_CALLBACK(menuDeleteCall), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), menuItem);
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootMenu), subMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), rootMenu);
	
	
	
	rootMenu = gtk_menu_item_new_with_label("查询(_Q)");
	gtk_menu_item_set_use_underline(GTK_MENU_ITEM(rootMenu), TRUE);
	subMenu = gtk_menu_new();
	
	menuItem = gtk_menu_item_new_with_label("租阅详情 ...");
	g_signal_connect(menuItem, "activate", G_CALLBACK(menuQueryRentDetailCall), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), menuItem);
	
	menuItem = gtk_menu_item_new_with_label("租阅次数 ...");
	g_signal_connect(menuItem, "activate", G_CALLBACK(menuQueryRentTimesCall), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), menuItem);
	
	menuItem = gtk_menu_item_new_with_label("超期天数 ...");
	g_signal_connect(menuItem, "activate", G_CALLBACK(menuQueryExDaysCall), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), menuItem);
	
	menuItem = gtk_menu_item_new_with_label("借出还入数 ...");
	g_signal_connect(menuItem, "activate", G_CALLBACK(menuQueryInOutNumCall), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), menuItem);
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootMenu), subMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), rootMenu);
	
	return menuBar;
	
}