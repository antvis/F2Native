#include "StringUtil.h"

void StringUtil::SplitString(const std::string &s, std::vector<std::string> &sv, const char delim) {
        sv.clear();
        std::istringstream iss(s);
        std::string temp;

        while(std::getline(iss, temp, delim)) {
            sv.emplace_back(std::move(temp));
        }
}

std::vector<std::string> StringUtil::ParseFields(const std::string &field) {
        if(field.find('*') != std::string::npos) {
            std::vector<std::string> v;
            SplitString(field, v, '*');
            return v;
        } else {
            return {field};
        }
    }