#include "Application.h"
#include "Signals.h"
#include "Incbin.h"
#include "EntryFile.h"
#include <algorithm>

extern "C"{
    INCBIN(char, UI, "../app.ui");
}

Application::Application()
{
    m_app = GTK_APPLICATION(adw_application_new("my.desktopper.app", G_APPLICATION_DEFAULT_FLAGS));
    g_signal_connect(m_app, "activate", G_CALLBACK(Activate), this);
}

Application::~Application()
{
    g_object_unref(m_app);
}

gint Application::Run(int argc, char** argv)
{
    return g_application_run(G_APPLICATION(m_app), argc, argv);
}

void Application::Clear()
{
    for(const auto& [id, widget] : widgets)
    {
        auto name = gtk_widget_get_name(widget);

        if(strcmp(name, "AdwEntryRow") == 0)
            SetEditableText(id, "");
        else if(strcmp(name, "GtkCheckButton") == 0)
            SetChecked(id, false);
    }

    SetChecked("menu-check", true);
    SetChecked("desktop-check", true);
}

std::vector<std::string> Application::GetCategories()
{
    std::vector<std::string> categories;

    if(GetChecked("audio-check"))       categories.emplace_back("Audio");
    if(GetChecked("graphics-check"))    categories.emplace_back("Graphics");
    if(GetChecked("game-check"))        categories.emplace_back("Game");
    if(GetChecked("network-check"))     categories.emplace_back("Network");
    if(GetChecked("utility-check"))     categories.emplace_back("Utility");
    if(GetChecked("office-check"))      categories.emplace_back("Office");
    if(GetChecked("settings-check"))    categories.emplace_back("Settings");

    return categories;
}

void Application::SetEditableText(const std::string &componentName, const std::string &value)
{
    auto editable = GTK_EDITABLE(widgets.at(componentName));
    gtk_editable_set_text(editable, value.c_str());
}

void Application::SetChecked(const std::string& componentName, bool value)
{
    auto checkButton = GTK_CHECK_BUTTON(widgets.at(componentName));
    gtk_check_button_set_active(checkButton, value);
}

std::string Application::GetEditableText(const std::string &componentName)
{
    auto editable = GTK_EDITABLE(widgets.at(componentName));
    return gtk_editable_get_text(editable);
}

bool Application::GetChecked(const std::string& componentName)
{
    return gtk_check_button_get_active(GTK_CHECK_BUTTON(widgets.at(componentName)));
}

void Application::LoadEntry(const std::string& path)
{
    EntryFile file;
    file.Parse(path);

    SetEditableText("name-input",       file.Get("Name"));
    SetEditableText("command-input",    file.Get("Exec"));
    SetEditableText("comment-input",    file.Get("Comment"));
    SetEditableText("icon-input",       file.Get("Icon"));
    SetChecked("terminal-check",        file.Get("Terminal"));
    SetChecked("menu-check",            !file.Get("NoDisplay"));

    std::vector<std::string> categories = file.Get("Categories");

    const auto beg = categories.begin();
    const auto end = categories.end();

    SetChecked("audio-check",       std::find(beg, end, "Audio"     ) != end);
    SetChecked("graphics-check",    std::find(beg, end, "Graphics"  ) != end);
    SetChecked("game-check",        std::find(beg, end, "Game"      ) != end);
    SetChecked("network-check",     std::find(beg, end, "Network"   ) != end);
    SetChecked("utility-check",     std::find(beg, end, "Utility"   ) != end);
    SetChecked("office-check",      std::find(beg, end, "Office"    ) != end);
    SetChecked("settings-check",    std::find(beg, end, "Settings"  ) != end);
}

static void AppendToMap(gpointer object, gpointer userData)
{
    auto app = (Application*)userData;
    app->widgets[gtk_buildable_get_buildable_id(GTK_BUILDABLE(object))] = GTK_WIDGET(object);
}

void Application::CreateWidgets()
{
    GtkBuilder* builder = gtk_builder_new_from_string(gUIData, gUISize);
    //GtkBuilder* builder = gtk_builder_new_from_file("../app.ui");

    auto objects = gtk_builder_get_objects(builder);
    g_slist_foreach(objects, AppendToMap, this);
    g_slist_free(objects);
    g_object_unref(builder);

    auto window = GTK_WINDOW(widgets.at("MainWindow"));
    gtk_window_set_title(window, "Desktopper");
    gtk_window_set_application(window, m_app);
    gtk_widget_set_visible(GTK_WIDGET(window), true);

    g_signal_connect(widgets.at("open-exec-button"),   "clicked", G_CALLBACK(OpenExecutable),   this);
    g_signal_connect(widgets.at("create-button"),      "clicked", G_CALLBACK(CreateFile),       this);
    g_signal_connect(widgets.at("open-icon-button"),   "clicked", G_CALLBACK(OpenIcon),         this);
    g_signal_connect(widgets.at("open-file-button"),   "clicked", G_CALLBACK(OpenFile),         this);
    g_signal_connect(widgets.at("new-file-button"),    "clicked", G_CALLBACK(NewFile),          this);
}