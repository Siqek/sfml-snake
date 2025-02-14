#include "stdafx.hpp"
#include "utils/IniParser.hpp"
#include "utils/string_utils.hpp"

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
