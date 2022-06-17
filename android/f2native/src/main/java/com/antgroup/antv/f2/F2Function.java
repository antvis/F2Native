package com.antgroup.antv.f2;

import java.util.UUID;

/**
 * @author qingyuan.yl
 * @date 2020/12/1
 */
public abstract class F2Function {

    protected String functionId = null;

    public F2Function() {
        this.functionId = UUID.randomUUID().toString();
    }

    final void bindChart(F2Chart chart) {
        chart.bindChart(functionId, this);
    }

    public abstract F2Config execute(String param);
}
