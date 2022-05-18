package com.antgroup.antv.f2;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.LinearGradient;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PathEffect;
import android.graphics.PorterDuff;
import android.graphics.RadialGradient;
import android.graphics.RectF;
import android.graphics.Shader;
import android.graphics.Typeface;

/**
 * android native canvas需要的CanvasContext
 */
class F2AndroidCanvasContext  {
    public Bitmap bitmap = null;
    private Canvas bitmapCanvas = null;
    private Paint paint = null;
    private Path path = null;
    private String textAlign = "left";
    private String textBaseline = "top";
    private Matrix matrix = new Matrix();
    //screen density
    private float ratio;
    //因为paint区分不出来fill还是stroke，只有在绘制的时候才设置颜色
    private int fillStyle;
    private int strokeStyle;
    private float globalAlpha = 1;
    private static float M_DEGREE = 57.295776F;//180.0F/(float)Math.PI;
    public boolean mHadOOM;

    F2AndroidCanvasContext(int width, int height, float ratio) {
        height = Math.max(height, 1);
        width = Math.max(width, 1);
        this.ratio = ratio;
        try {
            this.bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            this.bitmapCanvas = new Canvas(this.bitmap);
        } catch (OutOfMemoryError e) {
            mHadOOM = true;
            F2Log.e("F2AndroidCanvasContext", "createBitmap OutOfMemoryError");
        }
    }

    private void innerLog(String content) {
        if (BuildConfig.DEBUG) {
            F2Log.i("F2AndroidCanvasContext", content);
        }
    }

    //set
    public void setStrokeStyle(String style) {
        innerLog("setStrokeStyle style:" + style);
        if (this.bitmapCanvas == null) {
            return;
        }
        try {
            int color = Color.parseColor(style);
            setStrokeStyle(color);
        } catch (Exception e) {
            F2Log.e("F2AndroidCanvasContext", "#setStrokeStyle exception " + e.toString());
        }
    }

    public void setStrokeStyle(int color) {
        innerLog("setStrokeStyle color:");
        this.strokeStyle = color;
    }

    //线性渐变
    public void setLinearGradient(float sx, float sy, float ex, float ey, int[] colors, float[] positions) {
        innerLog("setLinearGradient sx:" + sx + " sy:" + sy + " ex:" + ex + " ey:" + ey);
        if (this.bitmapCanvas == null) {
            return;
        }
        if (colors.length != positions.length && colors.length < 2) {
            return;
        }
        LinearGradient gradient = new LinearGradient(sx, sy, ex, ey, colors, positions, Shader.TileMode.CLAMP);
        this.paint.setShader(gradient);
    }

    //雷达渐变
    public void setRadialGradient(float sx, float sy, float sr, float ex, float ey, float er, int[] colors, float[] positions) {
        innerLog("setRadialGradient sx:" + sx + " sy:" + sy + " sr:" + sr + " ex:" + ex + " ey:" + ey + " er:" + er);
        if (this.bitmapCanvas == null) {
            return;
        }
        if (colors.length != positions.length && colors.length < 2) {
            return;
        }
        RadialGradient gradient = new RadialGradient(sx, sy, sr, colors, positions, Shader.TileMode.CLAMP);
        this.paint.setShader(gradient);
    }

    public void setFillStyle(String style) {
        innerLog("setFillStyle style:" + style);
        if (this.bitmapCanvas == null) {
            return;
        }
        try {
            int color = Color.parseColor(style);
            setFillStyle(color);
        } catch (Exception e) {
            F2Log.e("F2AndroidCanvasContext", "#setFillStyle exception " + e.toString());
        }
    }

    public void setFillStyle(int color) {
        innerLog("setFillStyle color:");
        this.fillStyle = color;
    }

    public void rect(float x, float y, float width, float height) {
        innerLog("rect x:" + x + " y:" + y + " width:" + width + " height:" + height);
        if (this.bitmapCanvas == null) {
            return;
        }
        //cw?
        this.path.addRect(x, y, x + width, y + height, Path.Direction.CW);
    }

    public void fillRect(float x, float y, float width, float height) {
        innerLog("fillRect float x:" + x + " y:" + y + " width:" + width + " height:" + height);
        if (this.bitmapCanvas == null) {
            return;
        }
        setFillPaint();
        this.bitmapCanvas.drawRect(x, y, x + width, y + height, this.paint);
    }

    public void fillRect(int x, int y, int width, int height) {
        innerLog("fillRect int x:" + x + " y:" + y + " width:" + width + " height:" + height);
        if (this.bitmapCanvas == null) {
            return;
        }
        setFillPaint();
        this.bitmapCanvas.drawRect(x, y, x + width, y + height, this.paint);
    }

    public void strokeRect(float x, float y, float width, float height) {
        innerLog("strokeRect x:" + x + " y:" + y + " width:" + width + " height:" + height);
        if (this.bitmapCanvas == null) {
            return;
        }
        setStrokePaint();
        this.bitmapCanvas.drawRect(x, y, x + width, y + height, this.paint);
    }

    public void clearRect(float x, float y, float width, float height) {
        innerLog("clearRect x:" + x + " y:" + y + " width:" + width + " height:" + height);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.bitmapCanvas.save();
        RectF rectF = new RectF(x, y, x + width, y + height);
        this.bitmapCanvas.clipRect(rectF);
        this.bitmapCanvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
        this.bitmapCanvas.restore();
    }

    public void stroke() {
        innerLog("stroke");
        if (this.bitmapCanvas == null) {
            return;
        }
        setStrokePaint();
        this.bitmapCanvas.drawPath(this.path, this.paint);
    }

    public void fill() {
        innerLog("fill");
        if (this.bitmapCanvas == null) {
            return;
        }
        setFillPaint();
        this.bitmapCanvas.drawPath(this.path, this.paint);
    }

    public void beginPath() {
        innerLog("beginPath");
        if (this.bitmapCanvas == null) {
            return;
        }
        this.path = new Path();
    }

    public void closePath() {
        innerLog("closePath");
        if (this.bitmapCanvas == null) {
            return;
        }
        this.path.close();
    }

    public void lineTo(float x, float y) {
        innerLog("lineTo x:" + x + " y:" + y);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.path.lineTo(x, y);
    }

    public void moveTo(float x, float y) {
        innerLog("moveTo x:" + x + " y:" + y);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.path.moveTo(x, y);
    }

    public void clip() {
        innerLog("clip");
        if (this.bitmapCanvas == null) {
            return;
        }
        this.bitmapCanvas.clipPath(this.path);
        //新开启一个路径
        beginPath();
    }

    public void quadraticCurveTo(float cpx, float cpy, float x, float y) {
        innerLog("quadraticCurveTo cpx:" + cpx + " cpy:" + cpy + " x:" + x + " y:" + y);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.path.quadTo(cpx, cpy, x, y);
    }

    public void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
        innerLog("bezierCurveTo cp1x:" + cp1x + " cp1y:" + cp1y + "cp2x:" + cp2x + " cp2y:" + cp2y + " x:" + x + " y:" + y);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.path.cubicTo(cp1x, cp1y, cp2x, cp2y, x, y);
    }

    public void arc(float x, float y, float r, float sAngle, float eAngle, boolean antiClockwise) {
        innerLog("arc x:" + x + " y:" + y);
        if (this.bitmapCanvas == null) {
            return;
        }
        float s = sAngle * M_DEGREE;
        //扫过的角度 eAngele - sAngle
        float d = eAngle * M_DEGREE - s;
        RectF rectF = new RectF(x - r, y - r, x + r, y + r);
        if (Math.abs(d) == 360f) {
            this.path.addArc(rectF, s, d);
        } else {
            this.path.arcTo(rectF, s, d, false);
        }
    }

    public void arcTo(float x1, float y1, float x2, float y2, float r) {
        innerLog("arcTo x1:" + x1 + " y1:" + y1 + " x2:" + x2 + " y2:" + y2);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.path.arcTo(new RectF(x1 - 2 * r, y1, x2, y2 + r), -90, 90, false);
    }

    public void scale(float sw, float sh) {
        innerLog("scale sw:" + sw + " sh:" + sh);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.bitmapCanvas.scale(sw, sh);
    }

    public void rotate(float angle) {
        innerLog("rotate angle:" + angle);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.bitmapCanvas.rotate(angle);
    }

    public void translate(float x, float y) {
        innerLog("translate x:" + x + " y:" + y);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.bitmapCanvas.translate(x, y);
    }

    public void transform(float a, float b, float c, float d, float e, float f) {
        innerLog("transform a:" + a + " b:" + b + " c:" + c + " d:" + d);
        if (this.bitmapCanvas == null) {
            return;
        }
        Matrix m = new Matrix();
        m.reset();
        m.preTranslate(e, f);
        m.preScale(a, d);
        m.preSkew(c, b);
        matrix.setConcat(matrix, m);

        this.bitmapCanvas.setMatrix(matrix);
    }

    public void setTransform(float a, float b, float c, float d, float e, float f) {
        innerLog("setTransform a:" + a + " b:" + b + " c:" + c + " d:" + d);
        if (this.bitmapCanvas == null) {
            return;
        }
        Matrix m = new Matrix();
        m.reset();
        m.preTranslate(e, f);
        m.preScale(a, d);
        m.preSkew(c, b);
        this.bitmapCanvas.setMatrix(m);
    }

    public void setLineCap(String lineCap) {
        //todo unsupport
        throw new AssertionError("setLineCap not support");
    }

    public void setLineJoin(String lineJoin) {
        //todo unsupport
        throw new AssertionError("setLineJoin not support");
    }

    public void setLineWidth(float lineWidth) {
        innerLog("setLineWidth lineWidth:" + lineWidth);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.paint.setStrokeWidth(lineWidth);
    }

    public void setLineDash(float[] dashes) {
        innerLog("setLineWidth dashes:");
        if (this.bitmapCanvas == null) {
            return;
        }
        if (dashes.length == 0) {
            //clear path effect
            this.paint.setPathEffect(new PathEffect());
        } else if (dashes.length == 1) {
            //<=2 DashPathEffect会抛出异常
            this.paint.setPathEffect(new DashPathEffect(new float[]{dashes[0], dashes[1]}, 0));
        } else {
            this.paint.setPathEffect(new DashPathEffect(dashes, 0));
        }
    }

    public void setLineDashOffset(float[] lineDashOffset) {
        //todo unsupport
        throw new AssertionError("setLineDashOffset not support");
    }

    public void setMiterLimit(float miterLimit) {
        innerLog("setMiterLimit miterLimit:" + miterLimit);
        if (this.bitmapCanvas == null) {
            return;
        }
        this.paint.setStrokeMiter(miterLimit);
    }

    public void setFont(String font) {
        //todo unsupport
        throw new AssertionError("setFont not support");
    }

    public void setFont(int style, int variant, int weight, String name, float size) {
        innerLog("setFont style:" + style + " name:" + name + " size:" + size);
        if (this.bitmapCanvas == null) {
            return;
        }
        //todo support variant weight

        if (name == null) {
            return;
        }
        //NDK 16只支持这个方式创建Typeface
        Typeface face = Typeface.create(name, style);
        this.paint.setTypeface(face);
        this.paint.setTextSize(size);
    }

    public void setTextAlign(String textAlign) {
        innerLog("setTextAlign textAlign:" + textAlign);
        if (this.bitmapCanvas == null) {
            return;
        }
        if (textAlign == null) {
            return;
        }
        Paint.Align align = Paint.Align.LEFT;
        if ("start".equals(textAlign) || "left".equals(textAlign)) {
            align = Paint.Align.LEFT;
        } else if ("end".equals(textAlign) || "right".equals(textAlign)) {
            align = Paint.Align.RIGHT;
        } else if ("center".equals(textAlign)) {
            align = Paint.Align.CENTER;
        }
        this.textAlign = textAlign;
        this.paint.setTextAlign(align);
    }

    public String getTextAlign() {
        innerLog("getTextAlign ");
        return textAlign;
    }

    public void setTextBaseline(String textBaseline) {
        innerLog("setTextBaseline textBaseline:" + textBaseline);
        if (textBaseline == null) {
            return;
        }
        this.textBaseline = textBaseline;
    }

    public String getTextBaseline() {
        innerLog("getTextBaseline");
        return textBaseline;
    }

    public void drawText(String text, float x, float y) {
        innerLog("drawText text:" + text + " x:" + x + " y:" + y);
        if (this.bitmapCanvas == null) {
            return;
        }
        if (text == null) {
            return;
        }
        Paint.FontMetrics fm = this.paint.getFontMetrics();
        float height = fm.descent - fm.ascent;
        if ("top".equals(textBaseline)) {
            y += height - fm.bottom;
        } else if ("bottom".equals(textBaseline)) {
            //do nothing
            y -= fm.bottom;
        } else if ("middle".equals(textBaseline)) {
            y += height / 2 - fm.bottom;
        }
        //默认基线是baseline
        this.bitmapCanvas.drawText(text, x, y, this.paint);
    }

    public void fillText(String text, float x, float y) {
        innerLog("fillText text:" + text + " x:" + x + " y:" + y);
        if (text == null) {
            return;
        }
        setFillPaint();
        drawText(text, x, y);
    }

    public void strokeText(String text, float x, float y) {
        innerLog("strokeText text:" + text + " x:" + x + " y:" + y);
        if (text == null) {
            return;
        }
        setStrokePaint();
        drawText(text, x, y);
    }

    public float measureText(String text) {
        innerLog("measureText text:" + text);
        if (this.bitmapCanvas == null) {
            return 0;
        }
        if (text == null) {
            return 0;
        }
        return this.paint.measureText(text);
    }

    public void setGlobalAlpha(float alpha) {
        innerLog("setGlobalAlpha alpha:" + alpha);
        alpha = Math.max(alpha, 0);
        alpha = Math.min(alpha, 1);
        this.globalAlpha = alpha;
    }

    public float getGlobalAlpha() {
        innerLog("getGlobalAlpha");
        return this.globalAlpha;
    }

    public void save() {
        innerLog("save");
        if (this.bitmapCanvas == null) {
            return;
        }
        this.bitmapCanvas.save();
    }

    public void restore() {
        innerLog("restore ");
        if (this.bitmapCanvas == null) {
            return;
        }
        this.bitmapCanvas.restore();
    }

    public void setShadowColor(String color) {
        //warning unsupport
        throw new AssertionError("setShadowColor not support");
    }

    public void setShadowBlur(int v) {
        //warning unsupport
        throw new AssertionError("setShadowBlur not support");
    }

    public void setShadowOffsetX(float v) {
        //warning unsupport
        throw new AssertionError("setShadowOffsetX not support");
    }

    public void setShadowOffsetY(float v) {
        //warning unsupport
        throw new AssertionError("setShadowOffsetY not support");
    }

    public void drawImage(Bitmap bitmap, float dx, float dy) {
        innerLog("drawImage ");
        if (this.bitmapCanvas == null) {
            return;
        }
        this.bitmapCanvas.drawBitmap(bitmap, dx, dy, this.paint);
    }

    public void drawImage(Bitmap bitmap, float dx, float dy, float sw, float sh) {
        innerLog("drawImage ");
//        bitmapCanvas.drawBitmap(bitmap, new Rect);
        throw new AssertionError("drawImage not support");
    }

    private void setFillPaint() {
        innerLog("setFillPaint");
        if (this.bitmapCanvas == null) {
            return;
        }
        this.paint.setColor(this.fillStyle);
        this.paint.setAlpha((int) (this.globalAlpha * 255));
        this.paint.setStyle(Paint.Style.FILL);
    }

    private void setStrokePaint() {
        innerLog("setStrokePaint");
        if (this.bitmapCanvas == null) {
            return;
        }
        this.paint.setColor(this.strokeStyle);
        this.paint.setAlpha((int) (this.globalAlpha * 255));
        this.paint.setStyle(Paint.Style.STROKE);
    }

    void resetContext() {
        this.paint = new Paint();
        this.paint.setStrokeWidth(1 * ratio);
        this.paint.setAntiAlias(true);
        this.path = new Path();
        if (this.bitmap != null) {
            this.bitmap.eraseColor(Color.TRANSPARENT);
        }
    }
}
