package com.antgroup.antv.f2.service;

public interface F2LogService {

    String TAG = "F2Native";

    void i(String traceId, String content);

    void w(String traceId, String content);

    void e(String traceId, String content);

    void e(String traceId, String msg, Throwable t);

}
