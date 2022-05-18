#include <regex>
#include <sstream>
#include <string.h>
#include <vector>

#ifndef XG_UTILS_STRING_H
#define XG_UTILS_STRING_H

class StringUtil final {
  public:
    static void Split(const std::string &s, std::vector<std::string> &sv, const char delim = ' ') {
        sv.clear();
        std::istringstream iss(s);
        std::string temp;

        while(std::getline(iss, temp, delim)) {
            sv.emplace_back(std::move(temp));
        }
        return;
    }
    
    static std::vector<std::string> ParseFields(const std::string &field) {
        if(field.find('*') != field.npos) {
            std::vector<std::string> v;
            StringUtil::Split(field, v, '*');
            return v;
        } else {
            return {field};
        }
    }
};

#endif /* XG_UTILS_STRING_H */
