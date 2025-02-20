#include "stdafx.hpp"
#include "utils/IniParser.hpp"
#include "utils/string_utils.hpp"

IniParser::IniParser()
{
    this->data = {};
}

IniParser::IniParser(const std::string &filename)
{
    this->data = {};
    this->loadFromFile(filename);
}

void IniParser::loadFromFile(const std::string &filename)
{
    std::filesystem::path p(filename);
    std::ifstream file(p);

    if (!file.is_open()) {
        std::cerr << "IniParser::Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line = "";
    std::string current_section = "";
    while (std::getline(file, line))
    {
        // strip comments
        size_t pos = line.find_first_of(";#");
        if (pos != std::string::npos) {
            line = line.substr(0, pos);
        }

        // trim whitespaces
        line = rtrim(ltrim(line));

        if (line.empty()) continue;

        if (line[0] == '[' && line.back() == ']') {
            // section
            current_section = line.substr(1, line.length() - 2);
            continue;
        }

        // key-value pair
        std::istringstream ss(line);
        std::string key = "";
        std::string value = "";

        if (std::getline(ss, key, '=') && std::getline(ss, value)) {
            key = rtrim(key);
            value = ltrim(value);
            this->data[current_section][key] = value;
        } else {
            std::cerr << "IniParser::Invalid format in INI file. Expected: \'key=value\'. Invalid line: " << line << std::endl;
        }
    }

    file.close();
}

std::string IniParser::getString(const std::string& section, const std::string& key, const std::string& defaultValue) const
{
    auto it_section = this->data.find(section);
    if (it_section != this->data.end()) {
        auto it_value = it_section->second.find(key);
        if (it_value != it_section->second.end()) {
            return it_value->second;
        }
    }
    return defaultValue;
}

int IniParser::getInt(const std::string& section, const std::string& key, int defaultValue) const
{
    auto it_section = this->data.find(section);
    if (it_section != this->data.end()) {
        auto it_value = it_section->second.find(key);
        if (it_value != it_section->second.end()) {
            std::string str_value = it_value->second;

            try {
                size_t pos;
                int value = std::stoi(str_value, &pos);
                if (pos != str_value.size()) {
                    throw std::invalid_argument("Invalid argument");
                }
                return value;
            } catch (const std::exception& e) {
                std::cerr << "Error::IniParser::getInt::" << e.what() << '\n';
                return defaultValue;
            }

            return defaultValue;
        }
    }
    return defaultValue;
}

bool IniParser::getBool(const std::string& section, const std::string& key, bool defaultValue) const
{
    auto it_section = this->data.find(section);
    if (it_section != this->data.end()) {
        auto it_value = it_section->second.find(key);
        if (it_value != it_section->second.end()) {
            std::string str_value = toLowerCase(it_value->second);

            if (str_value == "true" || str_value == "1")
                return true;
            if (str_value == "false" || str_value == "0")
                return false;

            return defaultValue;
        }
    }
    return defaultValue;
}

const std::unordered_map<std::string, std::string>& IniParser::getSection(const std::string &section) const
{
    static const std::unordered_map<std::string, std::string> empty_map;
    auto it = this->data.find(section);
    if (it != this->data.end())
        return it->second;
    return empty_map;
}
