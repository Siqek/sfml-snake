#ifndef INIPARSER_HPP
#define INIPARSER_HPP

class IniParser
{
public:
    IniParser();
    IniParser(const std::string& filename);
    void loadFromFile(const std::string& filename);

    std::string getString(const std::string& section, const std::string& key, const std::string& defaultValue = "") const;
    int getInt(const std::string& section, const std::string& key, int defaultValue = 0) const;
    bool getBool(const std::string& section, const std::string& key, bool defaultValue = false) const;

    const std::unordered_map<std::string, std::string>& getSection(const std::string& section) const;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;
};

#endif
