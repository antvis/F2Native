#include "graphics/util/Point.h"
#include "utils/xtime.h"
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#ifndef XG_GRAPHICS_EVENT_CONTROLLER_H
#define XG_GRAPHICS_EVENT_CONTROLLER_H

#define PRESS_DELAY 250

namespace xg {
namespace event {

class Event;
typedef std::function<bool(Event &event)> EventCallbackType;

class Event {
  public:
    std::string eventType;
    std::vector<util::Point> points;
    std::string direction;
    long long timeStamp;
    double zoom = 1.;
    util::Point center;
    double deltaX = 0;
    double deltaY = 0;
    double devicePixelRatio = 1.0;
};

class EventController {
  public:
    void AddCallback(std::string type, EventCallbackType callback) { listeners_[type].push_back(callback); }

    ~EventController() { listeners_.clear(); }

    bool OnTouchEvent(Event &event) {
        // TODO 增加安全行校验，当前是否数据 or 状态是否支持能够支持事件分发

        if(event.eventType == "touchstart") {
            bool ret = false;
            // fixed: 上一次的事件序列还未结束又开始新的事件，先结束之前的。
            if(!startEvent_.eventType.empty()) {
                ret = this->OnTouchEnd(startEvent_);
            }
            return this->OnTouchStart(event) || ret;
        } else if(event.eventType == "touchmove") {
            return this->OnTouchMove(event);
        } else if(event.eventType == "touchend") {
            return this->OnTouchEnd(event);
        } else if(event.eventType == "touchcancel") {
            //            return this->OnTouchCancel(event);
            return this->OnTouchEnd(event);
        }
        return false;
    }

    bool EmitEvent(std::string eventType, Event &event) {
        bool ret = false;
        auto &callbacks = listeners_[eventType];
        std::for_each(callbacks.begin(), callbacks.end(),
                      [&](EventCallbackType &callback) -> void { ret = callback(event) || ret; });
        return ret;
    }

  private:
    bool OnTouchStart(Event &event) {
        if(event.points.empty())
            return false;
        Reset();
        startEvent_ = event;
        preEvent_ = event;
        if(event.points.size() > 1) {
            startDistance_ = CalcDistance(event.points[0], event.points[1]);
            startEvent_.center = GetCenter(event.points[0], event.points[1]);
            startEvent_.zoom = 1;
        }
        return false;
    }

    bool OnTouchMove(Event &event) {
        // EmitEvent("touchmove", event);
        if(startEvent_.points.empty())
            return false;
        if(event.points.size() > 1) {
            double currentDistance = CalcDistance(event.points[0], event.points[1]);
            double preDistance = CalcDistance(preEvent_.points[0], preEvent_.points[1]);
            event.zoom = currentDistance / preDistance;
            event.center = startEvent_.center;
            event.eventType = "pinch";

            preEvent_ = event;
            if(startEvent_.eventType != "pinch") {
                startEvent_.eventType = "pinch";
                return EmitEvent("pinchstart", event);
            }
            return EmitEvent("pinch", event);
        } else {
            std::string eventType = GetEventType(startEvent_, event);
            if(lockGesture) {
                eventType = preEvent_.eventType;
            }

            if(eventType.empty())
                return false;
            if(startEvent_.eventType != eventType) {
                EmitEvent(startEvent_.eventType + "end", event);
                startEvent_.eventType = eventType;
                EmitEvent(eventType + "start", event);
            }

            event.direction = CalcDirection(startEvent_.points[0], event.points[0]);

            auto &points = event.points;
            double deltaX = points[0].x - preEvent_.points[0].x;
            double deltaY = points[0].y - preEvent_.points[0].y;

            event.eventType = startEvent_.eventType;
            event.deltaX = deltaX;
            event.deltaY = deltaY;

            if(eventType == "press") {
                lockGesture = true;
            } else if(eventType == "pan" && fabs(deltaX) >= 10 * event.devicePixelRatio) {
                lockGesture = true;
            }

            preEvent_ = event;
            return EmitEvent(eventType, event);
        }
        return false;
    }

    bool OnTouchEnd(Event &event) {
        std::string &type = startEvent_.eventType;
        event.direction = startEvent_.direction;
        event.eventType = startEvent_.eventType;
        bool ret = EmitEvent(type + "end", event);
        // EmitEvent("touchend", event);
        Reset();
        return ret;
    }

    bool OnTouchCancel(Event &event) {
        // EmitEvent("touchcancel", event);
        Reset();
        return false;
    }

    void Reset() {
        startEvent_.direction = "none";
        startEvent_.points = {};
        startEvent_.timeStamp = 0;
        startEvent_.zoom = 1.;
        startEvent_.eventType = "";
        startDistance_ = 1;

        preEvent_ = startEvent_;
        lockGesture = false;
    }

    static std::string GetEventType(Event &startEvent, Event &event) {
        if(startEvent.eventType == "press") {
            // 已经确定是 press 了。 再移动过程中始终保持 press 事件
            return "press";
        }

        std::string type;

        if(event.timeStamp - startEvent.timeStamp > PRESS_DELAY &&
           CalcDistance(startEvent.points[0], event.points[0]) < 10 * event.devicePixelRatio) {
            type = "press";
        } else {
            type = "pan";
        }
        return type;
    }

    static double CalcDistance(util::Point &point1, util::Point &point2) {
        double xDistance = fabs(point2.x - point1.x);
        double yDistance = fabs(point2.y - point1.y);
        return sqrt(xDistance * xDistance + yDistance * yDistance);
    }

    static std::string CalcDirection(util::Point &start, util::Point &end) {
        double xDistance = end.x - start.x;
        double yDistance = end.y - start.y;

        if(fabs(xDistance) > fabs(yDistance)) {
            return xDistance > 0 ? "right" : "left";
        } else {
            return yDistance > 0 ? "down" : "up";
        }
    }

    static util::Point GetCenter(util::Point &point1, util::Point &point2) {
        double x = point1.x + (point2.x - point1.x) / 2;
        double y = point2.y + (point2.y - point1.y) / 2;
        return util::Point(x, y);
    }

  private:
    std::map<std::string, std::vector<EventCallbackType>> listeners_;
    Event startEvent_;
    Event preEvent_;
    double startDistance_ = 1;
    bool lockGesture = false;
};
} // namespace event
} // namespace xg

#endif // XG_GRAPHICS_EVENT_CONTROLLER_H
