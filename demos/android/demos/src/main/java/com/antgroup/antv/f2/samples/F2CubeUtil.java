package com.antgroup.antv.f2.samples;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.graphics.Color;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2ChartBridge;
import com.antgroup.antv.f2.F2Log;

/**
 * author by luna
 * date on 2023/12/6
 */

public class F2CubeUtil {
    private F2ChartBridge mF2ChartBridge;

    public F2CubeUtil(F2ChartBridge f2ChartBridge) {
        this.mF2ChartBridge = f2ChartBridge;
    }

    public static void sendNoInvokeRenderEvent(String mCanvasBizId, boolean b) {
    }

    public static double getPx2RpxRatio(Context context) {
        return 1;
    }

    public static String getBizCode(F2CanvasView mCanvasView) {
        return "demo";
    }

    public void initThemeTokenMap() {
        HashMap<String, Integer> map1 = new HashMap<>();
        map1.put("COLOR_TEXT_SECONDARY", -10066330);// #FF666666:-10066330
        map1.put("COLOR_BORDER", -1118482);// #FFEEEEEE:-1118482
        map1.put("COLOR_BRAND2", -15304705);// #FF1677FF:-15304705
        map1.put("COLOR_CARD", -1);// #FFFFFFFF:-1
        map1.put("COLOR_BRAND1", -15304705);// #FF1677FF:-15304705
        map1.put("COLOR_CARD_PRESS", -2500135);// #FFD9D9D9:-2500135
        map1.put("COLOR_RED", -52927);// #FFFF3141:-52927
        map1.put("COLOR_WHITE_CHANGE", -1);// #FFFFFFFF:-1
        map1.put("COLOR_YELLOW", -24808);// #FFFF9F18:-24808
        map1.put("COLOR_TEXT_PRIMARY", -13421773);// #FF333333:-13421773
        map1.put("COLOR_BLACK", -16777216);// #FF000000:-16777216
        map1.put("COLOR_TEXT_WEAK", -3355444);// #FFCCCCCC:-3355444
        map1.put("COLOR_WHITE", -1);// #FFFFFFFF:-1
        map1.put("COLOR_BLACK_CHANGE", -16777216);// #FF000000:-16777216
        map1.put("COLOR_STATUS_BAR", -1);// #FFFFFFFF:-1
        map1.put("COLOR_GREEN", -14503074);// #FF22B35E:-14503074
        map1.put("COLOR_TEXT_ASSIST", -6710887);// #FF999999:-6710887
        map1.put("COLOR_BACKGROUND", -657931);// #FFF5F5F5:-657931


        HashMap<String, Integer> map2 = new HashMap<>();
        map2.put("COLOR_TEXT_SECONDARY", -10066330);// #FF666666:-10066330
        map2.put("COLOR_BORDER", -1118482);// #FFEEEEEE:-1118482
        map2.put("COLOR_BRAND2", -15304705);// #FF1677FF:-15304705
        map2.put("COLOR_CARD", -1);// #FFFFFFFF:-1
        map2.put("COLOR_BRAND1", -15304705);// #FF1677FF:-15304705
        map2.put("COLOR_CARD_PRESS", -2500135);// #FFD9D9D9:-2500135
        map2.put("COLOR_RED", -52927);// #FFFF3141:-52927
        map2.put("COLOR_WHITE_CHANGE", -1);// #FFFFFFFF:-1
        map2.put("COLOR_YELLOW", -24808);// #FFFF9F18:-24808
        map2.put("COLOR_TEXT_PRIMARY", -13421773);// #FF333333:-13421773
        map2.put("COLOR_BLACK", -16777216);// #FF000000:-16777216
        map2.put("COLOR_TEXT_WEAK", -3355444);// #FFCCCCCC:-3355444
        map2.put("COLOR_WHITE", -1);// #FFFFFFFF:-1
        map2.put("COLOR_BLACK_CHANGE", -16777216);// #FF000000:-16777216
        map2.put("COLOR_STATUS_BAR", -1);// #FFFFFFFF:-1
        map2.put("COLOR_GREEN", -14503074);// #FF22B35E:-14503074
        map2.put("COLOR_TEXT_ASSIST", -6710887);// #FF999999:-6710887
        map2.put("COLOR_BACKGROUND", -657931);// #FFF5F5F5:-657931


        HashMap<String, String> map3 = new HashMap<>();
        map3.put("#FFF93A4A", "#FFFF4A58");
        map3.put("#FFFFD9D9", "#FF391D1C");
        map3.put("#FFE62B3B", "#FFFF4A58");
        map3.put("#FF4B6B99", "#FF3086FF");
        map3.put("#FFCCCCCC", "#FF474747");
        map3.put("#B2E1E1E1", "#B2474747");
        map3.put("#FF00B7F4", "#FF3086FF");
        map3.put("#FFF4F9FD", "#FF0D2543");
        map3.put("#FFF4F9FF", "#FF2B2B2B");
        map3.put("#FFF93A49", "#FFFF4A58");
        map3.put("#FFFF8B16", "#FFE65A2B");
        map3.put("#FFE3EFFF", "#FF408FFF");
        map3.put("#FFF4AA51", "#FFFFA930");
        map3.put("#FF0F0F0F", "#FFEAE0BA");
        map3.put("#FFD8D8D8", "#FF474747");
        map3.put("#FF999999", "#FF616161");
        map3.put("#FFF9F9F9", "#FF121212");
        map3.put("#FFA9A9A9", "#FF616161");
        map3.put("#99000000", "#99FFFFFF");
        map3.put("#FFF4333B", "#FFFF4A58");
        map3.put("#FFFFCCCC", "#FFFF5865");
        map3.put("#FF428BCA", "#FF3086FF");
        map3.put("#FFE5E5E5", "#FF2B2B2B");
        map3.put("#512E2E2E", "#51C5CAD1");
        map3.put("#FF666666", "#FF808080");
        map3.put("#FFFCFDFC", "#FF121212");
        map3.put("#FFF4F4F4", "#FF121212");
        map3.put("#FF088E81", "#FF34B368");
        map3.put("#FFF7F7F7", "#FF121212");
        map3.put("#FFE2E2E2", "#FF2B2B2B");
        map3.put("#FFFFD2D6", "#FF421D20");
        map3.put("#FFEEEEEE", "#FF2B2B2B");
        map3.put("#FF2A83FF", "#FF3086FF");
        map3.put("#FFFF411C", "#FFE65A2B");
        map3.put("#FF436B9D", "#FF3086FF");
        map3.put("#FF000000", "#FFFFFFFF");
        map3.put("#FF2DD070", "#FF34B368");
        map3.put("#FFE6E6E6", "#FF2B2B2B");
        map3.put("#80000000", "#80FFFFFF");
        map3.put("#FFFFFFFF", "#FF1A1A1A");
        map3.put("#FFFC5D31", "#FFE65A2B");
        map3.put("#FFFF6430", "#FFE65A2B");
        map3.put("#FFFF7E10", "#FFE65A2B");
        map3.put("#FF4A4A4A", "#FFC5CAD1");
        map3.put("#7F000000", "#7FFFFFFF");
        map3.put("#FFF1F1F1", "#FF121212");
        map3.put("#FFBBF0D7", "#FF1A332C");
        map3.put("#05000000", "#05FFFFFF");
        map3.put("#FFE8E8E8", "#FF2B2B2B");
        map3.put("#FFFFE5E7", "#FF2E1B1D");
        map3.put("#FF699827", "#FF34B368");
        map3.put("#FF3996E6", "#FF3086FF");
        map3.put("#FFFF340B", "#FFE65A2B");
        map3.put("#08333333", "#08C5CAD1");
        map3.put("#FF555555", "#FF808080");
        map3.put("#FF378FFE", "#FF3086FF");
        map3.put("#FFFAFBFC", "#FF121212");
        map3.put("#FFFE7630", "#FFE65A2B");
        map3.put("#FFFFAA18", "#FFE65A2B");
        map3.put("#FFDDDDDD", "#FF474747");
        map3.put("#FFF0841A", "#FFFFA930");
        map3.put("#FFD9D9D9", "#FF2B2B2B");
        map3.put("#CC000000", "#FF525252");
        map3.put("#FFF0F3F7", "#FF121212");
        map3.put("#FF948D98", "#FF616161");
        map3.put("#FFFF6010", "#FFE65A2B");
        map3.put("#FFFFF2F2", "#FF391D1C");
        map3.put("#FFFF3141", "#FFFF4A58");
        map3.put("#FF1677FF", "#FF3086FF");
        map3.put("#FF00B578", "#FF34B368");
        map3.put("#FFFDEBE6", "#FFE7663B");
        map3.put("#FFFFD2CC", "#FFFF5865");
        map3.put("#FFE62C3B", "#FFFF4A58");
        map3.put("#30000000", "#30FFFFFF");
        map3.put("#FFD12B2B", "#FFFF4A58");
        map3.put("#FF0E9976", "#FF34B368");
        map3.put("#FF3A8BDE", "#FF3086FF");
        map3.put("#FF333333", "#FFC5CAD1");
        map3.put("#99333333", "#99C5CAD1");
        map3.put("#FFEDF5FF", "#FF408FFF");
        map3.put("#FFF5F5F5", "#FF121212");
        map3.put("#FFD6F5E6", "#FF192623");
        map3.put("#3D000000", "#3DFFFFFF");
        map3.put("#FFE6572C", "#FFE7663B");
        map3.put("#FF22B35E", "#FF34B368");
        map3.put("#FFD1D1D1", "#FF474747");
        map3.put("#FFFFEFE7", "#FFE7663B");
        map3.put("#FF108ee9", "#FF3086FF");
        map3.put("#14999999", "#14616161");
        map3.put("#FFFAFAFA", "#FF0E0E0E");
        map3.put("#FFEAE0BA", "#FF0F0F0F");
        map3.put("#FFFAFAFC", "#FF121212");
        map3.put("#FFE7F1FF", "#FF1C232C");
        map3.put("#FFFF3B30", "#FFFF4A58");
        map3.put("#FFFF8800", "#FFE65A2B");
        map3.put("#FFFF6900", "#FFE65A2B");
        map3.put("#FFFFEFEA", "#FFE7663B");
        map3.put("#FF444444", "#FFC5CAD1");
        map3.put("#FFFFF3F4", "#FFFF5865");
        map3.put("#FFF8F8F8", "#FF121212");
        map3.put("#FFFF5780", "#FFE65A2B");
        map3.put("#FFFF6111", "#FFE65A2B");
        map3.put("#FFFFD7CC", "#FFE7663B");
        map3.put("#FFFF9F18", "#FFE65A2B");
        map3.put("#FFFAEFD9", "#FF2B2B2B");

        setThemeTokenMap(map3, true);
    }

    private void setThemeTokenMap(Map<String, String> replaceColorTokens,
                                  boolean isDark) {
        Map<Integer, Integer> sReplaceColors = parseReplaceMap(replaceColorTokens);
        mF2ChartBridge.updateThemeToken(sReplaceColors, isDark);
    }

    public static Map<Integer, Integer> parseReplaceMap(Map<String, String> replaceColors) {
        if (replaceColors == null || replaceColors.isEmpty()) {
            return Collections.emptyMap();
        }
        Map<Integer, Integer> intColors = new HashMap<>();
        for (Map.Entry<String, String> entry : replaceColors.entrySet()) {
            try {
                int keyColor = Color.parseColor(entry.getKey());
                int valueColor = Color.parseColor(entry.getValue());
                intColors.put(keyColor, valueColor);
            } catch (Exception e) {
                F2Log.get().e("DemoThemeUtils", "parseReplaceMap: color parse failed:" + entry.getKey() + " - " + entry.getValue(), e);
            }
        }
        return intColors;
    }

    public static boolean getDarkModeConfig(){
        return true;
    }
}
