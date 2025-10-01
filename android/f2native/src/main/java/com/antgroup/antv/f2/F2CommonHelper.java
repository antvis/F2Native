package com.antgroup.antv.f2;

import com.antgroup.antv.f2.service.F2CommonDefault;
import com.antgroup.antv.f2.service.F2CommonService;

/**
 * author by luna
 * date on 2022/5/18
 * 图形通用工具类，包括：so的加载、事件上报cs开关
 */
public final class F2CommonHelper {

    private static F2CommonService mF2CommonService;

    public static void setServiceImpl(F2CommonService iCommon) {
        mF2CommonService = iCommon;
    }

    public static F2CommonService get() {
        if (mF2CommonService == null) {
            mF2CommonService = new F2CommonDefault();
        }
        return mF2CommonService;
    }

    private F2CommonHelper() {
    }
}