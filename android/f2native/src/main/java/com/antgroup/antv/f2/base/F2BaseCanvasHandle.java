package com.antgroup.antv.f2.base;

/**
 * 业务层调用CanvasContext所需要的接口，antg和native canvas相关的CanvasContext子类分别实现
 */
public interface F2BaseCanvasHandle {

     void setFillStyle(String style);

     void fill();

     void beginPath();

     void arc(float x, float y, float r, float sAngle, float eAngle, boolean antiClockwise);

     void setLineWidth(float lineWidth);

     void setTextAlign(String textAlign);

     void fillText(String text, float x, float y);

     void save();

     void restore();

     void fillRect(int x, int y, int i, int i1);
}
