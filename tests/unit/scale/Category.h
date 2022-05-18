#include "graphics/scale/Category.h"
#include "TestF2Function.h"
#include <assert.h>

using namespace xg;
using namespace std;

namespace unit {

class Category {
public:
    static bool TickCount0() {
        nlohmann::json values = {"2017", "2018", "2019", "2020", "2021"};
        nlohmann::json config = {{"tickCount", 0},};
        scale::Category cat("field", values, config);
        std::vector<scale::Tick> ticks = cat.GetTicks();
        return ticks.size() == 0;
    }
    
    static bool TickCountLess2() {
        nlohmann::json values = {"2017", "2018", "2019", "2020", "2021"};
        nlohmann::json config = {{"tickCount", 1},};
        scale::Category cat("field", values, config);
        std::vector<scale::Tick> ticks = cat.GetTicks();
        return ticks.size() == 5 && ticks[0].text == "2017" && ticks[4].text == "2021";
    }
    
    static bool ValueCountLess2() {
        nlohmann::json values = {"2017"};
        nlohmann::json config = {{"tickCount", 3},};
        scale::Category cat("field", values, config);
        std::vector<scale::Tick> ticks = cat.GetTicks();
        return ticks.size() == 1 && ticks[0].text == "2017";
    }
    
    static bool ValueCountZero() {
        nlohmann::json values = {};
        nlohmann::json config = {{"tickCount", 3},};
        scale::Category cat("field", values, config);
        std::vector<scale::Tick> ticks = cat.GetTicks();
        return ticks.size() == 0;
    }
    
    static bool TickCountZero() {
        nlohmann::json values = {"2017"};
        nlohmann::json config = {{"tickCount", 0},};
        scale::Category cat("field", values, config);
        std::vector<scale::Tick> ticks = cat.GetTicks();
        return ticks.size() == 0;
    }
    
    //保头保温算法
    static bool TickCountCatEnd() {
        nlohmann::json values = {"2017", "2018", "2019", "2020"};
        nlohmann::json config = {{"tickCount", 3},};
        scale::Category cat("field", values, config);
        std::vector<scale::Tick> ticks = cat.GetTicks();
        return ticks.size() == 3 && ticks[0].text == "2017" && ticks[2].text == "2020";
    }
    
    static bool TickCountCatEnd2() {
        nlohmann::json values = {"2014", "2015", "2016", "2017", "2018", "2019", "2020","2021", "2022", "2023", "2024", "2025"};
        nlohmann::json config = {{"tickCount", 8},};
        scale::Category cat("field", values, config);
        std::vector<scale::Tick> ticks = cat.GetTicks();
        //step =1 , ticks = 12
        return ticks.size() == values.size() && ticks[0].text == "2014" && ticks[values.size() - 1].text == values[values.size() -1];
    }
    
    static bool WithoutTickCount() {
        nlohmann::json values = {"2017", "2018", "2019", "2020"};
        nlohmann::json config = {};
        scale::Category cat("field", values, config);
        std::vector<scale::Tick> ticks = cat.GetTicks();
        return ticks.size() == 4 && ticks[0].text == "2017" && ticks[values.size() -1].text == values[values.size() - 1];
    }
    
//    static bool tickCountDomain() {
//        nlohmann::json values = {"2017", "2018", "2019", "2020"};
//        nlohmann::json config = {"domain", {0, 3}};
//        scale::Category cat("field", values, config);
//        std::vector<scale::Tick> ticks = cat.GetTicks();
//        return ticks.size() == 4 && ticks[0].text == "2017" && ticks[values.size() -1].text == values[values.size() - 1];
//    }
    
    static bool Scale() {
        nlohmann::json values = {"2017", "2018", "2019", "2020"};
        nlohmann::json config = {};
        scale::Category cat("field", values, config);
        nlohmann::json key;
        double first = cat.Scale("2017");
        double cached = cat.Scale("2017");
        double last = cat.Scale("2020");
        bool isnull = cat.Scale(key) == 0;
        return first == 0 && cached == 0 && last == 1 && isnull;
    }
    
    static bool ScaleEmptyValue() {
        nlohmann::json values = {};
        nlohmann::json config = {};
        scale::Category cat("field", values, config);
        //因为values为空，scale原值返回
        double string = cat.Scale("2017");
        double num = cat.Scale(2017);
        return string == 0 && num == 2017;
    }
    
    static bool Invert() {
        nlohmann::json values = {"2017", "2018", "2019", "2020"};
        nlohmann::json config = {};
        scale::Category cat("field", values, config);
        auto first = cat.Invert(0);
        auto last = cat.Invert(1);
        auto notExist = cat.Invert(-1);
        return first == "2017" && last == "2020" && notExist.is_null() == true;
    }
    
    static bool CustomTicks() {
        nlohmann::json values = {"2017", "2018", "2019", "2020"};
        nlohmann::json config = {{"ticks", {"2017", "2019"}}};
        scale::Category cat("field", values, config);
        std::vector<scale::Tick> ticks = cat.GetTicks();
        return ticks.size() == 2 && ticks[0].tickValue == "2017" && ticks[1].tickValue == "2019";
    }
    
    //设置了tick的callback
    static bool TicksCallback() {
        auto callback = [&](const nlohmann::json &param) {
            const std::string &value = param.get<std::string>();
            const nlohmann::json ret = {"content", value};
            assert((value == "2017"  || value == "2020"));
            return ret;
        };
        auto func = new TestF2Function(callback);
        xg::func::FunctionManager::GetInstance().Add(func);
        nlohmann::json values = {"2017", "2018", "2019", "2020"};
        nlohmann::json config = {{"tickCount", 2}, {"tick", func->functionId}};
        scale::Category cat("field", values, config);
        std::vector<scale::Tick> ticks = cat.GetTicks();
        xg::func::FunctionManager::GetInstance().Remove(func->functionId);
        delete func;
        return true;
    }
    
    static bool Performance(int size) {
        nlohmann::json values = {"2017", "2018", "2019", "2020"};
        nlohmann::json config = {};
        scale::Category cat("field", values, config);
        for(int i = 0; i < size; ++i) {
            cat.Scale("2017");
        }
        return true;
    }
};

}//unit


