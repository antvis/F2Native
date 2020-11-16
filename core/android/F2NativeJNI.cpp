#include "JNIUtil.h"

#include <android/log.h>
#include <jni.h>
#include <nlohmann/json.hpp>

#include "F2CanvasView.h"
#include "F2NativeJNI.h"
#include "JavaRef.h"

#include "graphics/XChart.h"
#include "graphics/geom/Geom.h"
#include "graphics/geom/Interval.h"
#include "graphics/guide/GuideController.h"

#ifndef xg_jni_arraysize

template <typename T, size_t N> char (&_xg_ArraySizeHelper(T (&array)[N]))[N];

#define xg_jni_arraysize(array) (sizeof(_xg_ArraySizeHelper(array)))
#endif // xg_jni_arraysize

// extern GCanvasSystemLog gcanvasSystemLog;

namespace xg {
namespace jni {

static jlong CreateNativeCanvasView(JNIEnv *env, jclass clazz, jstring stringOptions) {
    std::string _options = JavaStringToString(env, stringOptions);
#if defined(DEBUG)
    nlohmann::json optionsCfg = nlohmann::json::parse(_options);
#else
    nlohmann::json optionsCfg = nlohmann::json::parse(_options, nullptr, false);
#endif
    nlohmann::json defaultCfg = {{"devicePixelRatio", 1}, {"canvas_width", 0}, {"canvas_height", 0}, {"backgroundColor", "#fff"}};
    defaultCfg.merge_patch(optionsCfg);

    F2CanvasView *view = new F2CanvasView(defaultCfg);
    return reinterpret_cast<jlong>(view);
}

static jint OnSurfaceAvailable(JNIEnv *env, jclass clazz, jlong view, jobject surface) {
    F2_LOG_I("F2ChartView", "%s", "#OnSurfaceAvailable");
    if(surface != nullptr) {
        ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
        F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
        F2_LOG_I(chartView->GetCanvasId(), "%s", "#OnSurfaceAvailable");
        return chartView->OnSurfaceCreated(window);
    }
    return 0;
}

static jint InitCanvasContext(JNIEnv *env, jclass clazz, jlong view) {
    return reinterpret_cast<F2CanvasView *>(view)->InitCanvasContext();
}

static jboolean IsCanvasContextInitted(JNIEnv *env, jclass clazz, jlong view) {
    return reinterpret_cast<F2CanvasView *>(view)->IsInitCanvasContext();
}

static jint OnSurfaceSizeChanged(JNIEnv *env, jclass clazz, jlong view, jint width, jint height) {
    return reinterpret_cast<F2CanvasView *>(view)->OnSurfaceChanged(width, height);
}

static jint OnSurfaceDestroy(JNIEnv *env, jclass clazz, jlong view) {
    return reinterpret_cast<F2CanvasView *>(view)->OnSurfaceDestroyed();
}

static jint OnViewDestroy(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    delete chartView;
    return 0;
}

static jint PerformSwapBuffer(JNIEnv *env, jclass clazz, jlong view) {
    return reinterpret_cast<F2CanvasView *>(view)->PerformSwapFrame();
}

static ScopedJavaGlobalRef<jclass> *gNativeCanvasProxyClass = nullptr;
static const JNINativeMethod native_canvas_methods[] = {{
                                                            .name = "nCreateNativeChartView",
                                                            .signature = "(Ljava/lang/String;)J",
                                                            .fnPtr = reinterpret_cast<void *>(CreateNativeCanvasView),
                                                        },
                                                        {
                                                            .name = "nSurfaceAvailable",
                                                            .signature = "(JLandroid/view/Surface;)I",
                                                            .fnPtr = reinterpret_cast<void *>(OnSurfaceAvailable),
                                                        },
                                                        {
                                                            .name = "nInitCanvasContext",
                                                            .signature = "(J)I",
                                                            .fnPtr = reinterpret_cast<void *>(InitCanvasContext),
                                                        },
                                                        {
                                                            .name = "nIsCanvasContextInitted",
                                                            .signature = "(J)Z",
                                                            .fnPtr = reinterpret_cast<void *>(IsCanvasContextInitted),
                                                        },
                                                        {
                                                            .name = "nSurfaceSizeChanged",
                                                            .signature = "(JII)I",
                                                            .fnPtr = reinterpret_cast<void *>(OnSurfaceSizeChanged),
                                                        },
                                                        {
                                                            .name = "nSurfaceDestroyed",
                                                            .signature = "(J)I",
                                                            .fnPtr = reinterpret_cast<void *>(OnSurfaceDestroy),
                                                        },
                                                        {
                                                            .name = "nDestroyNativeChartView",
                                                            .signature = "(J)I",
                                                            .fnPtr = reinterpret_cast<void *>(OnViewDestroy),
                                                        },
                                                        {
                                                            .name = "nPerformSwapFrame",
                                                            .signature = "(J)I",
                                                            .fnPtr = reinterpret_cast<void *>(PerformSwapBuffer),
                                                        }};

static jlong CreateNativeChart(JNIEnv *env, jclass clazz, jstring name, jdouble width, jdouble height, jdouble ratio) {
    std::string _name = JavaStringToString(env, name);
    F2_LOG_I("#CreateNativeChart", "create NativeChart:%s", _name.data());
    xg::XChart *chart = new xg::XChart(_name, width, height, ratio);
    return reinterpret_cast<jlong>(chart);
}

static jint SetChartCanvas(JNIEnv *env, jclass clazz, jlong chart, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetCanvas");
    _chart->SetCanvasContext(chartView);
    return 0;
}

static jint SetChartSource(JNIEnv *env, jclass clazz, jlong chart, jstring source) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    std::string data = JavaStringToString(env, source);
    _chart->Source(std::move(data));
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartSource");
    return 0;
}

static jint SetChartPadding(JNIEnv *env, jclass clazz, jlong chart, jdouble left, jdouble top, jdouble right, jdouble bottom) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Padding(left, top, right, bottom);
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartPadding");
    return 0;
}

static jint SetChartMargin(JNIEnv *env, jclass clazz, jlong chart, jdouble left, jdouble top, jdouble right, jdouble bottom) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Margin(left, top, right, bottom);
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartMargin");
    return 0;
}

static jint SetChartScale(JNIEnv *env, jclass clazz, jlong chart, jstring field, jstring config) {
    std::string _field = JavaStringToString(env, field);
    std::string _config = JavaStringToString(env, config);

    nlohmann::json cfg = nlohmann::json::parse(_config, nullptr, false);
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Scale(std::move(_field), cfg);

    F2_LOG_I(_chart->GetChartName(), "%s", "#SetScaleConfig");
    return 0;
}

static jint SetChartAxis(JNIEnv *env, jclass clazz, jlong chart, jstring field, jstring config) {
    std::string _field = JavaStringToString(env, field);
    std::string _config = JavaStringToString(env, config);

    nlohmann::json cfg = nlohmann::json::parse(_config, nullptr, false);
    if(!cfg.is_object())
        return 0;
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Axis(std::move(_field), cfg);
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetAxisConfig");
    return 0;
}

static jint SetChartCoord(JNIEnv *env, jclass clazz, jlong chart, jstring config) {
    std::string _config = JavaStringToString(env, config);

    nlohmann::json cfg = nlohmann::json::parse(_config, nullptr, false);
    if(cfg.is_object()) {
        xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
        _chart->Coord(cfg);
        F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartCoord");
    }
    return 0;
}

static jint SetChartInteraction(JNIEnv *env, jclass clazz, jlong chart, jstring type) {
    std::string _type = JavaStringToString(env, type);
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Interaction(_type);
    return 0;
}

static jint SetChartToolTip(JNIEnv *env, jclass clazz, jlong chart, jstring config) {
    std::string _config = JavaStringToString(env, config);

    nlohmann::json cfg = nlohmann::json::parse(_config, nullptr, false);
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Tooltip(cfg);
    return 0;
}

static jint SetChartLegend(JNIEnv *env, jclass clazz, jlong chart, jstring field, jstring config) {
    std::string _field = JavaStringToString(env, field);
    std::string _config = JavaStringToString(env, config);

    nlohmann::json cfg = nlohmann::json::parse(_config, nullptr, false);
    if(!cfg.is_object())
        return 0;
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Legend(_field, cfg);
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetAxisConfig");
    return 0;
}

static jint SetChartGuideText(JNIEnv *env, jclass clazz, jlong chart, jstring config) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);

    std::string _config = JavaStringToString(env, config);

    nlohmann::json cfg = nlohmann::json::parse(_config, nullptr, false);
    if(!cfg.is_object())
        return 0;
    _chart->Guide().Text(cfg);
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartGuideText");
    return 0;
}

static jint SetChartGuideFlag(JNIEnv *env, jclass clazz, jlong chart, jstring config) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);

    std::string _config = JavaStringToString(env, config);

    nlohmann::json cfg = nlohmann::json::parse(_config, nullptr, false);
    if(!cfg.is_object())
        return 0;
    _chart->Guide().Flag(cfg);
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartGuideFlag");
    return 0;
}

static jint SendChartTouchEvent(JNIEnv *env, jclass clazz, jlong chart, jstring event) {
    std::string _event = JavaStringToString(env, event);

    nlohmann::json eventJson = nlohmann::json::parse(_event, nullptr, false);
    if(eventJson.is_object()) {
        xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
        _chart->OnTouchEvent(eventJson);
    }
    return 0;
}

static jlong CreateChartGeom(JNIEnv *env, jclass clazz, jlong chart, jstring type) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    std::string _type = JavaStringToString(env, type);
    xg::geom::AbstractGeom *geom = nullptr;
    if(_type == "line") {
        geom = &_chart->Line();
    } else if(_type == "area") {
        geom = &_chart->Area();
    } else if(_type == "interval") {
        geom = &_chart->Interval();
    }

    F2_LOG_I(_chart->GetChartName(), "createGeom: %s", _type.data());

    if(geom == nullptr) {
        return 0;
    }
    return reinterpret_cast<jlong>(geom);
}

static jstring GetChartRenderDumpInfo(JNIEnv *env, jclass clazz, jlong chart) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    F2_LOG_I(_chart->GetChartName(), "%s", "#:GetChartRenderDumpInfo");
    std::string dumpInfo = _chart->GetRenderInfo();
    return env->NewStringUTF(dumpInfo.data());
}

static jint DestroyChart(JNIEnv *env, jclass clazz, jlong chart) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    F2_LOG_I(_chart->GetChartName(), "%s", "#destroy");
    delete _chart;
    return 0;
}

static jint ChartRender(JNIEnv *env, jclass clazz, jlong chart) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    F2_LOG_I(_chart->GetChartName(), "%s", "#render");
    _chart->Render();
    return 0;
}

static jint SetChartGeomPosition(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring fields) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _fields = JavaStringToString(env, fields);
    _geom->Position(_fields);
    return 0;
}

static jint SetChartGeomColors(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring field, jobjectArray colors) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _field = JavaStringToString(env, field);
    jsize colorsSize = env->GetArrayLength(colors);
    std::vector<std::string> _colors;
    for(int index = 0; index < colorsSize; ++index) {
        jstring color = (jstring)env->GetObjectArrayElement(colors, index);
        std::string _color = JavaStringToString(env, color);
        _colors.push_back(std::move(_color));
    }

    _geom->Color(_field, std::move(_colors));
    return 0;
}

static jint SetChartGeomColor(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring color) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _color = JavaStringToString(env, color);
    _geom->Color(_color);
    return 0;
}

static jint SetChartGeomSizes(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring field, jfloatArray sizes) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _field = JavaStringToString(env, field);
    std::vector<float> _sizes;
    jsize len = env->GetArrayLength(sizes);
    jfloat *array = env->GetFloatArrayElements(sizes, 0);
    for(int i = 0; i < len; ++i) {
        _sizes.push_back(array[i]);
    }
    _geom->Size(_field, std::move(_sizes));
    return 0;
}

static jint SetChartGeomSize(JNIEnv *env, jclass clazz, jlong geom, jstring type, jfloat size) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    _geom->Size(size);
    return 0;
}

static jint SetChartGeomIntervalTag(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring config) {
    std::string _type = JavaStringToString(env, type);
    if(_type != "interval") {
        return CODE_FAIL_UNKNOWN;
    }

    xg::geom::Interval *_interval = reinterpret_cast<xg::geom::Interval *>(geom);
    std::string _configStr = JavaStringToString(env, config);
    nlohmann::json cfg = nlohmann::json::parse(_configStr, nullptr, false);
    _interval->Tag(cfg);
    return 0;
}

static jint SetChartGeomIntervalStyle(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring config) {
    std::string _type = JavaStringToString(env, type);
    if(_type != "interval") {
        return CODE_FAIL_UNKNOWN;
    }

    xg::geom::Interval *_interval = reinterpret_cast<xg::geom::Interval *>(geom);
    std::string _configStr = JavaStringToString(env, config);
    nlohmann::json cfg = nlohmann::json::parse(_configStr, nullptr, false);
    _interval->Style(cfg);
    return 0;
}

static jint SetChartGeomShapes(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring field, jobjectArray shapes) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _field = JavaStringToString(env, field);
    jsize len = env->GetArrayLength(shapes);
    std::vector<std::string> _shapes;
    for(int index = 0; index < len; ++index) {
        jstring shape = (jstring)env->GetObjectArrayElement(shapes, index);
        std::string _shape = JavaStringToString(env, shape);
        _shapes.push_back(std::move(_shape));
    }

    _geom->Shape(_field, std::move(_shapes));
    return 0;
}

static jint SetChartGeomShape(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring shape) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _shape = JavaStringToString(env, shape);
    _geom->Shape(_shape);
    return 0;
}

static jint SetChartGeomAdjust(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring adjust) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _adjust = JavaStringToString(env, adjust);
    _geom->Adjust(_adjust);
    return 0;
}

static const JNINativeMethod native_chart_methods[] = {{
                                                           .name = "nCreateNativeChart",
                                                           .signature = "(Ljava/lang/String;DDD)J",
                                                           .fnPtr = reinterpret_cast<void *>(CreateNativeChart),
                                                       },
                                                       {
                                                           .name = "nSetCanvasView",
                                                           .signature = "(JJ)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartCanvas),
                                                       },
                                                       {
                                                           .name = "nSetSource",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartSource),
                                                       },
                                                       {
                                                           .name = "nSetPadding",
                                                           .signature = "(JDDDD)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartPadding),
                                                       },
                                                       {
                                                           .name = "nSetMargin",
                                                           .signature = "(JDDDD)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartMargin),
                                                       },
                                                       {
                                                           .name = "nSetScale",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartScale),
                                                       },
                                                       {
                                                           .name = "nSetAxis",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartAxis),
                                                       },
                                                       {
                                                           .name = "nSetCoord",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartCoord),
                                                       },
                                                       {
                                                           .name = "nSetInteraction",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartInteraction),
                                                       },
                                                       {
                                                           .name = "nSetToolTip",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartToolTip),
                                                       },
                                                       {
                                                           .name = "nSetLegend",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartLegend),
                                                       },
                                                       {
                                                           .name = "nSetGuideText",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGuideText),
                                                       },
                                                       {
                                                           .name = "nSetGuideFlag",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGuideFlag),
                                                       },
                                                       {
                                                           .name = "nSendTouchEvent",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SendChartTouchEvent),
                                                       },
                                                       {
                                                           .name = "nCreateGeom",
                                                           .signature = "(JLjava/lang/String;)J",
                                                           .fnPtr = reinterpret_cast<void *>(CreateChartGeom),
                                                       },
                                                       {
                                                           .name = "nGetRenderDumpInfo",
                                                           .signature = "(J)Ljava/lang/String;",
                                                           .fnPtr = reinterpret_cast<void *>(GetChartRenderDumpInfo),
                                                       },
                                                       {
                                                           .name = "nDestroy",
                                                           .signature = "(J)I",
                                                           .fnPtr = reinterpret_cast<void *>(DestroyChart),
                                                       },
                                                       {
                                                           .name = "nRender",
                                                           .signature = "(J)I",
                                                           .fnPtr = reinterpret_cast<void *>(ChartRender),
                                                       },
                                                       {
                                                           .name = "nGeomPosition",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomPosition),
                                                       },
                                                       {
                                                           .name = "nGeomColor",
                                                           .signature =
                                                               "(JLjava/lang/String;Ljava/lang/String;[Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomColors),
                                                       },
                                                       {
                                                           .name = "nGeomColor",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomColor),
                                                       },
                                                       {
                                                           .name = "nGeomSize",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;[F)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomSizes),
                                                       },
                                                       {
                                                           .name = "nGeomSize",
                                                           .signature = "(JLjava/lang/String;F)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomSize),
                                                       },
                                                       {
                                                           .name = "nGeomShape",
                                                           .signature =
                                                               "(JLjava/lang/String;Ljava/lang/String;[Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomShapes),
                                                       },
                                                       {
                                                           .name = "nGeomShape",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomShape),
                                                       },
                                                       {
                                                           .name = "nGeomAdjust",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomAdjust),
                                                       },
                                                       {
                                                           .name = "nGeomIntervalTag",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomIntervalTag),
                                                       },
                                                       {
                                                           .name = "nGeomIntervalStyle",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomIntervalStyle),
                                                       }};

static ScopedJavaGlobalRef<jclass> *gF2LogClass = nullptr;
static jmethodID gF2LogInfoMethod = nullptr;
static jmethodID gF2LogWarnMethod = nullptr;
static jmethodID gF2LogErrorMethod = nullptr;

static bool InitF2Log(JNIEnv *env) {
    if(gF2LogClass != nullptr) {
        return true;
    }

    jclass logClass = env->FindClass("com/antgroup/antv/f2/F2Log");

    if(logClass == nullptr) {
        return false;
    }

    gF2LogInfoMethod = env->GetStaticMethodID(logClass, "i", "(Ljava/lang/String;Ljava/lang/String;)V");
    gF2LogWarnMethod = env->GetStaticMethodID(logClass, "w", "(Ljava/lang/String;Ljava/lang/String;)V");
    gF2LogErrorMethod = env->GetStaticMethodID(logClass, "e", "(Ljava/lang/String;Ljava/lang/String;)V");
    if(HasException(env)) {
        return false;
    }

    gF2LogClass = new ScopedJavaGlobalRef<jclass>(env, logClass);

    return true;
}

static bool
RegisterJNIInterface(JNIEnv *env, ScopedJavaGlobalRef<jclass> **holder, const char *class_path, const JNINativeMethod *methods, int array_size) {
    jclass clazz = env->FindClass(class_path);
    (*holder) = new ScopedJavaGlobalRef<jclass>(env, clazz);
    if((*holder)->isNull()) {
        // LOGE("CAN'T FIND class %s", class_path);
        F2_LOG_E("#RegisterJNIInterface", "can't find class %s", class_path);
        return false;
    }
    if(env->RegisterNatives((*holder)->obj(), methods, array_size) != 0) {
        // LOGE("Register JNI methods FAILED!");
        F2_LOG_E("#RegisterJNIInterface", "%s", "Register JNI methods failed.");
        return false;
    }
    F2_LOG_I("#RegisterJNIInterface", "%s", "Register jni success");
    return true;
}

void InnerLog(int level, std::string traceId, const char *fmt, ...) {
    std::string _tag = "jni|" + traceId;

    char msg[2048] = {0};
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, 2048, fmt, args);
    va_end(args);

    jmethodID method = gF2LogInfoMethod;
    if(level == ANDROID_LOG_WARN) {
        method = gF2LogWarnMethod;
    } else if(level == ANDROID_LOG_ERROR) {
        method = gF2LogErrorMethod;
    }

    if(gF2LogClass == nullptr || gF2LogClass->isNull() || method == nullptr) {
        native_clog(level, _tag.data(), msg);
        return;
    }

    JNIEnv *env = GetJniEnvSafe();
    if(env == nullptr) {
        return;
    }
    ScopedJavaLocalRef<jstring> tagStr = StringToJavaString(env, _tag);
    ScopedJavaLocalRef<jstring> msgStr = StringToJavaString(env, msg);
    env->CallStaticVoidMethod(gF2LogClass->obj(), method, tagStr.obj(), msgStr.obj());
}

static bool OnJniLoad(JNIEnv *env) {
    if(!InitF2Log(env)) {
        return false;
    }

    F2_LOG_I("#OnJniLoad", "%s", "init log success");

    if(!RegisterJNIInterface(env, &gNativeCanvasProxyClass, "com/antgroup/antv/f2/NativeCanvasProxy", native_canvas_methods,
                             xg_jni_arraysize(native_canvas_methods))) {
        return false;
    }

    F2_LOG_I("#OnJniLoad", "%s", "register canvas view success");

    if(!RegisterJNIInterface(env, &gNativeCanvasProxyClass, "com/antgroup/antv/f2/NativeChartProxy", native_chart_methods,
                             xg_jni_arraysize(native_chart_methods))) {
        return false;
    }

    F2_LOG_I("#OnJniLoad", "%s", "register f2 chart success");
    return true;
}

} // namespace jni
} // namespace xg

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    F2_LOG_I("#JNI_OnLoad", "%s", "JNI_OnLoad Start...");
    JavaVM *old = xg::jni::GetJVM();
    if(old == vm) {
        F2_LOG_I("#JNI_OnLoad", "%s", "JNI_OnLoad dupliation vm, skip OnLoad");
        return JNI_VERSION_1_4;
    }

    xg::jni::InitJavaVM(vm);

    JNIEnv *env = xg::jni::AttachCurrentThread();
    F2_LOG_I("#JNI_OnLoad", "%s", "AttachCurrentThread FINISH");
    bool result = xg::jni::OnJniLoad(env);

    if(!result) {
        F2_LOG_E("#JNI_OnLoad", "%s", "JNI_OnLoad FAIL");
    }
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) { F2_LOG_I("#JNI_OnUnload", "%s", "JNI_OnUnload Finished"); }