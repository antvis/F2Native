#include "../../../core/graphics/scale/ScaleController.h"
#include "../../../core/graphics/canvas/Cartesian.h"
#include "../../../core/utils/common.h"

using namespace xg;
using namespace std;

namespace unit {

class ScaleController {
public:
    static std::unique_ptr<canvas::coord::AbstractCoord> MakeCoord(util::Point start = util::Point(0, 0), util::Point end = util::Point(0, 0)) {
        return xg::make_unique<canvas::coord::Cartesian>(start, end);
    }
    
    static utils::Tracer *MakeTracer() {
        return new utils::Tracer("f2native");
    }
    
    //通过空数据创建scale
    static bool MakeScaleWithEmptyData() {
        nlohmann::json data = {};
        scale::ScaleController ctrl(nullptr);
        auto coord = ScaleController::MakeCoord();
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isIdentity = (*scale).GetType() == scale::ScaleType::Identity;
        return isIdentity;
    }
    
    //通过指定的数据创建scale
    static bool MakeLinearScale() {
        nlohmann::json data = {{{"field", 1}}};
        scale::ScaleController ctrl(nullptr);
        auto coord = ScaleController::MakeCoord();
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isLinear = (*scale).GetType() == scale::ScaleType::Linear;
        return isLinear;
    }
    
    //通过指定的数据,数据是数组创建scale
    static bool MakeLinearScaleByArray() {
        nlohmann::json data = {{{"field", {1,2,3,4}}}};
        scale::ScaleController ctrl(nullptr);
        auto coord = ScaleController::MakeCoord();
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isLinear = (*scale).GetType() == scale::ScaleType::Linear;
        return isLinear;
    }
    
    //通过指定的type来创建scale
    static bool MakeLinearScaleByType() {
        //y轴数据为null的情况 创建的scale是identity
        nlohmann::json data = {{{"field", nullptr}}};
        scale::ScaleController ctrl(nullptr);
        //指定创建的类型
        ctrl.UpdateColConfig("field", {{"type", "linear"}});
        auto coord = ScaleController::MakeCoord();
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isLinear = (*scale).GetType() == scale::ScaleType::Linear;
        return isLinear;
    }
    
    //通过数据来创建category
    static bool MakeCategoryScale() {
        nlohmann::json data = {{{"field", "2017"}}};
        scale::ScaleController ctrl(nullptr);
        auto coord = ScaleController::MakeCoord();
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isCat = (*scale).GetType() == scale::ScaleType::Cat;
        return isCat;
    }
    
    //通过指定type来创建category
    static bool MakeCategoryScaleByType() {
        //y轴数据为null的情况 创建的scale是identity
        nlohmann::json data = {{{"field", nullptr}}};
        scale::ScaleController ctrl(nullptr);
        //指定创建的类型
        ctrl.UpdateColConfig("field", {{"type", "cat"}});
        auto coord = ScaleController::MakeCoord();
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isCat = (*scale).GetType() == scale::ScaleType::Cat;
        return isCat;
    }
    
    static bool MakeTimeCatScale() {
        nlohmann::json data = {{{"field", "2017"}}};
        nlohmann::json config = {{"type", "timeCat"}};
        scale::ScaleController ctrl(nullptr);
        auto coord = ScaleController::MakeCoord();
        ctrl.UpdateColConfig("field", config);
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isCat = (*scale).GetType() == scale::ScaleType::TimeCat;
        
        auto &cachedScale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isCat2 = (*cachedScale).GetType() == scale::ScaleType::TimeCat;
        return isCat && isCat2;
    }
    
    static bool MakeTimeSharingLinearScale() {
        nlohmann::json config = {{"type", "timeSharing"}};
        nlohmann::json data = {{{"field", "2017"}}};
        scale::ScaleController ctrl(nullptr);
        auto coord = ScaleController::MakeCoord();
        ctrl.UpdateColConfig("field", config);
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isTimeSharing = (*scale).GetType() == scale::ScaleType::TimeSharingLinear;
        return isTimeSharing;
    }
    
    static bool MakeKLineScale() {
        nlohmann::json config = {{"type", "kline-day"}};
        nlohmann::json data = {{{"field", "2017"}}};
        scale::ScaleController ctrl(nullptr);
        ctrl.UpdateColConfig("field", config);
        auto coord = ScaleController::MakeCoord();
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isKline = (*scale).GetType() == scale::ScaleType::Kline;
        return isKline;
    }
    
    //创建不存在的scale，默认是identity
    static bool MakeUnknowScale() {
        //field是一个对象
        nlohmann::json data = {{{"field", {{"time", 2017}}}}};
        scale::ScaleController ctrl(nullptr);
        auto coord = ScaleController::MakeCoord();
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool isIdentity = (*scale).GetType() == scale::ScaleType::Identity;
        return isIdentity;
    }
    
    static bool ClearScale() {
        nlohmann::json data = {{{"field", "2017"}}};
        scale::ScaleController ctrl(nullptr);
        auto coord = ScaleController::MakeCoord();
        auto &scale = ctrl.CreateScale("field", data, ScaleController::MakeTracer(), coord);
        bool ret1 = ctrl.ScaleCount() == 1;
        ctrl.Clear();
        bool ret2 = ctrl.Empty();
        return ret1 && ret2;
    }
};

}//unit
