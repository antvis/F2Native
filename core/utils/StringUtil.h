#include <regex>
#include <sstream>
#include <string.h>
#include <vector>
#include <random>
#include <ctime>

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

    // 转换给定的 std::string 到小写
    static std::string ToLower(const std::string &input) {
        std::string output = input;
        for (char &c : output) {
            c = std::tolower(static_cast<unsigned char>(c));
        }
        return output;
    }

    static void ReplaceString(std::string &subject,
                                          const std::string &search,
                                          const std::string &replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length(); // 移动到下一个位置
        }
    }

    static bool BeginWithString(const std::string &color,
                                            const std::string &compareStr) {
        if (color.empty() || compareStr.empty()) {
            return false;
        }
        if (color.length() < compareStr.length()) {
            return false;
        }
        if (color.compare(0, compareStr.length(), compareStr) == 0) {
            return true;
        }
        return false;
    }

    static std::string Trim(const std::string &str, const char *whitespace = nullptr, bool trimStart = true, bool trimEnd = true) {
        if(str.empty()) {
            return str;
        }

        std::string target;
        if (whitespace == nullptr) {
            target = " \t";
        } else {
            target = whitespace;
        }

        unsigned long strBegin = 0;
        if (trimStart) {
            strBegin = str.find_first_not_of(target);
        }

        if (strBegin == std::string::npos) { // trim start时找不到有效字符，直接返回空串
            return "";
        }

        if (trimEnd) {
            auto strEnd = str.find_last_not_of(target);
            if (strEnd == std::string::npos) { // trim end时找不到有效字符，直接返回空串
                return "";
            }
            auto strRange = strEnd - strBegin + 1;
            // ALOGI("str=%s|strBegin=%i|strEnd=%i|strRange:%i", str.data(), strBegin, strEnd, strRange);
            return str.substr(strBegin, strRange);
        } else {
            return str.substr(strBegin);
        }
    }

    static std::string GenerateRandomString(size_t length) {
        const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::random_device rd;  // 用于生成随机种子
        std::mt19937 generator(rd());  // 使用Mersenne Twister引擎
        std::uniform_int_distribution<> distribution(0, characters.size() - 1);
        
        std::string randomString;
        for (size_t i = 0; i < length; ++i) {
            char randomChar = characters[distribution(generator)];
            randomString += randomChar;
        }

        return randomString;
    }
};

#endif /* XG_UTILS_STRING_H */
