#include "TestF2Function.h"
#include "graphics/scale/continuous/Linear.h"
#include "utils/common.h"
#include <assert.h>

using namespace xg;
using namespace std;

namespace unit {

class Linear {
  public:
    static bool TickCount0() {
        nlohmann::json values = {10, 20, 30, 40, 50};
        //tickCount为0 返回min和max
        nlohmann::json config = {{"tickCount", 0}, {"min", 5}, {"max", 55}};
        scale::Linear linear("field", values, config);
        std::vector<scale::Tick> ticks = linear.GetTicks();
        return ticks.size() == 2 && ticks[0].tickValue == 5 && ticks[1].tickValue == 55;
    };
    
    
    static bool TickCountLess2() {
        nlohmann::json values = {10, 20, 30, 40, 50};
        nlohmann::json config = {{"tickCount", 1}, {"min", 5}, {"max", 55}};
        scale::Linear linear("field", values, config);
        std::vector<scale::Tick> ticks = linear.GetTicks();
        return ticks.size() == 2 && ticks[0].tickValue == 5 && ticks[1].tickValue == 55;
    };

    static bool Ticks() {
        nlohmann::json values = {10, 20, 30, 40, 50};
        nlohmann::json config = {{"tickCount", 3}, {"min", 10}, {"max", 50}};
        scale::Linear linear("field", values, config);
        std::vector<scale::Tick> ticks = linear.GetTicks();
        return ticks.size() == 3 && ticks[0].tickValue == 10 && ticks[2].tickValue == 50;
    };

    static bool TicksNice() {
        nlohmann::json values = {10, 20, 30, 40, 50, 60};
        nlohmann::json config = {{"tickCount", 4}, {"min", 10}, {"max", 60}, {"nice", true}};
        scale::Linear linear("field", values, config);
        std::vector<scale::Tick> ticks = linear.GetTicks();
        return ticks.size() == 4 && ticks[0].tickValue == 0 && ticks[3].tickValue == 60;
    };

    static bool TicksNice2() {
        nlohmann::json values = {10, 20, 30, 40, 50, 55};
        nlohmann::json config = {{"tickCount", 4}, {"min", 10}, {"max", 55}, {"nice", true}};
        scale::Linear linear("field", values, config);
        std::vector<scale::Tick> ticks = linear.GetTicks();
        return ticks.size() == 4 && ticks[0].tickValue == 0 && ticks[3].tickValue == 60;
    };

    static bool TicksNice3() {
        nlohmann::json values = {10, 20, 30, 40, 50, 55};
        nlohmann::json config = {{"tickCount", 2}, {"min", 10}, {"max", 55}, {"nice", true}};
        scale::Linear linear("field", values, config);
        std::vector<scale::Tick> ticks = linear.GetTicks();
        return ticks.size() == 2;
    };

    static bool TicksNice4() {
        nlohmann::json values = {
            0.1,
            0.2,
            -0.1,
        };
        // tickCount=2 ticks返回nan
        nlohmann::json config = {{"tickCount", 2}, {"nice", true}, {"min", -0.1}, {"max", 0.2}, {"precision", 1}};
        scale::Linear linear("field", values, config);
        std::vector<scale::Tick> ticks = linear.GetTicks();
        return ticks.size() == 2 && ticks[0].text == "-0.1" && ticks[1].text == "0.2";
    };

    //覆盖nan和cached scale
    static bool Scale() {
        nlohmann::json values = {10, 20, 30, 40, 50, 55};
        nlohmann::json config = {{"tickCount", 2}, {"min", 10}, {"max", 55}, {"nice", true}, {"precision", 2}};
        scale::Linear linear("field", values, config);
        bool isNana = std::isnan(linear.Scale("hello word"));
        linear.Scale(10);

        nlohmann::json key;
        bool isNana2 = std::isnan(linear.Scale(key));

        //scale int和double的度量应该相等
        double scaleUnsignInt = linear.Scale(10);
        double scaleDouble = linear.Scale(10.0);
        bool isSame = IsEqual(scaleUnsignInt, scaleDouble);
        
        //unsigned_int 0 , float 65应该不相等， 单测scale中的hash计算逻辑
        double scaleDouble0 = linear.Scale(0.0);
        double scaleUnsignedint = linear.Scale(65);
        bool isDiff = !IsEqual(scaleDouble0, scaleUnsignedint);
        
        bool isText1 = linear.GetTickText("20") == "20";
        bool isText2 = linear.GetTickText(20) == "20";
        bool isText3 = linear.GetTickText(20.02) == "20.02";
        bool isText4 = linear.GetTickText({"hello", "world"}) == "";
        return isNana && isNana2 && isText1 && isText2 && isText3 && isText4 && isSame && isDiff;
    }

    static bool ScaleEqualMinAndMax() {
        nlohmann::json values = {55};
        nlohmann::json config = {{"tickCount", 2}, {"min", 55}, {"max", 55}, {"nice", false}};
        scale::Linear linear("field", values, config);
        // nice max 60 min 55
        bool isMin = linear.Scale(55) == 0.5;
        return isMin;
    }

    static bool ScaleWithOutMinAndMAX() {
        nlohmann::json values = {55};
        nlohmann::json config = {{"tickCount", 2}, {"nice", false}};
        scale::Linear linear("field", values, config);
        //只有一个数据的时候linear的max 1 min 0， scale 55 返回为1
        bool isMax = linear.Scale(55) == 1;
        bool isMin = linear.Scale(-1) == 0;
        return isMax && isMin;
    }

    static bool Invert() {
        nlohmann::json values = {10, 20, 30, 40, 50};
        nlohmann::json config = {{"tickCount", 2}, {"min", 10}, {"max", 50}, {"nice", false}, {"precision", 2}};
        scale::Linear linear("field", values, config);
        bool isMin = linear.Invert(0) == 10;
        bool isMax = linear.Invert(1) == 50;
        bool isMiddel = linear.Invert(0.5) == 30;
        return isMin && isMax && isMiddel;
    }

    //设置了tick的callback
    static bool TicksCallback() {
        auto callback = [&](const nlohmann::json &param) {
            double value = param.get<double>();
            const nlohmann::json ret = {"content", value};
            assert(xg::IsEqual(value, 0) || xg::IsEqual(value, 60));
            return ret;
        };
        auto func = new TestF2Function(callback);
        xg::func::FunctionManager::GetInstance().Add(func);
        nlohmann::json values = {10, 20, 30, 40, 50, 55};
        nlohmann::json config = {{"tickCount", 2}, {"min", 10}, {"max", 55}, {"nice", true}, {"tick", func->functionId}};
        scale::Linear linear("field", values, config);
        std::vector<scale::Tick> ticks = linear.GetTicks();
        xg::func::FunctionManager::GetInstance().Remove(func->functionId);
        delete func;
        return true;
    }
    
    static bool Performance(int size) {
        nlohmann::json values = {10, 20, 30, 40, 50, 55};
        nlohmann::json config = {{"tickCount", 2}, {"min", 10}, {"max", 55}, {"nice", true}, {"precision", 2}};
        scale::Linear linear("field", values, config);
        for(int i = 0; i< size; ++i) {
            linear.Scale(10);
        }
        return true;
    }
};

} //unit


