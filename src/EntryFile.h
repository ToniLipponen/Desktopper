#pragma once
#include <map>
#include <string>
#include <vector>
#include <sstream>

class EntryValue
{
public:
    friend class EntryFile;
    operator std::vector<std::string>()
    {
        std::vector<std::string> values;
        std::stringstream ss(m_str);

        for(std::string line; std::getline(ss, line, ';');)
        {
            values.push_back(std::move(line));
        }

        return values;
    }

    operator bool()
    {
        return m_str == "true";
    }

    operator std::string()
    {
        return m_str;
    }
private:
    EntryValue(const std::string& str) : m_str(str) {}
    const std::string& m_str;
};

class EntryFile
{
public:
    EntryFile();
    void Parse(const std::string& path);
    EntryValue Get(const std::string& name);
private:
    std::map<std::string, std::string> m_data;
};