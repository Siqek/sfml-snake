#include "stdafx.hpp"
#include "utils/string_utils.hpp"

std::string rtrim(const std::string &str, const std::string& trim_set)
{
    size_t end = str.find_last_not_of(trim_set);

    if (end == std::string::npos) {
        return "";
    }

    return str.substr(0, end + 1);
}

std::string ltrim(const std::string &str, const std::string& trim_set)
{
    size_t start = str.find_first_not_of(trim_set);

    if (start == std::string::npos) {
        return "";
    }

    return str.substr(start);
}
