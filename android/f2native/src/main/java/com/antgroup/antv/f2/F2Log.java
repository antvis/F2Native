package com.antgroup.antv.f2;

import com.antgroup.antv.f2.service.F2LogDefault;
import com.antgroup.antv.f2.service.F2LogService;

public final class F2Log {

    private static F2LogService mF2LogService;

    private F2Log() {
    }

    public static void setServiceImpl(F2LogService logger) {
        mF2LogService = logger;
    }

    public static F2LogService get() {
        if (mF2LogService == null) {
            mF2LogService = new F2LogDefault();
        }
        return mF2LogService;
    }
}
