//
//  RefLine.cpp
//  AFWFinanceChart
//
//  Created by ougu on 2024/9/29.
//  Copyright © 2024 Alipay. All rights reserved.
//

#include "graphics/guide/RefLine.h"
#include "graphics/XChart.h"
#include "graphics/global.h"
#include "graphics/shape/Text.h"
#include "graphics/shape/Rect.h"
#include "graphics/shape/Circle.h"
#include "graphics/shape/Element.h"
#include <utils/common.h>
    
using namespace xg;

void guide::RefLine::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string yField = yFields[0];
    
    
    for (auto it = chart.GetGeoms().begin(); it != chart.GetGeoms().end(); ++it) {
        auto &dataArray = (*it)->GetDataArray();
        long dataArraySize = dataArray.size();
        const std::string &geomType = (*it)->GetType();
        const Group *group = (*it)->GetContainer();
        if (group == nullptr) {
            continue;
        }
        
        long dataRatio = group->children_.size() / dataArraySize;
        if (dataRatio == 0) {
            return;
        }
        
        for (int index = 0; index < group->children_.size(); index++) {
            auto &rect = group->children_[index];
            
            bool shouldDrawRefLine = false;
            std:string content = "";
            float textSize = 30.f;
            const std::string &contentKey = "content";
            const std::string &textColorKey = "textColor";
            const std::string &textSizeKey = "textSize";
            canvas::CanvasFillStrokeStyle refColorStyle = CanvasFillStrokeStyle("#000000");
    
            if (this->config_.contains("refLines") && this->config_["refLines"].is_array()) {
                for(const auto &oneRefLine : this->config_["refLines"]) {
                    std::string key = oneRefLine.contains("recordIndex") ? "recordIndex" : "dataArrayIndex";
                    if(json::GetIntNumber(oneRefLine, key, -1) == index / dataRatio) {
                        //dataArrayIndex与data的index匹配，开始校验数据是否valid
                        //标题
                        if (oneRefLine.contains(contentKey) && oneRefLine[contentKey].is_string()) {
                            content = oneRefLine[contentKey];
                        }
                        //字颜色
                        if (oneRefLine.contains(textColorKey) && oneRefLine[textColorKey].is_string()) {
                            refColorStyle = CanvasFillStrokeStyle(oneRefLine[textColorKey]);
                        }
                        //字大小
                        if (oneRefLine.contains(textSizeKey) && oneRefLine[textSizeKey].is_number()) {
                            textSize = oneRefLine[textSizeKey].get<float>();
                        }
                        shouldDrawRefLine = true;
                    }
                }
            }
            
            if (!shouldDrawRefLine) {
                continue;
            }

            double r0 = 0.f;
            double r = 0.f;
            double startAngle = 0.f;
            double endAngle = 0.f;
            double sidePadding = 0.f;
            double textDefaultMargin = 10.0f; //引线和文字稍微分开点，贴一起不好看
            float lineWidth = 3.0f;
            string textBaseline = "bottom";
            
            const nlohmann::json rectConfig = rect->GetSpecifyConfig();
            if (rectConfig.contains("radius0") && rectConfig["radius0"].is_number()) {
                r0 = rectConfig["radius0"].get<double>();
            }
            if (rectConfig.contains("radius") && rectConfig["radius"].is_number()) {
                r = rectConfig["radius"].get<double>();
            }
            if (rectConfig.contains("startAngle") && rectConfig["startAngle"].is_number()) {
                startAngle = rectConfig["startAngle"].get<double>();
            }
            if (rectConfig.contains("endAngle") && rectConfig["endAngle"].is_number()) {
                endAngle = rectConfig["endAngle"].get<double>();
            }
            
            auto &coord = chart.GetCoord();
            
            // 计算角平分线的方向
            // bisectorDirection[0]其实就是cos，[1]是sin
            Vector2D bisectorDirection = {std::cos((startAngle + endAngle) / 2), std::sin((startAngle + endAngle) / 2)};
            bisectorDirection = Vector2DUtil::Normalize(bisectorDirection); // 单位向量
            
            // 计算中点半径
            double rMid = (r0 + r) / 2;
            // 计算中点的坐标
            util::Point midPoint;
            midPoint.x = coord.GetCenter().x + bisectorDirection[0] * rMid;
            midPoint.y = coord.GetCenter().y + bisectorDirection[1] * rMid;
            
            // 计算文字大小
            auto tag = xg::make_unique<xg::shape::Text>(content, coord.GetCenter(), textSize, refColorStyle.color, refColorStyle.color);
            tag->SetTextAlign("center");
            tag->SetTextBaseline("middle");
            xg::util::BBox bbox = tag->GetBBox(context);
            float textWidth = bbox.width;
            
            //计算边距
            if (this->config_.contains("sidePadding") && this->config_["sidePadding"].is_number()) {
                sidePadding = this->config_["sidePadding"].get<double>() * context.GetDevicePixelRatio();
            }
            
            //引线粗细
            if (this->config_.contains("refLineWidth") && this->config_["refLineWidth"].is_number()) {
                lineWidth = this->config_["refLineWidth"].get<float>() * context.GetDevicePixelRatio();
            }
            
            //文字baseline
            if (this->config_.contains("textBaseline") && this->config_["textBaseline"].is_string()) {
                textBaseline = this->config_["textBaseline"].get<string>();
            }
            
            const auto &anchorStyle = json::GetObject(this->config_ , "anchorStyle");
            //锚点style
//            if (this->config_.contains("anchorStyle") && this->config_["anchorStyle"].is_object()) {
//                anchorStyle = this->config_["anchorStyle"];
//            }
            
            bool isF2Style = false;
            //f2样式，从中心点向外辐射的引线
            if (isF2Style) {
                //如果有锚点偏移量，则引线起点就在锚点偏移量，反之则从半径差中点开始画
                if (this->config_.contains("anchorOffset") && this->config_["anchorOffset"].is_number()) {
                    rMid = r + this->config_["anchorOffset"].get<double>();
                    midPoint.x = coord.GetCenter().x + bisectorDirection[0] * rMid;
                    midPoint.y = coord.GetCenter().y + bisectorDirection[1] * rMid;
                }
                
                // 计算终点坐标
                double inflectionOffset = 50;
                util::Point endPoint;
                endPoint.x = midPoint.x + bisectorDirection[0] * inflectionOffset;
                endPoint.y = midPoint.y + bisectorDirection[1] * inflectionOffset;
                
                //画第一段引线
                auto line = xg::make_unique<xg::shape::Line>(midPoint, endPoint);
                line->SetStorkStyle(refColorStyle);
                line->SetLineWidth(5);
                container->AddElement(std::move(line));
                
                //画第二段扩展到边缘的引线
                auto pointStart = coord.GetStart();
                auto pointEnd = coord.GetEnd();
                float tmpMargin = 300;
                if (endPoint.x <= coord.GetCenter().x) {
                    //左边
                    auto line = xg::make_unique<xg::shape::Line>(util::Point(pointStart.x + tmpMargin, endPoint.y), endPoint);
                    line->SetStorkStyle(refColorStyle);
                    line->SetLineWidth(5);
                    container->AddElement(std::move(line));
                    
                    auto tag = xg::make_unique<xg::shape::Text>(content, util::Point(pointStart.x + tmpMargin, endPoint.y), textSize, refColorStyle.color, refColorStyle.color);
                    tag->SetTextAlign("end");
                    tag->SetTextBaseline("middle");

                    container->AddElement(std::move(tag));
                } else {
                    //右边
                    auto line = xg::make_unique<xg::shape::Line>(util::Point(pointEnd.x - tmpMargin, endPoint.y), endPoint);
                    line->SetStorkStyle(refColorStyle);
                    line->SetLineWidth(5);
                    container->AddElement(std::move(line));
                    
                    auto tag = xg::make_unique<xg::shape::Text>(content, util::Point(pointEnd.x - tmpMargin, endPoint.y), textSize, refColorStyle.color, refColorStyle.color);
                    tag->SetTextAlign("start");
                    tag->SetTextBaseline("middle");

                    container->AddElement(std::move(tag));
                }
            }
            //普通样式，引线从重点往两边横向辐射
            else {
                string textAlign = "center";
                float textStartPointX = sidePadding;
                float lineStartPointX = sidePadding;
                if (midPoint.x <= coord.GetCenter().x) {
                    //左边
                    //左边的引线文字
                    textAlign = "start";//贴左边
                    if (textBaseline == "middle") {
                        lineStartPointX = sidePadding + textWidth + textDefaultMargin;
                    }
                } else {
                    //右边
                    //右边的引线文字
                    textStartPointX = coord.GetEnd().x - sidePadding;
                    textAlign = "end"; //贴右边
                    if (textBaseline == "middle") {
                        lineStartPointX = coord.GetEnd().x - sidePadding - textWidth - textDefaultMargin;
                    } else {
                        lineStartPointX = coord.GetEnd().x - sidePadding;
                    }
                }
                
                auto tag = xg::make_unique<xg::shape::Text>(content, util::Point(textStartPointX, midPoint.y), textSize, refColorStyle.color, refColorStyle.color);
                tag->SetTextAlign(textAlign);
                tag->SetTextBaseline(textBaseline);
                container->AddElement(std::move(tag));
                
                auto line = xg::make_unique<xg::shape::Line>(util::Point(lineStartPointX, midPoint.y), midPoint);
                line->SetStorkStyle(refColorStyle);
                line->SetLineWidth(lineWidth);
                container->AddElement(std::move(line));
            }
            
            if (anchorStyle.is_object()) {
                const auto &anchorShape = json::GetString(anchorStyle, "anchorShape");
                if (anchorShape == "circle") {
                    const auto radius = json::GetNumber(anchorStyle, "radius", 2.f) * context.GetDevicePixelRatio();
                    auto circle = xg::make_unique<xg::shape::Circle>(midPoint, radius, "#FFFFFF", refColorStyle.color, lineWidth);
                    container->AddElement(std::move(circle));
                }
            }
        }
    }
}
