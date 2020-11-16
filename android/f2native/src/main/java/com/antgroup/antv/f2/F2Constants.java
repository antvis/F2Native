package com.antgroup.antv.f2;

/**
 * @author qingyuan.yl
 * @date 2020-09-22
 */
class F2Constants {
    public static final String CANVAS_ID = "canvas_id";

    public static final int CODE_SUCCESS = 0;
    public static final int CODE_FAIL_UNKNOWN = -1;
    public static final int CODE_INIT_EGL_FAIL = 1;
    public static final int CODE_INIT_CANVAS_CONTEXT_FAIL = 2;
    public static final int CODE_CREATE_CANVAS_2D_CONTEXT_FAIL = 3;
    public static final int CODE_RESUME_CANVAS_FAIL = 4;

    public static boolean isSuccessCode(int code) {
        return code == CODE_SUCCESS;
    }
}
