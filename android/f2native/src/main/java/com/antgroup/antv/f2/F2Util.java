package com.antgroup.antv.f2;

import android.text.TextUtils;

import org.json.JSONArray;
import org.json.JSONObject;

import java.lang.reflect.Field;
import java.math.BigDecimal;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

/**
 * @author qingyuan.yl
 */
public final class F2Util {

    private static final String DEFAULT_TIMEZONE = "Asia/Shanghai";
    private static final String TIME_PATTERN_ALL = "yyyy-MM-dd HH:mm:ss";

    public static class ColorGradient<T extends ColorGradient> {
        protected JSONArray colorStops = new JSONArray();
        F2Config.Builder gradient = new F2Config.Builder();

        protected ColorGradient() {
            gradient.setOption("colorStops", colorStops);
        }

        public T addColorStop(float offset, String color) {
            if (offset < 0 || offset > 1) {
                return (T) this;
            }
            try {
                JSONObject colorStop = new JSONObject();
                colorStop.put("offset", offset);
                colorStop.put("color", color);
                colorStops.put(colorStop);
            } catch (Exception e) {
            }
            return (T) this;
        }
    }

    public static class ColorLinearGradient extends ColorGradient<ColorLinearGradient> {
        public ColorLinearGradient() {
            super();
            gradient.setOption("isLinear", true);
        }

        public ColorLinearGradient setPosition(float startX, float startY, float endX, float endY) {
            gradient.setOption("position", new float[]{startX, startY, endX, endY});
            return this;
        }
    }

    public static class ColorRadialGradient extends ColorGradient<ColorRadialGradient> {
        public ColorRadialGradient() {
            super();
            gradient.setOption("isLinear", false);
        }

        public ColorRadialGradient setPosition(float startX, float startY, float startR, float endX, float endY, float endR) {
            gradient.setOption("position", new float[]{startX, startY, startR, endX, endY, endR});
            return this;
        }
    }

    final static Field reflectGetFieldByName(Field[] fields, String name) {
        Field targetField = null;
        for (int i = 0; i < fields.length; i++) {
            if (TextUtils.equals(name, fields[i].getName())) {
                targetField = fields[i];
                break;
            }
        }
        return targetField;
    }

    // 以格林威治为标准时间，计算时间差，单位s
    public static long getTimeZoneOffset(long utcTime, String tz) {
        // 更新时区
        if (TextUtils.isEmpty(tz)) {
            tz = "GMT+8";
        }
        TimeZone timeZone = TimeZone.getTimeZone(tz);
        return timeZone.getOffset(utcTime) / 1000;
    }

    public static String getFormatTime(String dateString, String timeZoneStr, String formatter) {
        if (TextUtils.isEmpty(formatter)) {
            formatter = TIME_PATTERN_ALL;
        }
        if (TextUtils.isEmpty(timeZoneStr)) {
            timeZoneStr = DEFAULT_TIMEZONE;
        }
        try {
            SimpleDateFormat dateFormat = new SimpleDateFormat(formatter);
            dateFormat.setTimeZone(TimeZone.getTimeZone(timeZoneStr));
            long timeStamp = new BigDecimal(dateString).longValue();
            return dateFormat.format(new Date(timeStamp));
        } catch (Exception e) {
            F2Log.e("F2Util", "getFormatTime exception: " + e.getMessage());
        }
        return dateString;
    }

}
