#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

std::string rtrim(const std::string& str, const std::string& trim_set = " \t\r\n\f\v");
std::string ltrim(const std::string& str, const std::string& trim_set = " \t\r\n\f\v");

std::string toUpperCase(const std::string& str);
std::string toLowerCase(const std::string& str);

#endif