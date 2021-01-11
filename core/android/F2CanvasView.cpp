//
// Created by 青原 on 2020-09-17.
//

#include "F2CanvasView.h"

#include <GCanvas/core/src/platform/Android/GFontManagerAndroid.h>
#include <GCanvas/core/src/support/Util.h>
#include <graphics/util/json.h>

#include "android/F2NativeJNI.h"

using namespace xg;

//// 静态设备gl信息(单进程只需获取一次)
// static std::string deviceGLVersion = "";
// static float deviceGLVersionShort = 0.0f;

void OnGCanvasCoreException(const char *canvasId, const char *tag, const char *detail, GCanvasHooks *selfHooks) {
    std::string _tag = std::string(canvasId) + "|" + std::string(tag);
    F2_LOG_E(_tag.data(), "OnGCanvasCoreException: %s", detail);
}

F2CanvasView::F2CanvasView(const std::string &jsonConfig) {
    nlohmann::json optionsCfg = xg::json::ParseString(jsonConfig);
    nlohmann::json cfg = {{"devicePixelRatio", 1}, {"canvas_width", 0}, {"canvas_height", 0}, {"backgroundColor", "#fff"}};
    if(optionsCfg.is_object()) {
        cfg.merge_patch(optionsCfg);
    }

    this->canvasId_ = cfg["canvas_id"];
    this->devicePixelRatio_ = cfg["devicePixelRatio"];
    this->canvas_width_ = cfg["canvas_width"];
    this->canvas_height_ = cfg["canvas_height"];
    this->backgroundColor_ = cfg["backgroundColor"];

    hooks_ = new GCanvasHooks();
    hooks_->GCanvasException = OnGCanvasCoreException;

    egl_context_ = new GEGLWindowContext(EGL_NO_DISPLAY, EGL_NO_CONTEXT);
    egl_context_->SetUsePreserveBackBuffer(false);
    egl_context_->SetTraceId(this->canvasId_);

    F2_LOG_I(canvasId_, "%s", "#F2CanvasView egl created");
}

F2CanvasView::~F2CanvasView() {
    Destroy();

    delete hooks_;
    if(fontManager_) {
        delete fontManager_;
    }
}

int F2CanvasView::InitCanvasContext() {
    F2_LOG_I(canvasId_, "%s", "#InitCanvasContext:start");
    bool shouldInit = true;

    if(canvas_context_init_) {
        F2_LOG_I(canvasId_, "%s", "#InitCanvasContext:skipped, already init.");
        return CODE_SUCCESS;
    }

    int retCode = CODE_SUCCESS;

    if(surface_destroyed_) {
        F2_LOG_W(canvasId_, "%s", "#InitCanvasContext:fail, surface destroyed.");
        shouldInit = false;
        retCode = CODE_INIT_CANVAS_CONTEXT_FAIL;
    }

    bool initEGLSuccess = true;

    if(shouldInit) {
        ANativeWindow *native_window = system_native_window_;

        if(!egl_context_->HasInit()) {
            F2_LOG_I(canvasId_, "#InitCanvasContext:InitEGL, window=%p", native_window);
            bool v = egl_context_->Init(native_window);
            if(!v) {
                F2_LOG_E(canvasId_, "%s", "#InitCanvasContext:InitEGL:fail, egl error");
                initEGLSuccess = false;
                retCode = CODE_INIT_EGL_FAIL;
            }
        }

        F2_LOG_I(canvasId_, "#InitCanvasContext:InitEGL, ret: %s", initEGLSuccess ? "success" : "fail");

        egl_surface_valid_ = initEGLSuccess;

        if(initEGLSuccess) {
            MakeCurrent();
            int ew = egl_context_->GetWidth();
            int eh = egl_context_->GetHeight();

            // if (deviceGLVersion.empty()) {
            //     const GLubyte *version = glGetString(GL_VERSION);
            //     deviceGLVersion = (const char *) version;
            //     deviceGLVersionShort = GpuInfo::GetGLESShortVersion(deviceGLVersion);
            // }

            // F2_LOG_I(canvasId_, "#InitCanvasContext:info w:%i, h:%i, glVersion=%f", ew, eh, deviceGLVersionShort);
            if(canvas_ == nullptr) {
                retCode = CreateCanvas2DImpl();
            }
        }
    }

    canvas_context_init_ = true;
    canvas_context_valid_ = initEGLSuccess;

    if(initEGLSuccess) {
        retCode = PerformSwapFrame();
    }

    F2_LOG_I(canvasId_, "#InitCanvasContext:end ret: %d", retCode);
    return retCode;
}

int F2CanvasView::OnSurfaceCreated(ANativeWindow *nativeWindow) {
    UpdateSurfaceCreatedStatus();

    system_native_window_ = nativeWindow;
    if(nativeWindow != nullptr) {
        view_width_ = ANativeWindow_getWidth(nativeWindow);
        view_height_ = ANativeWindow_getHeight(nativeWindow);
    }

    F2_LOG_I(canvasId_, "#OnSurfaceCreated view_width: %lf, view_height: %lf", view_width_, view_height_);
    if(canvas_width_ <= 0) {
        canvas_width_ = view_width_ / devicePixelRatio_;
    }
    if(canvas_height_ <= 0) {
        canvas_height_ = view_height_ / devicePixelRatio_;
    }

    if(system_native_window_ != nullptr) {
        ANativeWindow_setBuffersGeometry(system_native_window_, canvas_width_, canvas_height_, AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM);
    }

    if(surface_destroyed_) {
        F2_LOG_E(canvasId_, "%s", "#OnSurfaceCreated:skip, surface destroy");
        return CODE_FAIL_UNKNOWN;
    }

    int retCode = CODE_SUCCESS;
    if(egl_context_ != nullptr && egl_context_->HasInit()) {
        retCode = ResumeCanvas(system_native_window_);
    } else {
        retCode = CODE_INIT_EGL_FAIL;
        F2_LOG_I(canvasId_, "%s", "eglContext is not init");
    }
    return retCode;
}

int F2CanvasView::OnSurfaceChanged(int width, int height) {
    F2_LOG_I(canvasId_, "#OnSurfaceChanged width: %d, hegith: %d", width, height);
    if(surface_destroyed_) {
        F2_LOG_I(canvasId_, "%s", "#OnSurfaceChanged:skip surface destroyed");
        return CODE_FAIL_UNKNOWN;
    }

    if(egl_context_ == nullptr || !egl_context_->HasInit()) {
        F2_LOG_I(canvasId_, "%s", "#OnSurfaceChanged:skip egl not init");
        return CODE_FAIL_UNKNOWN;
    }

    UpdateSurfaceChangedStatus(width, height);

    egl_surface_valid_ = true;
    if(system_native_window_ != nullptr) {
        ANativeWindow_setBuffersGeometry(system_native_window_, canvas_width_, canvas_height_, AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM);
    }

    if(canvas_ == nullptr || !canvas_context_valid_) {
        F2_LOG_I(canvasId_, "%s", "#OnSurfaceChanged only resize egl, canvas is null.");
        egl_context_->Resize(canvas_width_, canvas_height_);
    } else {
        bool resizeFlag = egl_context_->Resize(canvas_width_, canvas_height_);
        return PerformSwapFrame();
    }
    return CODE_SUCCESS;
}

int F2CanvasView::OnSurfaceDestroyed() {
    UpdateSurfaceDestroyedStatus();
    if(!canvas_context_init_) {
        F2_LOG_I(canvasId_, "%s", "#OnSurfaceDestroyed:skip canvas context not init");
        return CODE_SUCCESS;
    }

    F2_LOG_I(canvasId_, "%s", "#OnSurfaceDestroyed:start");
    if(egl_context_ != nullptr && egl_context_->IsContextValid()) {
        MakeCurrent();

        if(canvas_2d_context_ != nullptr) {
            canvas_2d_context_->EndDraw();
        }
        egl_context_->Pause();
    }

    if(system_native_window_ != nullptr) {
        F2_LOG_I(canvasId_, "#OnSurfaceDestroyed releaseNativeWindow: %p", system_native_window_);
        ANativeWindow_release(system_native_window_);
        system_native_window_ = nullptr;
    }
    F2_LOG_I(canvasId_, "%s", "#OnSurfaceDestroyed:end");
    return CODE_SUCCESS;
}

int F2CanvasView::Destroy() {
    // F2_LOG_I(canvasId_, "%s", "#Destroy:start");
    if(has_destroy_) {
        // F2_LOG_I(canvasId_, "%s", "#Destroy has destroyed");
        return CODE_SUCCESS;
    }

    has_destroy_ = true;

    // 渲染线程清理graphic相关
    if(egl_context_ != nullptr && egl_context_->IsContextValid()) {
        // 由于surface删除时会clearCurrent, 为确保后续gl清理执行，需要先进行makeCurrent
        MakeCurrent();
    }

    if(canvas_ != nullptr) {
        delete canvas_;
        canvas_2d_context_ = nullptr;
    }

    if(egl_context_ != nullptr) {
        // F2_LOG_I(canvasId_, "%s", "#Destroy free egl context");
        // free egl_context会触发egl terminate
        delete egl_context_;
        egl_context_ = nullptr;
    }
    // F2_LOG_I(canvasId_, "%s", "#Destroy:end");
    return CODE_SUCCESS;
}

int F2CanvasView::PerformSwapFrame() {
    if(egl_context_ == nullptr || !egl_context_->IsValid() || !egl_surface_valid_ || system_native_window_ == nullptr || canvas_ == nullptr) {
        F2_LOG_E(canvasId_, "%s", "#PerformSwapFrame condition is not match");
        return CODE_FAIL_UNKNOWN;
    }
    canvas_2d_context_->DrawFrame(false);
    canvas_2d_context_->EndDraw();
    egl_context_->SwapBuffer();
    canvas_2d_context_->BeginDraw();
    F2_LOG_I(canvasId_, "%s", "#PerformSwapFrame success");
    return CODE_SUCCESS;
}

void F2CanvasView::MakeCurrent() {
    if(egl_context_ != nullptr) {
        egl_context_->MakeCurrent();
    }
}

int F2CanvasView::CreateCanvas2DImpl() {
    if(this->canvas_ != nullptr) {
        return CODE_SUCCESS;
    }

    int result = CODE_SUCCESS;

    GCanvasConfig config;
    config.useFbo = true;
    config.flip = false;
    config.sharedFont = true;
    config.sharedShader = false;

    GCanvasAndroid *_canvas = new GCanvasAndroid("GCanvasAndroid", config, hooks_);
    GCanvas2DContextAndroid *_canvas_2d_context = _canvas->GetCanvasContextAndroid();

    fontManager_ = new GFontManagerAndroid();

    _canvas_2d_context->SetFontManager(fontManager_);
    _canvas_2d_context->SetDevicePixelRatio(1);
    _canvas_2d_context->SetEnableFboMsaa(false);
    _canvas_2d_context->SetClearColor(gcanvas::StrValueToColorRGBA(backgroundColor_.data()));

    _canvas->OnSurfaceChanged(0, 0, egl_context_->GetWidth(), egl_context_->GetHeight());

    if(_canvas->IsGCanvasReady()) {
        this->canvas_ = _canvas;
        this->canvas_2d_context_ = _canvas_2d_context;

        canvas_2d_context_->BeginDraw(true);
        F2_LOG_I(canvasId_, "#CreateCanvas2DImpl:finish canvas2dContext:%p w:%i h:%i cw:%lf ch:%lf", canvas_2d_context_,
                 egl_context_->GetWidth(), egl_context_->GetHeight(), canvas_width_, canvas_height_);
    } else {
        _canvas_2d_context->BeginDraw(true);
        this->canvas_ = _canvas;
        this->canvas_2d_context_ = _canvas_2d_context;

        F2_LOG_I(canvasId_, "#CreateCanvas2DImpl:Fail canvas2dContext:%p w:%i h:%i cw:%lf ch:%lf", canvas_2d_context_,
                 egl_context_->GetWidth(), egl_context_->GetHeight(), canvas_width_, canvas_height_);
        result = CODE_CREATE_CANVAS_2D_CONTEXT_FAIL;
    }
    return result;
}

int F2CanvasView::ResumeCanvas(ANativeWindow *window) {
    F2_LOG_I(canvasId_, "%s", "#ResumeCanvas:start");

    if(!egl_context_->Resume(window)) {
        F2_LOG_E(canvasId_, "%s", "#ResumeCanvas:egl resume error");
        return CODE_RESUME_CANVAS_FAIL;
    }

    egl_surface_valid_ = true;
    F2_LOG_E(canvasId_, "#ResumeCanvas:canvas_: %p", canvas_);
    if(canvas_ != nullptr) {
        canvas_2d_context_->BeginDraw(true);
        PerformSwapFrame();
    }
    F2_LOG_I(canvasId_, "%s", "#ResumeCanvas:end");
    return CODE_SUCCESS;
}

void F2CanvasView::UpdateSurfaceCreatedStatus() {
    surface_created_ = true;
    surface_changed_ = false;
    surface_destroyed_ = false;
}

void F2CanvasView::UpdateSurfaceChangedStatus(int width, int height) {
    surface_created_ = false;
    surface_changed_ = true;
    surface_destroyed_ = false;
}

void F2CanvasView::UpdateSurfaceDestroyedStatus() {
    surface_created_ = false;
    surface_changed_ = false;
    surface_destroyed_ = true;
}

////////////////////////////////////// graphic 2d apis ///////////////////////////////////////////////////////

float F2CanvasView::MeasureText(const std::string &text) {
    if(canvas_context_valid_) {
        return canvas_2d_context_->MeasureTextWidth(text.data());
    }
    return 0;
}

void F2CanvasView::SetFillStyle(const std::string &style) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetFillStyle(style.data());
    }
}

void F2CanvasView::SetFillStyle(int r, int g, int b, float a) {
    if(canvas_context_valid_) {
        GColorRGBA value;
        value.components[0] = r * 1.0f / 255;
        value.components[1] = g * 1.0f / 255;
        value.components[2] = b * 1.0f / 255;
        value.components[3] = a;

        canvas_2d_context_->SetFillStyle(value);
    }
}

void F2CanvasView::SetStrokeStyle(const std::string &style) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetStrokeStyle(style.data());
    }
}

void F2CanvasView::SetLinearGradientFillStyle(float x0, float y0, float x1, float y1, std::vector<float> &positions, std::vector<std::string> &colors, bool is_stroke) {
    if(canvas_context_valid_ && !positions.empty() && !colors.empty()) {
        float startArr[2] = {x0, y0};
        float endArr[2] = {x1, y1};
        canvas_2d_context_->SetFillStyleLinearGradient(startArr, endArr, colors.size(), &positions[0], &colors[0], is_stroke);
    }
}

void F2CanvasView::SetPatternFillStyle(int textureId, int width, int height, std::string &&mode, bool is_stroke) {
    if(canvas_context_valid_) {
        if(textureId < 0) {
            F2_LOG_E(canvasId_, "SetPatternFillStyle fail: %i not exist", textureId);
        } else {
            canvas_2d_context_->SetFillStylePattern(textureId, width, height, mode.c_str(), is_stroke);
        }
    }
}

void F2CanvasView::Rect(float x, float y, float width, float height) {
    if(canvas_context_valid_) {
        canvas_2d_context_->Rect(x, y, width, height);
    }
}

void F2CanvasView::FillRect(float x, float y, float width, float height) {
    if(canvas_context_valid_) {
        canvas_2d_context_->FillRect(x, y, width, height);
    }
}

void F2CanvasView::StrokeRect(float x, float y, float width, float height) {
    if(canvas_context_valid_) {
        canvas_2d_context_->StrokeRect(x, y, width, height);
    }
}

void F2CanvasView::ClearRect(float x, float y, float width, float height) {
    if(canvas_context_valid_) {
        canvas_2d_context_->ClearRect(x, y, width, height);
    }
}

void F2CanvasView::Fill(GFillRule fr) {
    if(canvas_context_valid_) {
        canvas_2d_context_->Fill(fr);
    }
}

void F2CanvasView::Stroke() {
    if(canvas_context_valid_) {
        canvas_2d_context_->Stroke();
    }
}

void F2CanvasView::BeginPath() {
    if(canvas_context_valid_) {
        canvas_2d_context_->BeginPath();
    }
}

void F2CanvasView::MoveTo(float x, float y) {
    if(canvas_context_valid_) {
        canvas_2d_context_->MoveTo(x, y);
    }
}

void F2CanvasView::ClosePath() {
    if(canvas_context_valid_) {
        canvas_2d_context_->ClosePath();
    }
}

void F2CanvasView::LineTo(float x, float y) {
    if(canvas_context_valid_) {
        canvas_2d_context_->LineTo(x, y);
    }
}

void F2CanvasView::Clip(GFillRule f) {
    if(canvas_context_valid_) {
        canvas_2d_context_->Clip(f);
    }
}

void F2CanvasView::QuadraticCurveTo(float cpx, float cpy, float x, float y) {
    if(canvas_context_valid_) {
        canvas_2d_context_->QuadraticCurveTo(cpx, cpy, x, y);
    }
}

void F2CanvasView::BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
    if(canvas_context_valid_) {
        canvas_2d_context_->BezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
    }
}

void F2CanvasView::Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise) {
    if(canvas_context_valid_) {
        canvas_2d_context_->Arc(x, y, r, sAngle, eAngle, antiClockwise);
    }
}

void F2CanvasView::ArcTo(float x1, float y1, float x2, float y2, float r) {
    if(canvas_context_valid_) {
        canvas_2d_context_->ArcTo(x1, y1, x2, y2, r);
    }
}

void F2CanvasView::Scale(float scalewidth, float scaleheight) {
    if(canvas_context_valid_) {
        canvas_2d_context_->Scale(scalewidth, scaleheight);
    }
}

void F2CanvasView::Rotate(float angle) {
    if(canvas_context_valid_) {
        canvas_2d_context_->Rotate(angle);
    }
}

void F2CanvasView::Translate(float x, float y) {
    if(canvas_context_valid_) {
        canvas_2d_context_->Translate(x, y);
    }
}

void F2CanvasView::Transform(float a, float b, float c, float d, float e, float f) {
    if(canvas_context_valid_) {
        canvas_2d_context_->Transfrom(a, b, c, d, e, f);
    }
}

void F2CanvasView::SetTransform(float a, float b, float c, float d, float e, float f) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetTransform(a, b, c, d, e, f);
    }
}

void F2CanvasView::SetLineCap(const std::string &lineCap) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetLineCap(lineCap.data());
    }
}

void F2CanvasView::SetLineJoin(const std::string &lineJoin) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetLineJoin(lineJoin.data());
    }
}

void F2CanvasView::SetLineWidth(float lineWidth) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetLineWidth(lineWidth);
    }
}

void F2CanvasView::SetLineDashOffset(float offset) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetLineDashOffset(offset);
    }
}

void F2CanvasView::SetLineDash(std::vector<float> params) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetLineDash(params);
    }
}

std::vector<float> &F2CanvasView::GetLineDash() { return canvas_2d_context_->GetLineDash(); }

void F2CanvasView::SetMiterLimit(float limit) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetMiterLimit(limit);
    }
}

void F2CanvasView::SetFont(const std::string &font) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetFont(font.data());
    }
}

void F2CanvasView::SetTextAlign(const std::string &textAlign) {
    if(canvas_context_valid_) {
        bool validType = true;
        GTextAlign textAlignEnum;
        if(textAlign == "start") {
            textAlignEnum = TEXT_ALIGN_START;
        } else if(textAlign == "end") {
            textAlignEnum = TEXT_ALIGN_END;
        } else if(textAlign == "center") {
            textAlignEnum = TEXT_ALIGN_CENTER;
        } else if(textAlign == "left") {
            textAlignEnum = TEXT_ALIGN_LEFT;
        } else if(textAlign == "right") {
            textAlignEnum = TEXT_ALIGN_RIGHT;
        } else {
            validType = false;
        }
        if(validType) {
            canvas_2d_context_->SetTextAlign(textAlignEnum);
        }
    }
}

void F2CanvasView::SetTextAlign(GTextAlign &textAlign) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetTextAlign(textAlign);
    }
}

GTextAlign F2CanvasView::TextAlign() const {
    if(canvas_context_valid_) {
        return canvas_2d_context_->TextAlign();
    }
    return TEXT_ALIGN_CENTER;
}

void F2CanvasView::SetTextBaseline(const std::string &textBaseline) {
    if(canvas_context_valid_) {
        GTextBaseline baselineEnum;
        bool valid = true;

        if(textBaseline == "alphabetic") {
            baselineEnum = TEXT_BASELINE_ALPHABETIC;
        } else if(textBaseline == "top") {
            baselineEnum = TEXT_BASELINE_TOP;
        } else if(textBaseline == "hanging") {
            baselineEnum = TEXT_BASELINE_HANGING;
        } else if(textBaseline == "middle") {
            baselineEnum = TEXT_BASELINE_MIDDLE;
        } else if(textBaseline == "ideographic") {
            baselineEnum = TEXT_BASELINE_IDEOGRAPHIC;
        } else if(textBaseline == "bottom") {
            baselineEnum = TEXT_BASELINE_BOTTOM;
        } else {
            valid = false;
        }
        if(valid) {
            canvas_2d_context_->SetTextBaseline(baselineEnum);
        }
    }
}

void F2CanvasView::SetTextBaseline(GTextBaseline &textBaseline) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetTextBaseline(textBaseline);
    }
}

GTextBaseline F2CanvasView::TextBaseline() const {
    if(canvas_context_valid_) {
        return canvas_2d_context_->TextBaseline();
    }
    return TEXT_BASELINE_BOTTOM;
}

void F2CanvasView::FillText(const std::string &text, float x, float y, float maxWidth) {
    if(isnan(maxWidth) || maxWidth <= 0) {
        F2_LOG_E(canvasId_, "GCanvasView call 2dcontext execDrawText: invalid maxWidth=%f", maxWidth);
        return;
    }
    if(canvas_context_valid_) {
        canvas_2d_context_->DrawText(text.data(), x, y, maxWidth);
    }
}

void F2CanvasView::StrokeText(const std::string &text, float x, float y, float maxWidth) {
    if(isnan(maxWidth) || maxWidth <= 0) {
        F2_LOG_E(canvasId_, "GCanvasView call 2dcontext execDrawText: invalid maxWidth=%f", maxWidth);
        return;
    }
    if(canvas_context_valid_) {
        canvas_2d_context_->StrokeText(text.data(), x, y, maxWidth);
    }
}

void F2CanvasView::SetGlobalAlpha(float globalAlpha) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetGlobalAlpha(globalAlpha);
    }
}

float F2CanvasView::GlobalAlpha() {
    if(canvas_context_valid_) {
        return canvas_2d_context_->GlobalAlpha();
    }
    return 0;
}

void F2CanvasView::SetGlobalCompositeOp(const std::string &op) {
    if(canvas_context_valid_) {
        const char *pp = op.data();
        GCompositeOperation gop = gcanvas::StringToGlobalCompositeOp(pp);
        if(gop != GCompositeOperation::COMPOSITE_OP_NONE) {
            canvas_2d_context_->SetGlobalCompositeOperation(gop);
        }
    }
}

void F2CanvasView::Save() {
    if(canvas_context_valid_) {
        canvas_2d_context_->Save();
    }
}

void F2CanvasView::Restore() {
    if(canvas_context_valid_) {
        canvas_2d_context_->Restore();
    }
}

void F2CanvasView::SetShadowOffsetX(float v) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetShadowOffsetX(v);
    }
}

void F2CanvasView::SetShadowOffsetY(float v) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetShadowOffsetY(v);
    }
}

void F2CanvasView::SetShadowColor(const char *color) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetShadowColor(color);
    }
}

void F2CanvasView::SetShadowBlur(float v) {
    if(canvas_context_valid_) {
        canvas_2d_context_->SetShadowBlur(v);
    }
}

bool F2CanvasView::HasClip() {
    bool result = false;
    if(canvas_context_valid_) {
        result = canvas_2d_context_->HasClipRegion();
    }
    return result;
}