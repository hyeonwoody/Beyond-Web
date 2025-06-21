#include "StringParser.h"
#include <sstream>
std::vector <std::string> StringParser::Split (std::string str, char delimiter){
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        if (!token.empty()) {
            result.push_back(token);
        }
    }
    return result;
}

bool StringParser::IsNumber(const std::string& str) {
    return !str.empty() && std::find_if(str.begin(), 
    str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
}