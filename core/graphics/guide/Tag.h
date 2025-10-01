//
// Created by luna on 2023/8/14.
//

#ifndef CHART_NATIVE_CORE_TAG_H
#define CHART_NATIVE_CORE_TAG_H

#include "GuideBase.h"
#include "../util/BBox.h"
#include "../util/Point.h"
#include "../../nlohmann/json.hpp"

namespace xg {

    namespace guide {

        struct IconConfigInfo {
            std::string iconUrl;
            bool showIcon = false;
            float containerWidth = 0;
            float containerHeight = 0;
            float width = 0;
            float height = 0;
            std::array<float, 4> margin = {0,0,0,0};
        };

         struct SubConfigInfo {
            std::string content;
            std::string textColor;
            std::string textBaseline;
            std::string textAlign;
            float textSize = 0;
            float marginTop = 0;
        };

        class Tag : public GuideBase {
        public:
            Tag(nlohmann::json config = {}) : GuideBase("tag", MergeDefaultCfg(config)) {}

            void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context,
                        const std::vector<util::Rect> &dangerRects) override;

        private:
            void DrawTagPolygon(XChart &chart, shape::Group *container,
                                      canvas::CanvasContext &context, util::Point &position,
                                      const std::string direct, const float sideSize);

            void
            DrawTagContent(XChart &chart, shape::Group *container, canvas::CanvasContext &context,
                           util::Point &position, const float padding[], const std::string direct);

            std::vector<util::Point> GetArrowPoints(XChart &chart,
                                                    canvas::CanvasContext &context,
                                                    const util::Point &position,
                                                    const std::string direct,
                                                    const float sideSize);

            std::string GetDirect(XChart &chart, canvas::CanvasContext &context,
                                  const util::Point &position,
                                  const std::vector<util::Rect> &dangerRects,
                                  const float sideSize);

        protected:
            static nlohmann::json MergeDefaultCfg(const nlohmann::json &config) {
                nlohmann::json defaultCfg = {
//                        {"fill",           "#FFFFFF7D"}, // 子元素统一颜色(点/线/方块边框)
                        {"textSize",        9.f},
                        {"textColor",       "#1890FF"},
                        {"textAlign",       "start"},
                        {"textBaseline",    "bottom"},
                        {"fillOpacity",     1},
                        {"content",         ""},
                        {"direct",          "tl"},
                        {"autoAdjust",      true},
                        {"sideSize",            3},
                        {"padding",         {0.f, 0.f, 0.f, 0.f}},
                        {"lineWidth",       1.0f},        // 线宽
                        {"iconConfig", {{"iconUrl", ""},{"showIcon", false},{"width",12}, {"height",12}, {"margin", {0, 0, 0, 0}}}},
                        {"margin", {0, 0}}
//                        {"stroke", "#1677FF"}, // 方块背景色
                };
                if (config.is_object()) {
                    defaultCfg.merge_patch(config);
                }
                return defaultCfg;
            }


        private:
            SubConfigInfo subConfigInfo_;
            IconConfigInfo iconConfigInfo_;
            util::Rect contentRect_;
            float posX_;
            float posY_;
            float fillOpacity_;
            float lineWidth_;
            std::string strokeColor_;
            std::string fillColor_;
            std::string content_;
            float fontSize_;
            float labelHeight_;
            float subLabelHeight_ = 0;

        };
    } // namespace guide
} // namespace xg


#endif //CHART_NATIVE_CORE_TAG_H
