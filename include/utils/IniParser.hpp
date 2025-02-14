#ifndef INIPARSER_HPP
#define INIPARSER_HPP

class IniParser
{
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;
public:
    void loadFromFile(const std::string& filename);
};

#endif
