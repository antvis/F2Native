//
// Created by luna on 2023/12/5.
//

#include "DarkModeManager.h"
#include "../utils/StringUtil.h"


using namespace xg;
using namespace token;

DarkModeManager::DarkModeManager(const std::string &tagName) {
    this->logTracer_ = new utils::Tracer(tagName + "@DarkModeManager");
}

DarkModeManager::~DarkModeManager() {
    this->logTracer_->trace(" %s ", "Destroy");
    this->isDarkMode_ = false;
    this->replaceColors_.clear();
    this->cacheMaps_.clear();
    if (this->logTracer_ != nullptr) {
        delete this->logTracer_;
    }
    this->logTracer_ = nullptr;
}

int DarkModeManager::ReplaceColor(int color) {
    if (!this->isDarkMode_ || this->replaceColors_.empty()) {
        return color;
    }
    auto it = this->replaceColors_.find(color);
    if (it != this->replaceColors_.end()) {
        int valueColor = it->second;
        return valueColor;
    }
    return color;
}

void DarkModeManager::SetDarkModeInfo(bool isDark, std::map<int, int> &colors) {
    if (colors.empty()) {
        this->logTracer_->trace("SetDarkModeInfo %s ", "colors empty");
        return;
    }
    this->logTracer_->trace("SetDarkModeInfo dark: %s, %d ", isDark ? "Y" : "N", colors.size());
    if (isDark != this->isDarkMode_) {
        this->isDarkMode_ = isDark;
    }
    for (auto it = colors.begin(); it != colors.end(); ++it) {
        this->replaceColors_[it->first] = it->second;
    }
}

// Android
int DarkModeManager::GetColorWithInt(const int colorInt) {
    int result = colorInt;
    if (!this->isDarkMode_) {
        return result;
    }
    result = ReplaceColor(colorInt);
    return result;
}

// iOS
void DarkModeManager::GetColorWithRGHA(canvas::CanvasColor &beforeCanvasColor) {
    if (!this->isDarkMode_ || this->replaceColors_.empty()) {
        return;
    }
    // 转换之前的颜色
    int beforeResult = canvas::CanvasColorParser::RGBAToHex(beforeCanvasColor);
    auto iter = cacheMaps_.find(beforeResult);
    if (iter != cacheMaps_.end()) {
        canvas::CanvasColor outputAgColor = iter->second;
        beforeCanvasColor.r = outputAgColor.r;
        beforeCanvasColor.g = outputAgColor.g;
        beforeCanvasColor.b = outputAgColor.b;
        beforeCanvasColor.a = outputAgColor.a;
        return;
    }
    // 转换之前的颜色 => 转换之后的颜色
    int afterResult = ReplaceColor(beforeResult);
    canvas::CanvasColorParser::HexToRGBA(afterResult, beforeCanvasColor);
    cacheMaps_[beforeResult] = {beforeCanvasColor.r, beforeCanvasColor.g, beforeCanvasColor.b, beforeCanvasColor.a};
}