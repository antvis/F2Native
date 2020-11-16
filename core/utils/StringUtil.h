

#ifndef StringUtil_h
#define StringUtil_h

#include <regex>
#include <sstream>
#include <string.h>
#include <vector>

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
};

#endif /* StringUtil_h */
