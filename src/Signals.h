#pragma once
#include <gtk/gtk.h>

void Activate(GtkApplication* app, gpointer userData);
void OpenExecutable(GtkWidget*, gpointer userData);
void OpenIcon(GtkWidget*, gpointer userData);
void CreateFile(GtkWidget*, gpointer userData);
void OpenFile(GtkWidget*, gpointer userData);
void NewFile(GtkWidget*, gpointer userData);
