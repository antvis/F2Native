//
// Created by ruize on 20212/1/7.
//

#include "CanvasColorParser.h"

using namespace xg::canvas;

std::unordered_map<std::string, CanvasColor> CanvasColorParser::colorMap;
std::string CanvasColorParser::hex_digit = "0123456789abcdef";

typedef union {
    struct {
        float r, g, b, a;
    } rgba;
    float components[4];
} InnerColorRGBA;

void CanvasColorParser::InitColorMapIfEmpty() {
    if(colorMap.empty()) {
        colorMap.insert(std::pair<std::string, CanvasColor>("black", {0, 0, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("silver", {192, 192, 192, 255})); //{{192.0f / 255, 192.0f / 255, 192.0f / 255, 1.0f}}));
        colorMap.insert(std::pair<std::string, CanvasColor>("gray", {128, 128, 128, 255})); //{{128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
        colorMap.insert(std::pair<std::string, CanvasColor>("white", {255, 255, 255, 255})); //{{255.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
        colorMap.insert(std::pair<std::string, CanvasColor>("maroon", {128, 0, 0, 255})); //{{128.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
        colorMap.insert(std::pair<std::string, CanvasColor>("red", {255, 0, 0, 255})); //{{255.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
        colorMap.insert(std::pair<std::string, CanvasColor>("purple", {128, 0, 128, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("fuchsia", {255, 0, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("green", {0, 128, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lime", {0, 255, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("olive", {128, 128, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("yellow", {255, 255, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("navy", {0, 0, 128, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("blue", {0, 0, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("teal", {0, 128, 128, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("aqua", {0, 255, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("aliceblue", {240, 248, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("antiquewhite", {250, 235, 215, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("aqua", {0, 255, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("aquamarine", {127, 255, 212, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("azure", {240, 255, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("beige", {245, 245, 220, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("bisque", {255, 228, 196, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("black", {0, 0, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("blanchedalmond", {255, 235, 205, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("blue", {0, 0, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("blueviolet", {138, 43, 226, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("brown", {165, 42, 42, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("burlywood", {222, 184, 135, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("cadetblue", {95, 158, 160, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("chartreuse", {127, 255, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("chocolate", {210, 105, 30, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("coral", {255, 127, 80, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("cornflowerblue", {100, 149, 237, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("cornsilk", {255, 248, 220, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("crimson", {220, 20, 60, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("cyan", {0, 255, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkblue", {0, 0, 139, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkcyan", {0, 139, 139, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkgoldenrod", {184, 134, 11, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkgray", {169, 169, 169, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkgreen", {0, 100, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkgrey", {169, 169, 169, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkkhaki", {189, 183, 107, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkmagenta", {139, 0, 139, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkolivegreen", {85, 107, 47, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkorange", {255, 140, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkorchid", {153, 50, 204, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkred", {139, 0, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darksalmon", {233, 150, 122, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkseagreen", {143, 188, 143, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkslateblue", {72, 61, 139, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkslategray", {47, 79, 79, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkslategrey", {47, 79, 79, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkturquoise", {0, 206, 209, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("darkviolet", {148, 0, 211, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("deeppink", {255, 20, 147, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("deepskyblue", {0, 191, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("dimgray", {105, 105, 105, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("dimgrey", {105, 105, 105, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("dodgerblue", {30, 144, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("firebrick", {178, 34, 34, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("floralwhite", {255, 250, 240, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("forestgreen", {34, 139, 34, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("fuchsia", {255, 0, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("gainsboro", {220, 220, 220, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("ghostwhite", {248, 248, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("gold", {255, 215, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("goldenrod", {218, 165, 32, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("gray", {128, 128, 128, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("green", {0, 128, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("greenyellow", {173, 255, 47, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("grey", {128, 128, 128, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("honeydew", {240, 255, 240, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("hotpink", {255, 105, 180, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("indianred", {205, 92, 92, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("indigo", {75, 0, 130, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("ivory", {255, 255, 240, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("khaki", {240, 230, 140, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lavender", {230, 230, 250, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lavenderblush", {255, 240, 245, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lawngreen", {124, 252, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lemonchiffon", {255, 250, 205, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightblue", {173, 216, 230, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightcoral", {240, 128, 128, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightcyan", {224, 255, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightgoldenrodyellow", {250, 250, 210, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightgray", {211, 211, 211, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightgreen", {144, 238, 144, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightgrey", {211, 211, 211, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightpink", {255, 182, 193, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightsalmon", {255, 160, 122, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightseagreen", {32, 178, 170, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightskyblue", {135, 206, 250, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightslategray", {119, 136, 153, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightslategrey", {119, 136, 153, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightsteelblue", {176, 196, 222, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lightyellow", {255, 255, 224, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("lime", {0, 255, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("limegreen", {50, 205, 50, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("linen", {250, 240, 230, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("magenta", {255, 0, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("maroon", {128, 0, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mediumaquamarine", {102, 205, 170, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mediumblue", {0, 0, 205, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mediumorchid", {186, 85, 211, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mediumpurple", {147, 112, 219, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mediumseagreen", {60, 179, 113, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mediumslateblue", {123, 104, 238, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mediumspringgreen", {0, 250, 154, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mediumturquoise", {72, 209, 204, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mediumvioletred", {199, 21, 133, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("midnightblue", {25, 25, 112, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mintcream", {245, 255, 250, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("mistyrose", {255, 228, 225, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("moccasin", {255, 228, 181, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("navajowhite", {255, 222, 173, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("navy", {0, 0, 128, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("oldlace", {253, 245, 230, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("olive", {128, 128, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("olivedrab", {107, 142, 35, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("orange", {255, 165, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("orangered", {255, 69, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("orchid", {218, 112, 214, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("palegoldenrod", {238, 232, 170, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("palegreen", {152, 251, 152, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("paleturquoise", {175, 238, 238, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("palevioletred", {219, 112, 147, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("papayawhip", {255, 239, 213, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("peachpuff", {255, 218, 185, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("peru", {205, 133, 63, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("pink", {255, 192, 203, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("plum", {221, 160, 221, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("powderblue", {176, 224, 230, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("purple", {128, 0, 128, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("red", {255, 0, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("rosybrown", {188, 143, 143, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("royalblue", {65, 105, 225, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("saddlebrown", {139, 69, 19, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("salmon", {250, 128, 114, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("sandybrown", {244, 164, 96, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("seagreen", {46, 139, 87, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("seashell", {255, 245, 238, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("sienna", {160, 82, 45, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("silver", {192, 192, 192, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("skyblue", {135, 206, 235, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("slateblue", {106, 90, 205, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("slategray", {112, 128, 144, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("slategrey", {112, 128, 144, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("snow", {255, 250, 250, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("springgreen", {0, 255, 127, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("steelblue", {70, 130, 180, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("tan", {210, 180, 140, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("teal", {0, 128, 128, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("thistle", {216, 191, 216, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("tomato", {255, 99, 71, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("turquoise", {64, 224, 208, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("violet", {238, 130, 238, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("wheat", {245, 222, 179, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("white", {255, 255, 255, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("whitesmoke", {245, 245, 245, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("yellow", {255, 255, 0, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("yellowgreen", {154, 205, 50, 255}));
        colorMap.insert(std::pair<std::string, CanvasColor>("transparent", {0, 0, 0, 0}));
        colorMap.insert(std::pair<std::string, CanvasColor>("transparent_white", {255, 255, 255, 0}));
    }
}

void ParseRGBAFullString(char value[8], CanvasColor &output) {
    unsigned long hex = 0x00000000 | strtoul(value, nullptr, 16);
    //        output.rgba = {(hex >> 24) / 255.0f, ((hex & 0xff0000) >> 16) / 255.0f,
    //                       ((hex & 0xff00) >> 8) / 255.0f, (hex & 0xff) / 255.0f};
    output.r = hex >> 24;
    output.g = ((hex & 0xff0000) >> 16);
    output.b = ((hex & 0xff00) >> 8);
    output.a = (hex & 0xff);
}

static std::string Trim(const std::string &str, const char *whitespace = nullptr, bool trimStart = true, bool trimEnd = true) {
    if(str.empty()) {
        return str;
    }

    std::string target;
    if(whitespace == nullptr) {
        target = " \t";
    } else {
        target = whitespace;
    }

    unsigned long strBegin = 0;
    if(trimStart) {
        strBegin = str.find_first_not_of(target);
    }

    if(strBegin == std::string::npos) { // trim start时找不到有效字符，直接返回空串
        return "";
    }

    if(trimEnd) {
        auto strEnd = str.find_last_not_of(target);
        if(strEnd == std::string::npos) { // trim end时找不到有效字符，直接返回空串
            return "";
        }
        auto strRange = strEnd - strBegin + 1;
        // ALOGI("str=%s|strBegin=%i|strEnd=%i|strRange:%i", str.data(), strBegin, strEnd, strRange);
        return str.substr(strBegin, strRange);
    } else {
        return str.substr(strBegin);
    }
}

bool HandleBraceRGBAColor(char *value, InnerColorRGBA &output) {
    int length = (int)strlen(value);
    int current = 0;
    std::string temp;
    float colorV;

    // separator format
    bool isSpaceSep = false;
    bool isCommaSep = false;
    bool isPercent = false;

    int start = 4;
    if(value[start] == '(') {
        start = 5;
    }

    bool endFlag;
    bool startFlag = false;
    bool hasAlphaContent = false;
    bool parseAlphaFlag = false;
    for(int i = start; i < length && current < 4; i++) {
        if(current == 3) { // parse alpha
            hasAlphaContent = true;
            std::string alpha = &(value[i]);
            alpha = Trim(alpha);
            // 从头部trim ','字符
            alpha = Trim(alpha, ",", true, false);
            // 从尾部trim ')'
            alpha = Trim(alpha, ")", false, true);
            // 再trim掉空格
            alpha = Trim(alpha);

            char lastChar = '\0';
            // percent处理
            bool isAlphaPercent = false;
            if(!alpha.empty()) {
                lastChar = alpha[alpha.size() - 1];
                if(lastChar == '%') {
                    isAlphaPercent = true;
                    alpha = alpha.substr(0, alpha.size() - 1);
                }
            }
            // 判断末尾字符是否digit
            if(!alpha.empty()) {
                parseAlphaFlag = isdigit(alpha[alpha.size() - 1]) != 0;
            } else {
                parseAlphaFlag = false;
            }

            // ALOGD("Trim alpha=%s,valid=%i", alpha.data(), parseAlphaFlag);
            if(parseAlphaFlag) {
                char *end = nullptr;
                // convert alpha，并利用end判断是否成功
                float d = strtof(alpha.data(), &end);
                // ALOGD("strtof:%s=%f|end=%s", alpha.data(), d, end);
                std::string end_str = end;
                if(!end_str.empty()) {
                    parseAlphaFlag = false;
                } else {
                    parseAlphaFlag = true;
                    if(isAlphaPercent) {
                        d = d / 100;
                    }
                    if(d < 0) {
                        d = 0;
                    }
                    if(d > 1) {
                        d = 1;
                    }
                    output.components[current] = d;
                }
            }
            current++;
        } else { // parse r/g/b
            if(isdigit(value[i]) || value[i] == '.' || value[i] == '%' || value[i] == '-' || value[i] == '+') {
                if(!startFlag) {
                    startFlag = true;
                }
                // reset not end
                endFlag = false;
                temp.push_back(value[i]);
            } else if(value[i] == ')' || isspace(value[i])) { // end
                endFlag = true;
            } else if(value[i] == ',') { // end
                endFlag = true;
                if(i == (length - 1)) { // end with comma, invalid
                    return false;
                }
            } else { // wrong format
                return false;
            }

            if(i == length - 1) {
                endFlag = true;
            }

            if(startFlag && endFlag) {
                // check is all percent?
                char lastChar = temp.at(temp.length() - 1);
                if(current == 0) {
                    if(value[i] == ',') {
                        isCommaSep = true;
                    } else if(isspace(value[i])) {
                        isSpaceSep = true;
                    }
                    if(lastChar == '%') {
                        isPercent = true;
                    }
                } else {
                    if(lastChar != '%' && isPercent) { // not percent consistent
                        return false;
                    }
                    if(value[i] == ',' && !isCommaSep) { // not comma consistent
                        return false;
                    }
                    if(isspace(value[i]) && !isSpaceSep) { // not space consistent
                        return false;
                    }
                }
                // TODO 增加convert是否成功的判断
                colorV = (float)atof(temp.data());
                // ALOGI("parse filed s=%s, value=%f", temp.data(), colorV);
                if(isPercent) {
                    colorV = colorV < 0 ? 0 : (colorV > 100 ? 100 : colorV);
                    output.components[current] = colorV / 100;
                } else {
                    colorV = colorV < 0 ? 0 : (colorV > 255 ? 255 : colorV);
                    output.components[current] = colorV / 255.0f;
                }

                current++;
                temp = "";
                startFlag = false;
                endFlag = false;
            }
        }
    }

    // 判断alpha
    if(hasAlphaContent) {
        if(!parseAlphaFlag) {
            return false;
        }
    }
    // 判断是否包含R/G/B 三个分段
    return current >= 3;
}

bool CanvasColorParser::Parse(const std::string &str, CanvasColor &outputAgColor) {
    InitColorMapIfEmpty();

    outputAgColor = {0, 0, 0, 255};
    if(str.empty()) {
        return false;
    }

    bool result = true;
    const char *value = str.data();
    do {
        if(value[0] == '#') {
            int length = (int)strlen(value);
            char str_arr[] = "ffffffff";

            // check value valid?
            bool sharp_valid = true;
            for(int i = 1; i < length; i++) {
                if(!((value[i] >= '0' && value[i] <= '9') || (value[i] >= 'a' && value[i] <= 'f') ||
                     (value[i] >= 'A' && value[i] <= 'F'))) { // wrong format
                    // LOG_E("check # wrong format: %c=", value[i]);
                    sharp_valid = false;
                    break;
                }
            }

            if(!sharp_valid) {
                result = false;
                break;
            }

            if(length == 4) {
                str_arr[0] = str_arr[1] = value[1];
                str_arr[2] = str_arr[3] = value[2];
                str_arr[4] = str_arr[5] = value[3];
                ParseRGBAFullString(str_arr, outputAgColor);
            } else if(length == 5) {
                str_arr[0] = str_arr[1] = value[1];
                str_arr[2] = str_arr[3] = value[2];
                str_arr[4] = str_arr[5] = value[3];
                str_arr[6] = str_arr[7] = value[4];
                ParseRGBAFullString(str_arr, outputAgColor);
            } else if(length == 7) { // #RRGGBB format
                str_arr[0] = value[1];
                str_arr[1] = value[2];
                str_arr[2] = value[3];
                str_arr[3] = value[4];
                str_arr[4] = value[5];
                str_arr[5] = value[6];
                ParseRGBAFullString(str_arr, outputAgColor);
            } else if(length == 9) { // #RRGGBBAA
                memcpy(str_arr, value + 1, 8);
                ParseRGBAFullString(str_arr, outputAgColor);
            } else {
                result = false;
            }
        } else {
            std::string colorVal = value;
            colorVal = Trim(value);
            std::transform(colorVal.begin(), colorVal.end(), colorVal.begin(), ::tolower);
            value = colorVal.c_str();

            // match color name
            auto iter = colorMap.find(value);
            if(iter != colorMap.end()) {
                outputAgColor = iter->second;
                break;
            }

            // handle hsla( format color (not support now)
            if(strncmp(value, "hsl(", 4) == 0 || strncmp(value, "hsla(", 5) == 0) {
                result = false;
                break;
            }

            // handle rgb( | rgba(  format
            bool isRgbBrace = strncmp(value, "rgb(", 4) == 0;
            bool isRgbaBrace = strncmp(value, "rgba(", 4) == 0;
            // 根据web spec https://drafts.csswg.org/css-color/#funcdef-rgba, rgb()和rgba()函数等价，都支持alpha
            if(isRgbBrace || isRgbaBrace) {
                // 注意：无需')'判断， 没有')'结尾也是有效的颜色格式
                InnerColorRGBA floatColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
                auto flag = HandleBraceRGBAColor((char *)value, floatColor);
                if(flag) {
                    outputAgColor.r = int32_t(floatColor.rgba.r * 255);
                    outputAgColor.g = int32_t(floatColor.rgba.g * 255);
                    outputAgColor.b = int32_t(floatColor.rgba.b * 255);
                    outputAgColor.a = int32_t(floatColor.rgba.a * 255);
                }
                result = flag;
            } else { // parse error
                result = false;
            }
        }
    } while(false);

    // ALOGD("parseColorString:success=%i|input=%s|color=(%i,%i,%i,%i)", result, str.data(), outputAgColor.r,
    //        outputAgColor.g, outputAgColor.b, outputAgColor.a);
    return result;
}

 int CanvasColorParser::RGBAToHex(const CanvasColor &color)
{
    return (((int)color.a & 0xff) << 24) + (((int)color.r & 0xff) << 16) + (((int)color.g & 0xff) << 8)
           + ((int)color.b & 0xff);
}
