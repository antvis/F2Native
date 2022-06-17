 #include "../../../core/utils/StringUtil.h"

namespace unit {

class StringUtils {
public:
    static bool SplitFields1() {
        std::vector<std::string> rst;
        StringUtil::Split("x*y", rst, '*');
        return (2 == rst.size()) && ("x" == rst[0]) && ("y"== rst[1]);
    }
    
    static bool SplitFields2() {
        std::vector<std::string> rst;
        StringUtil::Split("color", rst, '*');
        return (1 == rst.size()) && ("color"== rst[0]);
    };

    static bool SplitDay() {
        std::vector<std::string> rst;
        StringUtil::Split("kline-day", rst, '-');
        return (2 == rst.size()) && ("kline" == rst[0]) && ("day"== rst[1]);
    };

    static bool SplitMinutes() {
        std::vector<std::string> rst;
        StringUtil::Split("kline-minutes-5", rst, '-');
        int minutes = std::stoi(rst[2]);
        return (3 == rst.size()) && ("kline" == rst[0]) && ("minutes" == rst[1]) && (5 == minutes);
     };

};

}

