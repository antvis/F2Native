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
typedef std::function<void(Event &event)> EventCallbackType;

class Event {
  public:
    std::string eventType;
    std::vector<util::Point> points;
    std::string direction;
    long timeStamp;
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

    void OnTouchEvent(Event &event) {
        if(event.eventType == "touchstart") {
            this->OnTouchStart(event);
        } else if(event.eventType == "touchmove") {
            this->OnTouchMove(event);
        } else if(event.eventType == "touchend") {
            this->OnTouchEnd(event);
        } else if(event.eventType == "touchcancel") {
            this->OnTouchCancel(event);
        }
    }

    void EmitEvent(std::string eventType, Event &event) {
        auto &callbacks = listeners_[eventType];
        std::for_each(callbacks.begin(), callbacks.end(), [&](EventCallbackType &callback) -> void { callback(event); });
    }

  private:
    void OnTouchStart(Event &event) {
        if(event.points.empty())
            return;
        Reset();
        startEvent_ = event;
        if(event.points.size() > 1) {
            startDistance_ = CalcDistance(event.points[0], event.points[1]);
            startEvent_.center = GetCenter(event.points[0], event.points[1]);
            startEvent_.zoom = 1;
        }
    }

    void OnTouchMove(Event &event) {
        // EmitEvent("touchmove", event);
        if(startEvent_.points.empty())
            return;
        if(event.points.size() > 1) {
            double currentDistance = CalcDistance(event.points[0], event.points[1]);
            event.zoom = currentDistance / startDistance_;
            event.center = startEvent_.center;
            event.eventType = "pinch";
            if(startEvent_.eventType != "pinch") {
                startEvent_.eventType = "pinch";
                EmitEvent("pinchstart", event);
            }
            EmitEvent("pinch", event);
        } else {
            auto &points = event.points;
            double deltaX = points[0].x - startEvent_.points[0].x;
            double deltaY = points[0].y - startEvent_.points[0].y;

            std::string eventType = GetEventType(startEvent_, event);
            if(eventType.empty())
                return;

            if(startEvent_.direction.empty()) {
                startEvent_.direction = CalcDirection(startEvent_.points[0], event.points[0]);
            }

            if(startEvent_.eventType != eventType) {
                EmitEvent(startEvent_.eventType + "end", event);
                startEvent_.eventType = eventType;
                EmitEvent(eventType + "start", event);
            }

            event.direction = startEvent_.direction;
            event.eventType = startEvent_.eventType;
            event.deltaX = deltaX;
            event.deltaY = deltaY;

            EmitEvent(eventType, event);
        }
    }

    void OnTouchEnd(Event &event) {
        std::string &type = startEvent_.eventType;
        event.direction = startEvent_.direction;
        event.eventType = startEvent_.eventType;
        EmitEvent(type + "end", event);
        // EmitEvent("touchend", event);
        Reset();
    }

    void OnTouchCancel(Event &event) {
        // EmitEvent("touchcancel", event);
        Reset();
    }

    void Reset() {
        startEvent_.direction = "none";
        startEvent_.points = {};
        startEvent_.timeStamp = 0;
        startEvent_.zoom = 1.;
        startEvent_.eventType = "";
        startDistance_ = 1;
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
    double startDistance_ = 1;
};
} // namespace event
} // namespace xg

#endif // XG_GRAPHICS_EVENT_CONTROLLER_H
