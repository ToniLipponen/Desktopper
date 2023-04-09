#include "Signals.h"
#include "Application.h"

#include <fstream>
#include <filesystem>

void Activate(GtkApplication*, gpointer application)
{
    auto* app = static_cast<Application*>(application);
    app->CreateWidgets();
}

static void OnExecutableSelect(GObject* src, GAsyncResult* result, gpointer data)
{
    GError* error = nullptr;

    auto app = (Application*)data;
    auto file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(src), result, &error);

    if(error)
    {
        return;
    }

    auto input = GTK_EDITABLE(app->widgets.at("command-input"));
    gtk_editable_set_text(input, g_file_get_parse_name(file));
}

static void OnIconSelect(GObject* src, GAsyncResult* result, gpointer data)
{
    GError* error = nullptr;

    auto app = (Application*)data;
    auto file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(src), result, &error);

    if(error)
    {
        return;
    }

    auto filename = g_file_get_parse_name(file);
    auto input = GTK_EDITABLE(app->widgets.at("icon-input"));

    gtk_editable_set_text(input, filename);
}

static void OnEntrySelect(GObject* src, GAsyncResult* result, gpointer data)
{
    GError* error = nullptr;

    auto app = (Application*)data;
    auto file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(src), result, &error);

    if(error)
    {
        return;
    }

    auto filename = g_file_get_parse_name(file);
    puts(filename);

    app->LoadEntry(filename);
}

void OpenExecutable(GtkWidget*, gpointer userData)
{
    auto dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Select executable");
    gtk_file_dialog_open(dialog, nullptr, nullptr, OnExecutableSelect, userData);
}

void OpenIcon(GtkWidget*, gpointer userData)
{
    auto filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    auto filter = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(filter, "image/*");
    gtk_file_filter_set_name(filter, "Images");
    g_list_store_append(filters, filter);

    auto dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
    gtk_file_dialog_set_title(dialog, "Select icon");

    gtk_file_dialog_open(dialog, nullptr, nullptr, OnIconSelect, userData);
}

void CreateFile(GtkWidget*, gpointer userData)
{
    auto* app = (Application*)userData;

    const auto categories = app->GetCategories();
    const auto customCategories = app->GetEditableText("custom-categories");
    const bool terminal = app->GetChecked("terminal-check");
    const std::string name = app->GetEditableText("name-input");
    const std::string icon = app->GetEditableText("icon-input");
    const std::string comment = app->GetEditableText("comment-input");
    const std::string command = app->GetEditableText("command-input");
    const std::string homedir = g_get_home_dir();
    const std::string filename = name + ".desktop";
    const std::string filepath = homedir + "/.local/share/applications/" + filename;

    if(name.empty() || command.empty())
        return;

    std::ofstream file(filepath);
    file << "[Desktop Entry]\n";
    file << "Encoding=UTF-8\n";
    file << "Name=" << name << '\n';
    file << "Comment=" << comment << '\n';
    file << "Exec=" << command << '\n';
    file << "Icon=" << icon << '\n';
    file << "Terminal=" << (terminal ? "true" : "false") << '\n';
    file << "Type=Application\n";
    file << "Categories=";

    for(const auto& category : categories)
        file << category << ';';

    file << customCategories;

    file.close();

    if(app->GetChecked("desktop-check"))
    {
        try {
            std::string symlink = homedir + "/Desktop/" + filename;
            std::filesystem::create_symlink(filepath, symlink);
            std::filesystem::permissions(symlink, std::filesystem::perms::owner_all);
        }
        catch(...){}
    }
}

void OpenFile(GtkWidget*, gpointer userData)
{
    auto filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    auto filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.desktop");
    gtk_file_filter_set_name(filter, "Desktop file");
    g_list_store_append(filters, filter);

    auto dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
    gtk_file_dialog_set_title(dialog, "Open desktop entry");
    gtk_file_dialog_open(dialog, nullptr, nullptr, OnEntrySelect, userData);
}

void NewFile(GtkWidget*, gpointer userData)
{
    auto app = static_cast<Application*>(userData);

    app->Clear();
}