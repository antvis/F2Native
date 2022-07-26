#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

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
    
    static string ToFixed(double val, int precision) {
        if(fabs(val) < XG_EPS) {
            return "0";
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << val;
        return ss.str();
    }
    
    static vector<string> ToStringVector(const vector<double> &vec) {
        vector<string> rst(vec.size());
        std::transform(vec.begin(), vec.end(), rst.begin(), [](const double& val) {
            return std::to_string(val);
        });
        return rst;
    }

    static vector<double> ToDoubleVector(const vector<string> &vec) {
        std::vector<double> rst(vec.size());
        std::transform(vec.begin(), vec.end(), rst.begin(), [](const std::string& val) {
            return std::stod(val);
        });
        return rst;
    }
};

#endif /* XG_UTILS_STRING_H */
