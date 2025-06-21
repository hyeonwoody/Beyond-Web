#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

namespace StringParser{
    std::string PhraseParse (int start, int end, std::vector<std::string> vphrase);
    std::string Tail (std::string source, unsigned short length);

    std::vector <std::string> SpaceSpliter (std::string str, char delimeter=',');
    std::vector <std::string> Split(std::string str, char delimeter=',');
    bool IsNumber(const std::string& str);
};
