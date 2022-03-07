#include "CanvasFontParser.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace xg;
using namespace xg::canvas;

bool CanvasFontParser::Parse(const std::string &font, CanvasFont &fontStyle) {
    CanvasFont::Style style = CanvasFont::Style::INVALID;
    CanvasFont::Weight weight = CanvasFont::Weight::INVALID;
    CanvasFont::Variant variant = CanvasFont::Variant::INVALID;
    std::string outFontFamily;
    float fontStyleSize = 0;

    bool parseFlag = ParseFields(font, style, variant, weight, fontStyleSize, outFontFamily);
    if(parseFlag) {
        std::string formatFont = font;
        std::transform(formatFont.begin(), formatFont.end(), formatFont.begin(), ::tolower);
        fontStyle.fontSize = fontStyleSize;
        fontStyle.fontFamily = outFontFamily;
        fontStyle.fontStyle = style;
        fontStyle.fontVariant = variant;
        fontStyle.fontWeight = weight;
    }
    return parseFlag;
}

inline void SafePushStringToVector(std::vector<std::string> &out, const std::string &s) {
    if(!s.empty()) {
        out.emplace_back(s);
    }
}

void SplitFontFields(const std::string &s, std::vector<std::string> &out) {
    size_t len = s.length();
    size_t offset_start = 0;
    bool prev_space_flag = false;
    bool in_quote = false;
    for(size_t i = 0; i < len; i++) {
        if(prev_space_flag) {           // prev item is space
            if(std::isspace(s.at(i))) { // is space
                continue;
            } else {                  // not space
                if(out.size() >= 4) { // last font family
                    SafePushStringToVector(out, s.substr(i));
                    break;
                } else {
                    prev_space_flag = false;
                    if(s.at(i) == '\'' || s.at(i) == '\"') {
                        in_quote = true;
                    }
                    offset_start = i;
                }
            }
        } else { // prev item is not space
            if(in_quote) {
                if(s.at(i) == '\'' || s.at(i) == '\"') { // quote end
                    in_quote = false;
                    SafePushStringToVector(out, s.substr(offset_start, i - offset_start));
                } else {                 // quote not end
                    if(i == (len - 1)) { // final item
                        SafePushStringToVector(out, s.substr(offset_start));
                    } else {
                        continue;
                    }
                }
            } else {                        // not in quote
                if(std::isspace(s.at(i))) { // is space
                    prev_space_flag = true;
                    SafePushStringToVector(out, s.substr(offset_start, i - offset_start));
                } else {
                    if(i == (len - 1)) { // final item
                        SafePushStringToVector(out, s.substr(offset_start));
                    }
                }
            }
        }
    }
}

/**
 * [font-style] [font-variant] [font-weight] (font-size) (font-family)
 * font-size, font-family is required, If one of the other values is missing, their default value are used
 * default table:
 * font-style: normal (normal|italic|oblique)
 * font-variant: normal (normal|small-caps)
 * font-weight: normal (normal|bold|bolder|lighter|100|200|300|400|500|600|700|800|900)
 * font-size: Npx | Npt | Npx/ Npx(line-height)
 * font-family: font family name, can has space
 */
bool CanvasFontParser::ParseFields(const std::string &font, CanvasFont::Style &style, CanvasFont::Variant &variant, CanvasFont::Weight &weight, float &fontStyleSize, std::string &outFontFamily) {
    if(font.empty()) {
        return false;
    }
    std::string newFont(font);
    std::vector<std::string> parts;

    // split font str to fields
    SplitFontFields(newFont, parts);

    size_t count = parts.size();
    if(count < 2) { // field count 不足，解析失败
        return false;
    }

    // 1. parse fontSize
    bool parseFontSizeOK = false;
    std::string &fontSize = parts[count - 2];
    std::transform(fontSize.begin(), fontSize.end(), fontSize.begin(), ::tolower);
    // ignore slash
    std::size_t s_pos = fontSize.find('/');
    if(s_pos != std::string::npos) {
        fontSize = fontSize.substr(0, s_pos);
    }
    auto pos = fontSize.find("px");
    if(pos != std::string::npos) {
        if(pos == fontSize.length() - 2) {
            char *endPtr = nullptr;
            fontStyleSize = strtof(fontSize.data(), &endPtr);
            parseFontSizeOK = true;
        }
    }
    pos = fontSize.find("pt");
    if(pos != std::string::npos) {
        if(pos == (fontSize.length() - 2)) {
            char *endPtr = nullptr;
            fontStyleSize = strtof(fontSize.data(), &endPtr) * 4 / 3;
            parseFontSizeOK = true;
        }
    }

    if(!parseFontSizeOK) { // 解析font size失败, 中断其他field解析
        return false;
    }

    // 2. parse font family
    std::string fontFamily = parts[count - 1];
    if(fontFamily.length() > 0) {
        fontFamily.erase(std::remove(fontFamily.begin(), fontFamily.end(), '\"'), fontFamily.end());
        fontFamily.erase(std::remove(fontFamily.begin(), fontFamily.end(), '\''), fontFamily.end());
        std::transform(fontFamily.begin(), fontFamily.end(), fontFamily.begin(), ::tolower);
        // 不支持 initial|inherit|default, 排查特殊符号
        if(fontFamily != "initial" && fontFamily != "inherit" && fontFamily != "default" &&
           fontFamily.find("{") == std::string::npos && fontFamily.find("{") == std::string::npos) {
            outFontFamily = fontFamily;
        }
    }
    if(outFontFamily.empty()) { // 解析font family失败，中断其他field解析
        return false;
    }

    // 3. parse font-weight
    if(count >= 3) {
        std::string weightStr = parts[count - 3];
        std::transform(weightStr.begin(), weightStr.end(), weightStr.begin(), ::tolower);
        if(weightStr == "normal" || weightStr == "400") {
            weight = CanvasFont::Weight::NORMAL;
        } else if(weightStr == "bold" || weightStr == "700") {
            weight = CanvasFont::Weight::BOLD;
        } else if(weightStr == "bolder") {
            weight = CanvasFont::Weight::BOLDER;
        } else if(weightStr == "lighter") {
            weight = CanvasFont::Weight::LIGHTER;
        } else if(weightStr == "100") {
            weight = CanvasFont::Weight::THIN;
        } else if(weightStr == "200") {
            weight = CanvasFont::Weight::EXTRA_LIGHT;
        } else if(weightStr == "300") {
            weight = CanvasFont::Weight::LIGHT;
        } else if(weightStr == "500") {
            weight = CanvasFont::Weight::MEDIUM;
        } else if(weightStr == "600") {
            weight = CanvasFont::Weight::SEMI_BOLD;
        } else if(weightStr == "800") {
            weight = CanvasFont::Weight::EXTRA_BOLD;
        } else if(weightStr == "900") {
            weight = CanvasFont::Weight::BLACK;
        } else if(weightStr == "1000") {
            weight = CanvasFont::Weight::BLACK;
        } else {
            weight = CanvasFont::Weight::INVALID;
        }
    } else {
        weight = CanvasFont::Weight::NORMAL;
    }

    if(weight == CanvasFont::Weight::INVALID) { // 解析weight失败
        return false;
    }

    // 4. parse variant
    if(count >= 4) {
        std::string variantStr = parts[count - 4];
        std::transform(variantStr.begin(), variantStr.end(), variantStr.begin(), ::tolower);
        if(variantStr == "normal") {
            variant = CanvasFont::Variant::NORMAL;
        } else if(variantStr == "small-caps") {
            variant = CanvasFont::Variant::SMALL_CAPS;
        } else {
            variant = CanvasFont::Variant::INVALID;
        }
    } else {
        variant = CanvasFont::Variant::NORMAL;
    }

    if(variant == CanvasFont::Variant::INVALID) { // 解析variant失败
        return false;
    }

    // 5. parse style
    if(count == 5) {
        std::string styleStr = parts[count - 5];
        std::transform(styleStr.begin(), styleStr.end(), styleStr.begin(), ::tolower);
        if(styleStr == "normal") {
            style = CanvasFont::Style::NORMAL;
        } else if(styleStr == "oblique") {
            style = CanvasFont::Style::OBLIQUE;
        } else if(styleStr == "italic") {
            style = CanvasFont::Style::ITALIC;
        } else {
            style = CanvasFont::Style::INVALID;
        }
    } else {
        style = CanvasFont::Style::NORMAL;
    }

    if(style == CanvasFont::Style::INVALID) { // 解析style失败
        return false;
    }

    return true;
}
