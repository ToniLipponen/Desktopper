#pragma once
#include <libadwaita-1/adwaita.h>
#include <gtk/gtk.h>
#include <string>
#include <map>
#include <vector>

class Application
{
public:
    Application();
    ~Application();

    gint Run(int argc, char** argv);
    void Clear();
    std::vector<std::string> GetCategories();
    void SetEditableText(const std::string& componentName, const std::string& value);
    void SetChecked(const std::string& compoentName, bool value);
    std::string GetEditableText(const std::string& componentName);
    bool GetChecked(const std::string& componentName);
    void LoadEntry(const std::string& path);

    friend void Activate(GtkApplication*, gpointer);
    std::map<std::string, GtkWidget*> widgets;
private:
    void CreateWidgets();
    void ConnectSignals();
private:
    GtkApplication* m_app;
};