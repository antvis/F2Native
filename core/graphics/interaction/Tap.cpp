#include "Tap.h"
#include "graphics/XChart.h"
#include "utils/Tracer.h"

using namespace xg;

interaction::Tap::Tap(XChart *chart) : InteractionBase(chart) {
    this->chart_->eventController_->AddRetCallback("click", XG_MEMBER_CALLBACK_1(interaction::Tap::onClick));
}

interaction::Tap::~Tap() {
    this->chart_->eventController_->RemoveRetCallback("click");
}

const std::string interaction::Tap::onClick(event::Event &event) {
    util::Point point = event.points[0];
    const std::string ret = this->handleClick(point);
    return ret;
}

const std::string interaction::Tap::handleClick(const util::Point &point) {
    util::Point _point = point;
    
    utils::Tracer *tracer = this->chart_->GetLogTracer();
    
    nlohmann::json callbackGeomRecords;
    nlohmann::json geomRecords;
    // 先处理坐标轴点击
//    bool handleTap = false;
//    bool isSelect = true;
    nlohmann::json tickInfo;
    scale::Tick tick;
    
    // 新逻辑
    bool geomsHandleTap = false;
    for (auto &geom : this->chart_->geoms_) {
        if (!geom->isSelectionEnable()) {
            break;
        }
        
        nlohmann::json newSelectedArray;
        bool geomHandleTap = false;
        
        const nlohmann::json records = geom->GetSnapRecords(this->chart_, _point, true);
        tracer->trace("%s GetSnapRecords: %s", "#Tap handleClick", records.dump().c_str());
        if (records.size() > 0) {
            geomHandleTap = true;
            for (std::size_t index = 0; index < records.size(); ++index) {
                const nlohmann::json &record = records[index];
                if (!geom->isSelected(record)) {
                    geomRecords.push_back(record);
                    newSelectedArray.push_back(record);
                    nlohmann::json tr;
                    for (const auto &el : record.items()) {
                        if (el.key().substr(0, 1) != "_") {
                            tr[el.key()] = el.value();
                        }
                    }
                    callbackGeomRecords.push_back(tr);
                } else if (!geom->isDeselect()) {
                    geomHandleTap = false;
                }
            }
        }
        
        if (geomHandleTap) {
            geomsHandleTap = true;
            geom->SetSelectedArray(newSelectedArray, true);
        }
    }
    
    if (geomsHandleTap) {
        this->chart_->axisController_->associateGeomTap(this->chart_, _point, geomRecords);
        chart_->Repaint();
        nlohmann::json rst = {};
        if (callbackGeomRecords.size() > 0) {
            rst["geomRecords"] = callbackGeomRecords;
        }
        if (tickInfo.size() > 0) {
            rst["tickInfo"] = tickInfo;
        }
        return rst.dump();
    } else {
        bool tickHandleTap = false;
        bool tickIsSelect = false;
        std::tie(tickHandleTap, tickIsSelect, tickInfo, tick) = this->chart_->axisController_->onTap(this->chart_, _point);
        if (tickHandleTap) {
            nlohmann::json rst;
            if (tickInfo.contains("recordIndex")) {
                const auto &field = json::GetString(tickInfo, "field");
                for (auto &geom : chart_->geoms_) {
                    nlohmann::json newSelectedArray;
                    const nlohmann::json &records = geom->GetTickRecord(chart_, field, tick);
                    geomRecords = records;
                    if (records.size() > 0) {
                        bool canHandle = false;
                        for (const nlohmann::json& record : records) {
                            if (!geom->isSelected(record)) {
                                nlohmann::json tr = {};
                                for (const auto& el : record.items()) {
                                    if (el.key().substr(0, 1) != "_") {
                                        tr[el.key()] = el.value();
                                    }
                                }
                                callbackGeomRecords.push_back(tr);
                                newSelectedArray.push_back(record);
                                canHandle = true;
                            } else if (!geom->isDeselect()) {
                                canHandle = false;
                                break;
                            }
                        }
                        if (canHandle) {
                            geom->SetSelectedArray(newSelectedArray, true);
                        }
                    }
                }
                if (callbackGeomRecords.size() > 0) {
                    rst["geomRecords"] = callbackGeomRecords;
                }
            }
            
            if (tickInfo.size() > 0) {
                rst["tickInfo"] = tickInfo;
            }
            this->chart_->Repaint();
            return rst.dump();
        }
    }
    
    return "";
    
//    // 老逻辑
//    std::tie(handleTap, isSelect, tickInfo, tick) = chart_->axisController_->onTap(chart_, _point);
//    if (handleTap) {
//        if (tickInfo.contains("recordIndex")) {
//            const auto &field = json::GetString(tickInfo, "field");
//            for (auto &geom : chart_->geoms_) {
//                const nlohmann::json &records = geom->GetTickRecord(chart_, field, tick);
//                origGeomRecords = records;
//                if (records.size() > 0) {
//                    for (const nlohmann::json& record : records) {
//                        nlohmann::json tr = {};
//                        for (const auto& el : record.items()) {
//                            if (el.key().substr(0, 1) != "_") {
//                                tr[el.key()] = el.value();
//                            }
//                        }
//                        geomRecords.push_back(tr);
//                    }
//                    break;
//                }
//            }
//        } else {
//            chart_->Repaint();
//            nlohmann::json rst = {
//                {"tickInfo", tickInfo},
//            };
//            return rst.dump();
//        }
//    }
//    
//    // 再处理图元点击
//    bool hasSnapRecords = false;
//    std::for_each(chart_->geoms_.begin(), chart_->geoms_.end(), [&](auto &geom) -> void {
//        auto &selectedArray = geom->GetSelectedArray();
//        nlohmann::json newSelectedArray;
//        bool needReMapping = false;
//        nlohmann::json records;
//        if (handleTap) {
//            if (origGeomRecords.size() > 0) {
//                const nlohmann::json &origGeomRecord = origGeomRecords[0];
//                if (isSelect) {
//                    records.push_back(origGeomRecord);
//                } else {
////                    const size_t tickRecordIndex = json::GetIntNumber(tickRecord, "recordIndex", -1);
////                    for (const nlohmann::json &selectedItem : selectedArray) {
////                        if (selectedItem["recordIndex"] != tickRecordIndex) {
////                            newSelectedArray.push_back(selectedItem);
////                        }
////                    }
//                }
//                
//                // 此处无法兼容多折线 or 饼图 or 堆叠 or 分组等特殊类型，处理起来比较麻烦，暂时仅对外提供柱图关联保障
//                needReMapping = true;
//            }
//        } else {
//            records = geom->GetSnapRecords(chart_, _point, true);
//            
//            chart_->GetLogTracer()->trace("%s GetSnapRecords: %s", "#Tap handleClick", records.dump().c_str());
//            if (records.size() > 0) {
//                hasSnapRecords = true;
//                for (const nlohmann::json& record : records) {
//                    nlohmann::json tr = {};
//                    for (const auto& el : record.items()) {
//                        if (el.key().substr(0, 1) != "_") {
//                            tr[el.key()] = el.value();
//                        }
//                    }
//                    geomRecords.push_back(tr);
//                }
//            }
//        }
//        
//        if (selectedArray == nullptr || selectedArray.size() == 0) {
//            geom->SetSelectedArray(records, needReMapping);
//            return;
//        }
//        
//        for (std::size_t index = 0; index < records.size(); ++index) {
//            nlohmann::json &record = records[index];
//            if (!geom->isSelected(record)) {
//                newSelectedArray.push_back(record);
//            } else if (!geom->isDeselect()) {
//                hasSnapRecords = false;
//                handleTap = false;
//                return;
//            }
//        }
//        
//        geom->SetSelectedArray(newSelectedArray, needReMapping);
//    });
//    
//    if (hasSnapRecords || handleTap) {
//        chart_->Repaint();
//        nlohmann::json rst = {};
//        if (geomRecords.size() > 0) {
//            rst["geomRecords"] = geomRecords;
//        }
//        if (tickInfo.size() > 0) {
//            rst["tickInfo"] = tickInfo;
//        }
//        return rst.dump();
//    }
    
    return "";
}
