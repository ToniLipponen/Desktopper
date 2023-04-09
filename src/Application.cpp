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
    SetEditableText("name-input",       "");
    SetEditableText("command-input",    "");
    SetEditableText("comment-input",    "");
    SetEditableText("icon-input",       "");

    SetChecked("terminal-check",    false);
    SetChecked("menu-check",        true);
    SetChecked("audio-check",       false);
    SetChecked("graphics-check",    false);
    SetChecked("game-check",        false);
    SetChecked("network-check",     false);
    SetChecked("utility-check",     false);
    SetChecked("office-check",      false);
    SetChecked("settings-check",    false);
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

void Application::CreateWidgets()
{
    GtkBuilder* builder = gtk_builder_new_from_string(gUIData, gUISize);
    //GtkBuilder* builder = gtk_builder_new_from_file("../app.ui");
    GObject* window = gtk_builder_get_object(builder, "MainWindow");

    gtk_window_set_title(GTK_WINDOW(window), "Desktopper");
    gtk_window_set_application(GTK_WINDOW(window), m_app);
    gtk_widget_set_visible(GTK_WIDGET(window), true);

    auto GetWidget = [&](const char* name)
    {
        widgets[name] = GTK_WIDGET(gtk_builder_get_object(builder, name));
    };

    auto GetWidgets = [&](const std::vector<std::string>& names)
    {
        for(auto& name : names)
            GetWidget(name.c_str());

        g_object_unref(builder);
    };

    GetWidgets({
        "name-input",
        "icon-input",
        "command-input",
        "comment-input",
        "create-button",
        "open-file-button",
        "new-file-button",
        "desktop-check",
        "terminal-check",
        "menu-check",
        "open-exec-button",
        "open-icon-button",
        "audio-check",
        "graphics-check",
        "game-check",
        "network-check",
        "utility-check",
        "office-check",
        "settings-check",
        "custom-categories",
    });

    ConnectSignals();
}

void Application::ConnectSignals()
{
    auto ConnectWidget = [&](const char* name, const char* event, void(callback)(GtkWidget*, gpointer))
    {
        g_signal_connect(widgets.at(name), event, G_CALLBACK(callback), this);
    };

    ConnectWidget("open-exec-button",   "clicked", OpenExecutable);
    ConnectWidget("create-button",      "clicked", CreateFile);
    ConnectWidget("open-icon-button",   "clicked", OpenIcon);
    ConnectWidget("open-file-button",   "clicked", OpenFile);
    ConnectWidget("new-file-button",    "clicked", NewFile);
}