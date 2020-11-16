package com.antgroup.antv.f2;

import android.text.TextUtils;

import java.lang.reflect.Field;

/**
 * @author qingyuan.yl
 * @date 2020-09-18
 */
final class F2Util {

    final static Field reflectGetFieldByName(Field[] fields, String name) {
        Field targetField = null;
        for(int i = 0; i < fields.length; i++) {
            if (TextUtils.equals(name, fields[i].getName())) {
                targetField = fields[i];
                break;
            }
        }
        return targetField;
    }

    final static String generateId() {
        return String.valueOf(System.currentTimeMillis());
    }

}
