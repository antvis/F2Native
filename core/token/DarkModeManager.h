//
// Created by luna on 2023/12/5.
//

#ifndef CHART_NATIVE_CORE_DARKMODEMANAGER_H
#define CHART_NATIVE_CORE_DARKMODEMANAGER_H

#include <string>
#include <map>
#include <vector>
#include "../utils/Tracer.h"
#include "../graphics/canvas/CanvasColorParser.h"

namespace xg {
namespace token {

class DarkModeManager {
public:
    DarkModeManager(const std::string &tagName);

    ~DarkModeManager();

    void SetDarkModeInfo(bool isDark, std::map<int, int> &colors);

    int GetColorWithInt(const int color);

    void GetColorWithRGHA(canvas::CanvasColor &color);

private:
    int ReplaceColor(int color);
    utils::Tracer *logTracer_ = nullptr;
    std::map<int, int> replaceColors_;
    std::map<int, canvas::CanvasColor> cacheMaps_;


    bool isDarkMode_ = false;

};
}
}

#endif //CHART_NATIVE_CORE_DARKMODEMANAGER_H
