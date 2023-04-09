#include "EntryFile.h"
#include <fstream>
#include <filesystem>
#include <glib.h>

EntryFile::EntryFile() = default;

void EntryFile::Parse(const std::string& path)
{
    if(!std::filesystem::exists(path))
    {
        return;
    }

    bool hasHeader = false;
    std::stringstream content;
    std::ifstream file(path);

    for(std::string line; std::getline(file, line);)
    {
        if(line.find('#') != std::string::npos)
            continue;

        if(line == "[Desktop Entry]")
        {
            hasHeader = true;
            continue;
        }

        content << line << '\n';
    }

    if(!hasHeader)
    {
        g_message("File did not contain a [Desktop Entry] header");
        return;
    }

    for(std::string line; std::getline(content, line);)
    {
        std::string key, value;
        std::stringstream string(line);

        std::getline(string, key, '=');
        std::getline(string, value);

        m_data[key] = value;
    }
}

EntryValue EntryFile::Get(const std::string& name)
{
    try {
        return m_data.at(name);
    } catch(...) {
        return EntryValue("");
    }
}