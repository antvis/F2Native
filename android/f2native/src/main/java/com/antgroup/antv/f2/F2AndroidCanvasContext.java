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
import android.graphics.SweepGradient;
import android.graphics.Typeface;
import android.os.Build;
import android.text.TextUtils;

import com.antgroup.antv.f2.base.F2BaseCanvasContext;

/**
 * android native canvas需要的CanvasContext
 */
public class F2AndroidCanvasContext implements F2BaseCanvasContext {
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

    public F2AndroidCanvasContext(int width, int height, float ratio) {
        height = Math.max(height, 1);
        width = Math.max(width, 1);
        this.ratio = ratio;
        try {
            this.bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            this.bitmapCanvas = new Canvas(this.bitmap);
        } catch (OutOfMemoryError e) {
            mHadOOM = true;
            F2Log.get().e("F2AndroidCanvasContext", "createBitmap OutOfMemoryError");
        }
        resetContext();
    }

    private void debugLog(String content) {

    }

    private void innerLog(String content) {
        F2Log.get().i("F2AndroidCanvasContext", hashCode() + ", " + content);
    }

    //set
    public void setStrokeStyle(String style) {
        debugLog("setStrokeStyle style:" + style);
        if (this.bitmapCanvas == null) {
            innerLog("setStrokeStyle null");
            return;
        }
        try {
            int color = Color.parseColor(style);
            setStrokeStyle(color);
        } catch (Exception e) {
            F2Log.get().e("F2AndroidCanvasContext", "#setStrokeStyle exception " + e.toString());
        }
    }

    public void setStrokeStyle(int color) {
        debugLog("setStrokeStyle darkModeColor:" + color);
        if (this.paint == null) {
            innerLog("setStrokeStyle 2 null");
            return;
        }
        this.strokeStyle = color;
        this.paint.setShader(null);
    }

    //线性渐变
    public void setLinearGradient(float sx, float sy, float ex, float ey, int[] colors, float[] positions) {
        debugLog("setLinearGradient sx:" + sx + " sy:" + sy + " ex:" + ex + " ey:" + ey);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setLinearGradient null");
            return;
        }
        if (colors.length != positions.length && colors.length < 2) {
            innerLog("setLinearGradient length = 0 ");
            return;
        }
        LinearGradient gradient = new LinearGradient(sx, sy, ex, ey, colors, positions, Shader.TileMode.CLAMP);
        this.paint.setShader(gradient);
    }

    //雷达渐变
    public void setRadialGradient(float sx, float sy, float sr, float ex, float ey, float er, int[] colors, float[] positions) {
        debugLog("setRadialGradient sx:" + sx + " sy:" + sy + " sr:" + sr + " ex:" + ex + " ey:" + ey + " er:" + er);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setRadialGradient null");
            return;
        }
        if (colors.length != positions.length && colors.length < 2) {
            innerLog("setRadialGradient length = 0 ");
            return;
        }
        RadialGradient gradient = new RadialGradient(sx, sy, sr, colors, positions, Shader.TileMode.CLAMP);
        this.paint.setShader(gradient);
    }

    //扫描渐变
    public void setConicGradient(float cx, float cy, int[] colors, float[] positions) {
        debugLog("setConicGradient cx:" + cx + " cy:" + cy);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setConicGradient null");
            return;
        }
        if (colors.length != positions.length && colors.length < 2) {
            innerLog("setConicGradient length = 0 ");
            return;
        }
        SweepGradient gradient = new SweepGradient(cx, cy, colors, positions);

        Matrix matrix = new Matrix();
        matrix.setRotate(-90, cx, cy);
        gradient.setLocalMatrix(matrix);

        this.paint.setShader(gradient);
    }

    public void setFillStyle(String style) {
        debugLog("setFillStyle style:" + style);
        if (this.bitmapCanvas == null) {
            innerLog("setFillStyle null");
            return;
        }
        try {
            int color = Color.parseColor(style);
            setFillStyle(color);
        } catch (Exception e) {
            F2Log.get().e("F2AndroidCanvasContext", "#setFillStyle exception " + e.toString());
        }
    }

    public void setFillStyle(int color) {
        debugLog("setFillStyle darkModeColor:" + color);
        if (this.paint == null) {
            innerLog("setFillStyle 2 null");
            return;
        }
        this.fillStyle = color;
        this.paint.setShader(null);
    }

    public void rect(float x, float y, float width, float height) {
        debugLog("rect x:" + x + " y:" + y + " width:" + width + " height:" + height);
        if (this.bitmapCanvas == null || this.path == null) {
            innerLog("rect null");
            return;
        }
        //cw?
        this.path.addRect(x, y, x + width, y + height, Path.Direction.CW);
    }

    public void fillRect(float x, float y, float width, float height) {
        debugLog("fillRect float x:" + x + " y:" + y + " width:" + width + " height:" + height);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("fillRect null");
            return;
        }
        setFillPaint();
        this.bitmapCanvas.drawRect(x, y, x + width, y + height, this.paint);
    }

    public void fillRect(int x, int y, int width, int height) {
        debugLog("fillRect int x:" + x + " y:" + y + " width:" + width + " height:" + height);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("fillRect int null");
            return;
        }
        setFillPaint();
        this.bitmapCanvas.drawRect(x, y, x + width, y + height, this.paint);
    }

    public void strokeRect(float x, float y, float width, float height) {
        debugLog("strokeRect x:" + x + " y:" + y + " width:" + width + " height:" + height);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("strokeRect null");
            return;
        }
        setStrokePaint();
        this.bitmapCanvas.drawRect(x, y, x + width, y + height, this.paint);
    }

    public void clearRect(float x, float y, float width, float height) {
        debugLog("clearRect x:" + x + " y:" + y + " width:" + width + " height:" + height);
        if (this.bitmapCanvas == null) {
            innerLog("clearRect null");
            return;
        }
        this.bitmapCanvas.save();
        RectF rectF = new RectF(x, y, x + width, y + height);
        this.bitmapCanvas.clipRect(rectF);
        this.bitmapCanvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
        this.bitmapCanvas.restore();
    }

    public void stroke() {
        debugLog("stroke");
        if (this.bitmapCanvas == null || this.paint == null || this.path == null) {
            innerLog("stroke null");
            return;
        }
        setStrokePaint();
        this.bitmapCanvas.drawPath(this.path, this.paint);
    }

    public void fill() {
        debugLog("fill");
        if (this.bitmapCanvas == null || this.paint == null || this.path == null) {
            innerLog("fill null");
            return;
        }
        setFillPaint();
        this.bitmapCanvas.drawPath(this.path, this.paint);
    }

    public void beginPath() {
        debugLog("beginPath");
        if (this.bitmapCanvas == null) {
            innerLog("beginPath null");
            return;
        }
        this.path = new Path();
    }

    public void closePath() {
        debugLog("closePath");
        if (this.bitmapCanvas == null || this.path == null) {
            innerLog("closePath null");
            return;
        }
        this.path.close();
    }

    public void lineTo(float x, float y) {
        debugLog("lineTo x:" + x + " y:" + y);
        if (this.bitmapCanvas == null || this.path == null) {
            innerLog("lineTo null");
            return;
        }
        this.path.lineTo(x, y);
    }

    public void moveTo(float x, float y) {
        debugLog("moveTo x:" + x + " y:" + y);
        if (this.bitmapCanvas == null || this.path == null) {
            innerLog("moveTo null");
            return;
        }
        this.path.moveTo(x, y);
    }

    public void clip() {
        debugLog("clip");
        if (this.bitmapCanvas == null || this.path == null) {
            innerLog("clip null");
            return;
        }
        this.bitmapCanvas.clipPath(this.path);
        //新开启一个路径
        beginPath();
    }

    public void quadraticCurveTo(float cpx, float cpy, float x, float y) {
        debugLog("quadraticCurveTo cpx:" + cpx + " cpy:" + cpy + " x:" + x + " y:" + y);
        if (this.bitmapCanvas == null || this.path == null) {
            innerLog("quadraticCurveTo null");
            return;
        }
        this.path.quadTo(cpx, cpy, x, y);
    }

    public void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
        debugLog("bezierCurveTo cp1x:" + cp1x + " cp1y:" + cp1y + "cp2x:" + cp2x + " cp2y:" + cp2y + " x:" + x + " y:" + y);
        if (this.bitmapCanvas == null || this.path == null) {
            innerLog("bezierCurveTo null");
            return;
        }
        this.path.cubicTo(cp1x, cp1y, cp2x, cp2y, x, y);
    }

    public void arc(float x, float y, float r, float sAngle, float eAngle, boolean antiClockwise) {
        debugLog("arc x:" + x + " y:" + y + " sAngle：" + sAngle + " eAngle：" + eAngle);
        if (this.bitmapCanvas == null || this.path == null) {
            innerLog("arc null");
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
        debugLog("arcTo x1:" + x1 + " y1:" + y1 + " x2:" + x2 + " y2:" + y2);
        if (this.bitmapCanvas == null || this.path == null) {
            innerLog("arcTo null");
            return;
        }
        this.path.arcTo(new RectF(x1 - 2 * r, y1, x2, y2 + r), -90, 90, false);
    }

    public void scale(float sw, float sh) {
        debugLog("scale sw:" + sw + " sh:" + sh);
        if (this.bitmapCanvas == null) {
            innerLog("scale null");
            return;
        }
        this.bitmapCanvas.scale(sw, sh);
    }

    public void rotate(float angle) {
        debugLog("rotate angle:" + angle);
        if (this.bitmapCanvas == null) {
            innerLog("rotate null");
            return;
        }
        this.bitmapCanvas.rotate(angle);
    }

    public void translate(float x, float y) {
        debugLog("translate x:" + x + " y:" + y);
        if (this.bitmapCanvas == null) {
            innerLog("translate null");
            return;
        }
        this.bitmapCanvas.translate(x, y);
    }

    public void transform(float a, float b, float c, float d, float e, float f) {
        debugLog("transform a:" + a + " b:" + b + " c:" + c + " d:" + d);
        if (this.bitmapCanvas == null) {
            innerLog("transform null");
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
        debugLog("setTransform a:" + a + " b:" + b + " c:" + c + " d:" + d);
        if (this.bitmapCanvas == null) {
            innerLog("setTransform null");
            return;
        }
        Matrix m = new Matrix();
        m.reset();
        m.preTranslate(e, f);
        m.preScale(a, d);
        m.preSkew(c, b);
        this.bitmapCanvas.setMatrix(m);
    }

    public void setLineWidth(float lineWidth) {
        debugLog("setLineWidth lineWidth:" + lineWidth);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setLineWidth null");
            return;
        }
        this.paint.setStrokeWidth(lineWidth);
    }

    public void setLineDash(float[] dashes) {
        debugLog("setLineDash dashes: " + (dashes == null ? 0 : dashes.length));
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setLineDash null");
            return;
        }
        if (dashes == null || dashes.length == 0) {
            //clear path effect
            this.paint.setPathEffect(new PathEffect());
        } else if (dashes.length == 1) {
            //<2 DashPathEffect会抛出异常
            this.paint.setPathEffect(new DashPathEffect(new float[]{dashes[0], dashes[0]}, 0));
        } else {
            this.paint.setPathEffect(new DashPathEffect(dashes, 0));
        }
    }

    public void setMiterLimit(float miterLimit) {
        debugLog("setMiterLimit miterLimit:" + miterLimit);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setMiterLimit null");
            return;
        }
        this.paint.setStrokeMiter(miterLimit);
    }

    public void setFont(int style, int variant, int weight, String name, float size) {
        debugLog("setFont style:" + style + " variant:" + variant + " weight:" + weight + " name:" + name + " size:" + size);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setFont null");
            return;
        }
        if (TextUtils.isEmpty(name)) {
            return;
        }
        Typeface typeface;
        Typeface baseFace = F2CommonHelper.get().getAlipayNumber(name);
        if (baseFace == null) {
            //NDK 16只支持这个方式创建Typeface
            baseFace = Typeface.create(name, style);
        }
        // Android 9 才生效
        if (weight >= 50 && weight <= 1000 && Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            typeface = Typeface.create(baseFace, weight, false);
        } else {
            typeface = baseFace;
        }
        this.paint.setTypeface(typeface);
        this.paint.setTextSize(size);
    }

    public void setTextAlign(String textAlign) {
        debugLog("setTextAlign textAlign:" + textAlign);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setTextAlign null");
            return;
        }
        if (TextUtils.isEmpty(textAlign)) {
            innerLog("setTextAlign " + textAlign);
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
        debugLog("getTextAlign ");
        return textAlign;
    }

    public void setTextBaseline(String textBaseline) {
        debugLog("setTextBaseline textBaseline:" + textBaseline);
        if (TextUtils.isEmpty(textBaseline)) {
            innerLog("setTextBaseline " + textBaseline);
            return;
        }
        this.textBaseline = textBaseline;
    }

    public String getTextBaseline() {
        debugLog("getTextBaseline");
        return textBaseline;
    }

    public void drawText(String text, float x, float y) {
        debugLog("drawText text:" + text + " x:" + x + " y:" + y);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("drawText null");
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
        debugLog("fillText text:" + text + " x:" + x + " y:" + y);
        if (text == null) {
            return;
        }
        setFillPaint();
        drawText(text, x, y);
    }

    public void strokeText(String text, float x, float y) {
        debugLog("strokeText text:" + text + " x:" + x + " y:" + y);
        if (text == null) {
            return;
        }
        setStrokePaint();
        drawText(text, x, y);
    }

    public float measureText(String text) {
        debugLog("measureText text:" + text);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("measureText null");
            return 0;
        }
        if (text == null) {
            return 0;
        }
        return this.paint.measureText(text);
    }

    public float measureTextHeight(String text) {
        debugLog("measureTextHeight text:" + text);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("measureTextHeight null");
            return 0;
        }
        if (text == null) {
            return 0;
        }
        Paint.FontMetrics fm = this.paint.getFontMetrics();
        return fm.descent - fm.ascent;
    }

    public void setGlobalAlpha(float alpha) {
        debugLog("setGlobalAlpha alpha:" + alpha);
        alpha = Math.max(alpha, 0);
        alpha = Math.min(alpha, 1);
        this.globalAlpha = alpha;
    }

    public float getGlobalAlpha() {
        debugLog("getGlobalAlpha");
        return this.globalAlpha;
    }

    public void save() {
        debugLog("save");
        if (this.bitmapCanvas == null) {
            innerLog("save null");
            return;
        }
        this.bitmapCanvas.save();
    }

    public void restore() {
        debugLog("restore ");
        if (this.bitmapCanvas == null) {
            innerLog("restore null");
            return;
        }
        this.bitmapCanvas.restore();
    }

    public void drawImage(Bitmap bitmap, float dx, float dy) {
        debugLog("drawImage ");
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("drawImage null");
            return;
        }
        this.bitmapCanvas.drawBitmap(bitmap, dx, dy, this.paint);
    }

    public void drawImageRect(Bitmap bitmap, float dx, float dy, float sw, float sh) {
        debugLog("drawImageRect dx: " + dx + ",dy: " + dy + ",sw: " + sw + ",sh: " + sh);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("drawImageRect null");
            return;
        }
        this.bitmapCanvas.drawBitmap(bitmap, null, new RectF(dx, dy, dx + sw, dy + sh), this.paint);
    }

    public void setFillPaint() {
        debugLog("setFillPaint");
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setFillPaint null");
            return;
        }
        this.paint.setColor(this.fillStyle);
        this.paint.setAlpha((int) (this.globalAlpha * 255));
        this.paint.setStyle(Paint.Style.FILL);
    }

    public void setLineJoin(String lineJoin) {
        debugLog("setLineJoin " + lineJoin);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setLineJoin null");
            return;
        }
        if (TextUtils.isEmpty(lineJoin)) {
            innerLog("setLineJoin empty");
            return;
        }
        if ("miter".equalsIgnoreCase(lineJoin)) {
            this.paint.setStrokeJoin(Paint.Join.MITER);
        } else if ("round".equalsIgnoreCase(lineJoin)) {
            this.paint.setStrokeJoin(Paint.Join.ROUND);
        } else if ("bevel".equalsIgnoreCase(lineJoin)) {
            this.paint.setStrokeJoin(Paint.Join.BEVEL);
        }
    }

    public void setLineCap(String lineCap) {
        debugLog("setLineCap " + lineCap);
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setLineCap null");
            return;
        }
        if (TextUtils.isEmpty(lineCap)) {
            innerLog("setLineCap empty");
            return;
        }
        if ("butt".equalsIgnoreCase(lineCap)) {
            this.paint.setStrokeCap(Paint.Cap.BUTT);
        } else if ("round".equalsIgnoreCase(lineCap)) {
            this.paint.setStrokeCap(Paint.Cap.ROUND);
        } else if ("square".equalsIgnoreCase(lineCap)) {
            this.paint.setStrokeCap(Paint.Cap.SQUARE);
        }
    }

    public void setStrokePaint() {
        debugLog("setStrokePaint");
        if (this.bitmapCanvas == null || this.paint == null) {
            innerLog("setStrokePaint null");
            return;
        }
        this.paint.setColor(this.strokeStyle);
        this.paint.setAlpha((int) (this.globalAlpha * 255));
        this.paint.setStyle(Paint.Style.STROKE);
    }

    public void resetContext() {
        innerLog("resetContext");
        this.paint = new Paint();
        this.paint.setStrokeWidth(1 * ratio);
        this.paint.setAntiAlias(true);
        this.path = new Path();
        if (this.bitmap != null) {
            this.bitmap.eraseColor(Color.TRANSPARENT);
        }
    }
}