#pragma once

#include "utils.h"

class StringHelper{
public:

    static std::vector<std::string> Split(const std::string& str, char splitChar){
        std::vector<std::string> ret;
        int start = 0;
        for (size_t i = 0, len = str.length(); i < len; i++){
            if (str[i] == splitChar){
                ret.push_back(str.substr(start, i - start));
                start = i + 1;
            }
        }
        ret.push_back(str.substr(start));
        return ret;
    }

    static std::string RemoveSpaces(const std::string& str){
        std::vector<char> ret;
        for (size_t i = 0, len = str.length(); i < len; i++){
            char c = str[i];
            if (c != ' ' && c != '\t' && c != '\n'&& c != '\r')
                ret.push_back(c);
        }
        return std::string(ret.begin(), ret.end());
    }

};
