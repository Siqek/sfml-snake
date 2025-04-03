#include "stdafx.hpp"
#include "utils/string_utils.hpp"

std::string utils::rtrim(const std::string& str, const std::string& trim_set)
{
    size_t end = str.find_last_not_of(trim_set);

    if (end == std::string::npos) {
        return "";
    }

    return str.substr(0, end + 1);
}

std::string utils::ltrim(const std::string& str, const std::string& trim_set)
{
    size_t start = str.find_first_not_of(trim_set);

    if (start == std::string::npos) {
        return "";
    }

    return str.substr(start);
}

std::string utils::trim(const std::string& str, const std::string& trim_set)
{
    return ltrim(rtrim(str, trim_set), trim_set);
}

std::string utils::toUpperCase(const std::string& str)
{
    std::string result = "";
    result.reserve(str.size());

    for (auto c : str) {
        result += toupper(c);
    }
    return result;
}

std::string utils::toLowerCase(const std::string& str)
{
    std::string result = "";
    result.reserve(str.size());

    for (auto c : str) {
        result += tolower(c);
    }
    return result;
}
