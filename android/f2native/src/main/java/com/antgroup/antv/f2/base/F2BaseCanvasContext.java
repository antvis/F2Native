package com.antgroup.antv.f2.base;

import android.graphics.Bitmap;

public interface F2BaseCanvasContext {

    //set
    void setStrokeStyle(String style);

    void setStrokeStyle(int color);

    //线性渐变
    void setLinearGradient(float sx, float sy, float ex, float ey, int[] colors, float[] positions);

    //雷达渐变
    void setRadialGradient(float sx, float sy, float sr, float ex, float ey, float er, int[] colors, float[] positions);

    //扫描渐变
    void setConicGradient(float cx, float cy, int[] colors, float[] positions);

    void setFillStyle(String style);

    void setFillStyle(int color);

    void rect(float x, float y, float width, float height);

    void fillRect(float x, float y, float width, float height);

    void fillRect(int x, int y, int width, int height);

    void strokeRect(float x, float y, float width, float height);

    void clearRect(float x, float y, float width, float height);

    void stroke();

    void fill();

    void beginPath();

    void closePath();

    void lineTo(float x, float y);

    void moveTo(float x, float y);

    void clip();

    void quadraticCurveTo(float cpx, float cpy, float x, float y);

    void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);

    void arc(float x, float y, float r, float sAngle, float eAngle, boolean antiClockwise);

    void arcTo(float x1, float y1, float x2, float y2, float r);

    void scale(float sw, float sh);

    void rotate(float angle);

    void translate(float x, float y);

    void transform(float a, float b, float c, float d, float e, float f);

    void setTransform(float a, float b, float c, float d, float e, float f);

    void setLineWidth(float lineWidth);

    void setLineDash(float[] dashes);

    void setMiterLimit(float miterLimit);

    void setFont(int style, int variant, int weight, String name, float size);

    void setTextAlign(String textAlign);

    String getTextAlign();

    void setTextBaseline(String textBaseline);

    String getTextBaseline();

    void drawText(String text, float x, float y);

    void fillText(String text, float x, float y);

    void strokeText(String text, float x, float y);

    float measureText(String text);

    void setGlobalAlpha(float alpha);

    float getGlobalAlpha();

    void save();

    void restore();

    void drawImage(Bitmap bitmap, float dx, float dy);

    void drawImageRect(Bitmap bitmap, float dx, float dy, float sw, float sh);

    void setFillPaint();

    void setLineJoin(String lineJoin);

    void setLineCap(String lineCap);

    void setStrokePaint();

    void resetContext();

    float measureTextHeight(String text) ;
}