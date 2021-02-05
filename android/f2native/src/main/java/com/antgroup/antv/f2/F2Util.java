package com.antgroup.antv.f2;

import android.text.TextUtils;

import org.json.JSONArray;
import org.json.JSONObject;

import java.lang.reflect.Field;
import java.util.Random;

/**
 * @author qingyuan.yl
 * @date 2020-09-18
 */
public final class F2Util {

    static class ColorGradient<T extends ColorGradient> {
        protected JSONArray colorStops = new JSONArray();
        F2Config.Builder gradient = new F2Config.Builder();

        protected ColorGradient() {
            gradient.setOption("colorStops", colorStops);
        }

        public T addColorStop(float offset, String color) {
            if (offset < 0 || offset > 1) return (T) this;
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

    final static String generateId() {
        return String.valueOf(System.currentTimeMillis() + "-" + new Random().nextInt(100000));
    }

}
