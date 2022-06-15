#include "../../../core/graphics/scale/KLineCat.h"
#include <fstream>
#include <iostream>

using namespace xg;

namespace unit {

class KLineCat {
public:
    static bool kLineMinites5(const std::string &path = "../../res/scales/kline_minutes_5.json") {
        std::ifstream jsonFile(path);
        std::string json((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

        nlohmann::json values = nlohmann::json::parse(json);
        nlohmann::json config = {{"klineType", "kline-minutes-5"}, {"domain", {26, 68}}};
        scale::KLineCat klineCat("date", values, config);
        nlohmann::json ticks = klineCat.CalculateTicks();
        // std::cout << ticks.dump() << "---" << ticks.size() << std::endl;
        return (ticks.is_array()) &&
        (7 == ticks.size()) &&
        ("2021-01-08 11:30:00" == ticks[0].get<string>()) &&
        ("2021-01-11 10:30:59" == ticks[6].get<string>());
    };

    static bool kLineMinites60(const std::string &path = "../../res/scales/kline_minutes_60.json") {
        std::ifstream jsonFile(path);
        std::string json((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

        nlohmann::json values = nlohmann::json::parse(json);
        nlohmann::json config = {{"klineType", "kline-minutes-60"}, {"domain", {3, 102}}};
        scale::KLineCat klineCat("date", values, config);
        nlohmann::json ticks = klineCat.CalculateTicks();
        // std::cout << ticks.dump() << "---" << ticks.size() << std::endl;
        return (ticks.is_array()) && (4 == ticks.size());
    };

    static bool kLineDay(const std::string &path = "../../res/scales/kline_day.json") {
        std::ifstream jsonFile(path);
        std::string json((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

        nlohmann::json values = nlohmann::json::parse(json);
        nlohmann::json config = {{"klineType", "kline-day"}, {"domain", {3, 72}}};

        scale::KLineCat klineCat("date", values, config);
//        nlohmann::json ticks = klineCat.CalculateTicks();
//        std::cout << ticks.dump() << "---" << ticks.size() << std::endl;
//        return ticks.is_array();
        // EXPECT_EQ(4, ticks.size());
        return true;
    };

    static bool kLineDayScale(const std::string &path = "../../res/scales/kline_day.json") {
        std::ifstream jsonFile(path);
        std::string json((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

        nlohmann::json values = nlohmann::json::parse(json);
        nlohmann::json config = {{"klineType", "kline-day"}, {"domain", {3, 72}}};

        scale::KLineCat klineCat("date", values, config);
        nlohmann::json firstKey = values[3];
        double firstVal = klineCat.Scale(firstKey);
        return (0. == firstVal);
    };
};

}//unit
