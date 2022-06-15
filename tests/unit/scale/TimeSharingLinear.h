#include "../../../core/graphics/scale/continuous/TimeSharingLinear.h"
#include "../../../core/utils/common.h"

using namespace xg;
using namespace std;

namespace unit {

class TimeSharingLinear {
  public:
    static bool WithoutTimeRange() {
        nlohmann::json values = {};
        nlohmann::json config = {};
        //无timeRange
        scale::TimeSharingLinear timeSharing("field", values, config);
        std::vector<scale::Tick> ticks = timeSharing.GetTicks(nullptr);
        return ticks.size() == 0;
    }

    static bool TimeRangeHS() {
        nlohmann::json values = {{1608687000000}};
        nlohmann::json config = {{"timeRange", {{1608687000000, 1608694200000}, {1608699600000, 1608706800000}}}};
        scale::TimeSharingLinear timeSharing("field", values, config);
        std::vector<scale::Tick> ticks = timeSharing.GetTicks(nullptr);
        return ticks.size() == 3 && ticks[0].text == "09:30" && ticks[1].text == "11:30/13:00" && ticks[2].text == "15:00";
    }

    static bool TimeRangeHSSameMiddleRange() {
        nlohmann::json values = {{1608687000000}};
        nlohmann::json config = {{"timeRange", {{1608687000000, 1608694200000}, {1608694200000, 1608706800000}}}};
        scale::TimeSharingLinear timeSharing("field", values, config);
        std::vector<scale::Tick> ticks = timeSharing.GetTicks(nullptr);
        return ticks.size() == 3 && ticks[0].text == "09:30" && ticks[1].text == "11:30" && ticks[2].text == "15:00";
    }

    static bool TimeRangeHK() {
        nlohmann::json values = {{1628731800000}};
        nlohmann::json config = {{"timeRange", {{1628731800000, 1628740800000}, {1628744400000, 1628755200000}}}};
        scale::TimeSharingLinear timeSharing("field", values, config);
        std::vector<scale::Tick> ticks = timeSharing.GetTicks(nullptr);
        return ticks.size() == 3 && ticks[0].text == "09:30" && ticks[1].text == "12:00/13:00" && ticks[2].text == "16:00";
    }

    static bool TimeRangeUS() {
        nlohmann::json values = {{1628688600000}};
        //时区转换 夏令时
        nlohmann::json config = {{"timeZoneOffset", -4 * 60 * 60}, {"timeRange", {{1628688600000, 1628712000000}}}};
        scale::TimeSharingLinear timeSharing("field", values, config);
        std::vector<scale::Tick> ticks = timeSharing.GetTicks(nullptr);
        return ticks.size() == 2 && ticks[0].text == "09:30" && ticks[1].text == "16:00";
    }

    static bool Scale() {
        nlohmann::json values = {{1608687000000}};
        nlohmann::json config = {{"timeRange", {{1608687000000, 1608694200000}, {1608699600000, 1608706800000}}}};
        scale::TimeSharingLinear timeSharing("field", values, config);
        auto valueSize = timeSharing.GetValuesSize() - 1;
        nlohmann::json key;
        auto ret0 = std::isnan(timeSharing.Scale(key));           // 空数据
        auto ret1 = std::isnan(timeSharing.Scale(1608686000000)); //错误的时间
        auto ret2 = timeSharing.Scale(1608687000000) == 0;        //第一个也是0.。。
        auto ret3 = xg::IsEqual(timeSharing.Scale(1608694200000), 120.0 / valueSize);
        auto ret4 = xg::IsEqual(timeSharing.Scale(1608699600000), 121.0 / valueSize);
        auto ret5 = xg::IsEqual(timeSharing.Scale(1608706800000), 241.0 / valueSize);
        return ret0 && ret1 && ret2 && ret3 && ret4 && ret5;
    }

    static bool Invert() {
        nlohmann::json values = {{1608687000000}};
        nlohmann::json config = {{"timeRange", {{1608687000000, 1608694200000}, {1608699600000, 1608706800000}}}};
        scale::TimeSharingLinear timeSharing("field", values, config);
        auto ret1 = timeSharing.Invert(0) == 1608687000000;
        auto ret2 = timeSharing.Invert(0.5) == 1608694200000;
        auto ret3 = timeSharing.Invert(1) == 1608706800000;
        auto ret4 = timeSharing.Invert(timeSharing.Scale(1608687060000)) == 1608687060000; // 09:31
        return ret1 && ret2 && ret3 && ret4;
    }
    
    static bool InvalidTimeRange() {
        nlohmann::json values = {{1608687000000}};
        //两个空数组
        nlohmann::json config = {{"timeRange", {{}, {}}}};
        scale::TimeSharingLinear timeSharing("field", values, config);
        bool ret1 = timeSharing.Invert(0) == 0;
        bool ret2 = std::isnan(timeSharing.Scale(1608687000000));
        bool ret3 = timeSharing.GetTickText(1608687000000, nullptr) == "";
        return ret1 && ret2 && ret3;
    }
    
    static bool InvalidTimeRange2() {
        nlohmann::json values = {{1608687000000}};
        //只有一个数组 数组中是string
        nlohmann::json config = {{"timeRange", {{"a", "b"}}}};
        scale::TimeSharingLinear timeSharing("field", values, config);
        bool ret1 = timeSharing.Invert(0) == 0;
        bool ret2 = std::isnan(timeSharing.Scale(1608687000000));
        bool ret3 = timeSharing.GetTickText(1608687000000, nullptr) == "";
        return ret1 && ret2 && ret3;
    }
    
    static bool InvalidTimeRange3() {
        nlohmann::json values = {{1608687000000}};
        //第一个数组有效 第二个数组无效
        nlohmann::json config = {{"timeRange", {{1608687000000, 1608694200000}, {}}}};
        scale::TimeSharingLinear timeSharing("field", values, config);
        bool ret1 = timeSharing.Invert(0) == 0;
        bool ret2 = std::isnan(timeSharing.Scale(1608687000000));
        bool ret3 = timeSharing.GetTickText(1608687000000, nullptr) == "";
        return ret1 && ret2 && ret3;
    }
};

}//unit
