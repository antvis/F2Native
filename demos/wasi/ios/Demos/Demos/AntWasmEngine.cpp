//
// Created by Hogan Liu on 2020/7/26.
//

#include "engine/AntWasmEngine.h"
#include "Logger.h"
#include "AntWasmBinding.h"
#include "AssetProvider.h"


namespace AntMation {

#define EXPORT_ANT_WASM_API_WITH_ATT(symbol, signature, attachment) \
                            {#symbol, (void*)symbol, signature, attachment}


bool AntWasmEngine::create(int stackSize, int heapSize, int memEnlarge) {
    LOG_INFO("AntWasmEngine create");

    if (stackSize > 0) {
        wasmStackSize = stackSize;
    }
    if (heapSize > 0) {
        wasmHeapSize = heapSize;
    }
    if (memEnlarge > 0) {
        memoryEnlarge = memEnlarge;
    }
    if (ant_wasm_init() == ANT_WASM_OK) {
        AliWasmModuleConfig config;
        config.bizCode = "AntMation";
        wa_module = ant_wasm_create_module(config);
        return true;
    }

    return false;
}

void AntWasmEngine::destroy() {
    LOG_INFO("AntWasmEngine destroy");

    if (wa_module_inst != nullptr) {
        ant_wasm_destroy_module_instance(wa_module_inst);
    }

    if (wa_module != nullptr) {
        ant_wasm_unregister_api(wa_module);
        ant_wasm_destroy_module(wa_module);
    }

    AM_SAFE_DELETE_ARRAY(wasmData)
    AM_SAFE_DELETE(_symbolsRuntime)
    AM_SAFE_DELETE(_symbolsPlatform)
    AM_SAFE_DELETE(_symbolsCanvas)
}

bool AntWasmEngine::initCanvasBindings(CanvasContext *ctx) {
    if (wa_module == nullptr) {
        return false;
    }

    AliWasmApi native_symbols[] = {
            // Canvas
            EXPORT_ANT_WASM_API_WITH_ATT(_canvas_getWidth, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_canvas_getHeight, "()i", ctx),

            // Drawing rectangles
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_clearRect, "(ffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_fillRect, "(ffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_strokeRect, "(ffff)", ctx),

            // Drawing text
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_fillText, "($fff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_strokeText, "($fff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_measureText, "($)f", ctx),

            // Line styles
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setLineWidth, "(f)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getLineWidth, "()f", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setLineCap, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getLineCap, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setLineJoin, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getLineJoin, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setMiterLimit, "(f)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getMiterLimit, "()f", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setLineDashOffset, "(f)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getLineDashOffset, "()f", ctx),

            // Text styles
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_loadFont, "($$)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setFont, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getFont, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setTextAlign, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getTextAlign, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setTextBaseline, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getTextBaseline, "()i", ctx),

            // Fill and stroke styles
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setStrokeStyle_color, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getStrokeStyle_color, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setFillStyle_color, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getFillStyle_color, "()i", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setStrokeStyle_obj, "(i)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setFillStyle_obj, "(i)", ctx),

            // Gradients and patterns
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_createLinearGradient, "(iffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_createRadialGradient, "(iffffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_createPattern, "(ii$)", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_gradient_addColorStop, "(if$)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_pattern_setTransform, "(iffffff)", ctx),

            // Shadows
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setShadowOffsetX, "(f)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getShadowOffsetX, "()f", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setShadowOffsetY, "(f)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getShadowOffsetY, "()f", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setShadowBlur, "(f)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getShadowBlur, "()f", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setShadowColor, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getShadowColor, "()i", ctx),

            // Paths
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_beginPath, "()", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_closePath, "()", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_moveTo, "(ff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_lineTo, "(ff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_quadraticCurveTo, "(ffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_bezierCurveTo, "(ffffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_arcTo, "(fffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_rect, "(ffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_arc, "(fffffi)", ctx),

            // Drawing paths
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_fill, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_stroke, "()", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_clip, "($)", ctx),

            // Transformations
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_scale, "(ff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_rotate, "(f)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_translate, "(ff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_transform, "(ffffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setTransform, "(ffffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_resetTransform, "()", ctx),

            // Compositing & Image smoothing
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setGlobalAlpha, "(f)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getGlobalAlpha, "()f", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_setGlobalCompositeOperation, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_getGlobalCompositeOperation, "()i", ctx),

            // Drawing images
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_drawImage_2, "(iff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_drawImage_4, "(iffff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_drawImage_8, "(iffffffff)", ctx),

            // The canvas state
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_save, "()", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_2d_restore, "()", ctx),
    };
    int n_native_symbols = sizeof(native_symbols) / sizeof(AliWasmApi);
    _symbolsCanvas = new AliWasmApi[n_native_symbols];
    memcpy(_symbolsCanvas, native_symbols, sizeof(native_symbols));
    return ant_wasm_register_api(wa_module, _symbolsCanvas, n_native_symbols);
}
bool AntWasmEngine::initPlatformBindings(PlatformContext *ctx) {
    if (wa_module == nullptr) {
        return false;
    }

    AliWasmApi native_symbols[] = {
            // basic
            EXPORT_ANT_WASM_API_WITH_ATT(_am_logMsg, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_deleteObj, "(i)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_pixelRatio, "()f", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_clientVersion, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getPlatform, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getDeviceModel, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getOsVersion, "()i", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_am_screenPixelRatio, "()f", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_screenWidth, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_screenHeight, "()i", ctx),

            // file
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getFileLength, "($)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_readFile, "($*~)i", ctx),

            // image
            EXPORT_ANT_WASM_API_WITH_ATT(_image_create, "(i$)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_image_release, "(i)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_image_texture, "(i)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_image_getWidth, "(i)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_image_getHeight, "(i)i", ctx),
    };
    int n_native_symbols = sizeof(native_symbols) / sizeof(AliWasmApi);
    _symbolsPlatform = new AliWasmApi[n_native_symbols];
    memcpy(_symbolsPlatform, native_symbols, sizeof(native_symbols));
    return ant_wasm_register_api(wa_module, _symbolsPlatform, n_native_symbols);
}
bool AntWasmEngine::initRuntimeBindings(RuntimeBase *ctx) {
    if (wa_module == nullptr) {
        return false;
    }

    AliWasmApi native_symbols[] = {
            EXPORT_ANT_WASM_API_WITH_ATT(_am_memFree, "(i)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_memEnlarge, "(i)i", ctx),

            // basic
            EXPORT_ANT_WASM_API_WITH_ATT(_am_onInit, "(i$)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_preloadImages, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_onRenderSizeChanged, "(ii)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_setBackgroundImage, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getRenderWidth, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getRenderHeight, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_onRenderError, "($i)", ctx),

            // timer
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getTimestamp, "()I", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_setTimeout, "(i)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_clearTimeout, "(i)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_setInterval, "(i)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_clearInterval, "(i)", ctx),

            // property
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getIntProp, "($i)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getFloatProp, "($f)f", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getBooleanProp, "($i)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getStringProp, "($$)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_am_getConfigValue, "($)i", ctx),

            // batch render
            EXPORT_ANT_WASM_API_WITH_ATT(_batch_create, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_batch_draw, "(iii)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_batch_destroy, "()", ctx),

            // ant3d render
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_addPath, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_create, "($f)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_draw, "()i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_destroy, "()", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_animPlay, "($iff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_animPause, "($)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_animStop, "($)", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_rotate, "(fff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_translate, "(fff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_scale, "(fff)", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_rotateNode, "($fffi)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_translateNode, "($fffi)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_scaleNode, "($fffi)", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_getPosition, "($i)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_getRotation, "($i)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_getScale, "($i)i", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_isVisible, "($)i", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_setVisible, "($i)", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_setStatic, "($i)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_setBillBoard, "($i)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_setFixed, "($i)", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_setTexture, "($$i)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_removeTexture, "($)", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_particleEmit, "($i)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_setAmbientColor, "(fff)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_setLightColor, "($fff)", ctx),

            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_modulateAlpha, "($fi)", ctx),
            EXPORT_ANT_WASM_API_WITH_ATT(_ant3d_modulateColor, "($ffffi)", ctx),
    };
    int n_native_symbols = sizeof(native_symbols) / sizeof(AliWasmApi);
    _symbolsRuntime = new AliWasmApi[n_native_symbols];
    memcpy(_symbolsRuntime, native_symbols, sizeof(native_symbols));
    return ant_wasm_register_api(wa_module, _symbolsRuntime, n_native_symbols);
}

bool AntWasmEngine::loadFromFile(const char* path, const char *checkSum) {
    bool readOk = _asset->readFile(path, &wasmData, &wasmSize);
    if (readOk && wasmData != nullptr) {
        if (checkSum != nullptr) {
            if (!Utils::checkMD5(wasmData, wasmSize, checkSum)) {
                _error = "loadFromFile MD5 check failed";
                LOG_ERROR("%s", _error.c_str());
                return false;
            }
        } else {
            LOG_INFO("loadFromFile skip MD5 check");
        }

        return initWamr();
    }

    _error = "_asset->readFile failed";
    LOG_ERROR("loadFromFile failed: %s, %s", _error.c_str(), path);
    return false;
}
bool AntWasmEngine::loadFromBytes(const char* bytes, int len, const char *checkSum) {
    wasmSize = len;
    wasmData = new char[wasmSize];
    memcpy(wasmData, bytes, wasmSize * sizeof(char));

    if (checkSum != nullptr) {
        if (!Utils::checkMD5(wasmData, wasmSize, checkSum)) {
            _error = "loadFromBytes MD5 check failed";
            LOG_ERROR("%s", _error.c_str());
            return false;
        }
    } else {
        LOG_INFO("loadFromBytes skip MD5 check");
    }

    return initWamr();
}

bool AntWasmEngine::callInit() {
    if (wa_module_inst == nullptr) {
        return false;
    }

    int success = false;
    char *error = nullptr;
    int wa_args[1];
    int result = 0;

    AliWasmFunction func = ant_wasm_lookup_function(wa_module_inst, "_start", "()");
    if (func != nullptr) {
        result = ant_wasm_execute(wa_module_inst, "_start", "()", wa_args, 0, &success, &error);
        _error = (error ? error : "");
        LOG_INFO("call _start: %d, %d, %s", success, result, error);
    }

    result = ant_wasm_execute(wa_module_inst, "_am_init", "()", wa_args, 0, &success, &error);
    _error = (error ? error : "");
    LOG_INFO("call _am_init: %d, %d, %s", success, result, error);

    return success;
}
void AntWasmEngine::callResize(int w, int h) {
    if (wa_module_inst == nullptr) {
        return;
    }

    int success = false;
    char *error = nullptr;
    int wa_args[2];
    wa_args[0] = w;
    wa_args[1] = h;
    int result = 0;
    AliWasmFunction func = ant_wasm_lookup_function(wa_module_inst, "_am_resize", "(ii)");
    if (func != nullptr) {
        result = ant_wasm_execute(wa_module_inst, "_am_resize", "(ii)", wa_args, 2, &success, &error);
        if (!success) {
            LOG_INFO("call _am_resize: %d, %d, %s", success, result, error);
        }
    }
}
bool AntWasmEngine::callDrawFrame(int idx) {
    if (wa_module_inst == nullptr) {
        _error = "wa_module_inst == NULL";
        return false;
    }

    int success = false;
    char *error = nullptr;
    int wa_args[1];
    wa_args[0] = idx;
    int result = ant_wasm_execute(wa_module_inst, "_am_drawFrame", "(i)", wa_args, 1, &success, &error);
    if (!success) {
        LOG_INFO("call _am_drawFrame: %d, %d, %s", success, result, error);
        _error = (error ? error : "");
        return false;
    }

    return true;
}
void AntWasmEngine::callUpdateParams() {
    if (wa_module_inst == nullptr) {
        return;
    }

    int success = false;
    char *error = nullptr;
    int wa_args[1];
    int result = 0;
    AliWasmFunction func = ant_wasm_lookup_function(wa_module_inst, "_am_updateParams", "()");
    if (func != nullptr) {
        result = ant_wasm_execute(wa_module_inst, "_am_updateParams", "()", wa_args, 0, &success, &error);
        if (!success) {
            LOG_INFO("call _am_updateParams: %d, %d, %s", success, result, error);
        }
    }
}
void AntWasmEngine::callUnInit() {
    if (wa_module_inst == nullptr) {
        return;
    }

    int success = false;
    char *error = nullptr;
    int wa_args[1];
    int result = 0;
    AliWasmFunction func = ant_wasm_lookup_function(wa_module_inst, "_am_unInit", "()");
    if (func != nullptr) {
        result = ant_wasm_execute(wa_module_inst, "_am_unInit", "()", wa_args, 0, &success, &error);
        LOG_INFO("call _am_unInit: %d, %d, %s", success, result, error);
    }
}

void AntWasmEngine::callOnCreateImage(int idx, bool createSuccess) {
    if (wa_module_inst == nullptr) {
        return;
    }

    int success = false;
    char *error = nullptr;
    int wa_args[2];
    wa_args[0] = idx;
    wa_args[1] = createSuccess ? 1 : 0;
    int result = 0;
    AliWasmFunction func = ant_wasm_lookup_function(wa_module_inst, "_am_onCreateImage", "(ii)");
    if (func != nullptr) {
        result = ant_wasm_execute(wa_module_inst, "_am_onCreateImage", "(ii)", wa_args, 2, &success, &error);
        LOG_INFO("call _am_onCreateImage: %d, %d, %s", success, result, error);
    }
}
void AntWasmEngine::callOnPreloadImages(bool preloadSuccess) {
    if (wa_module_inst == nullptr) {
        return;
    }

    int success = false;
    char *error = nullptr;
    int wa_args[1];
    wa_args[0] = preloadSuccess ? 1 : 0;
    int result = 0;
    AliWasmFunction func = ant_wasm_lookup_function(wa_module_inst, "_am_onPreloadImages", "(i)");
    if (func != nullptr) {
        result = ant_wasm_execute(wa_module_inst, "_am_onPreloadImages", "(i)", wa_args, 1, &success, &error);
        LOG_INFO("call _am_onPreloadImages: %d, %d, %s", success, result, error);
    }
}

void AntWasmEngine::callOnEvent(const char *name, const char*extra) {
    if (wa_module_inst == nullptr) {
        return;
    }

    int success = false;
    char *error = nullptr;
    int wa_args[2];
    wa_args[0] = copyString2WasmInstance(wa_module_inst, name);
    wa_args[1] = copyString2WasmInstance(wa_module_inst, extra);
    int result = 0;
    AliWasmFunction func = ant_wasm_lookup_function(wa_module_inst, "_am_onEvent", "($$)");
    if (func != nullptr) {
        result = ant_wasm_execute(wa_module_inst, "_am_onEvent", "($$)", wa_args, 2, &success, &error);
        LOG_INFO("call _am_onEvent: %d, %d, %s", success, result, error);
    }
}


void AntWasmEngine::callOnTimerCallback(int tid) {
    if (wa_module_inst == nullptr) {
        return;
    }

    int success = false;
    char *error = nullptr;
    int wa_args[1];
    wa_args[0] = tid;
    int result = 0;
    AliWasmFunction func = ant_wasm_lookup_function(wa_module_inst, "_am_onTimer", "(i)");
    if (func != nullptr) {
        result = ant_wasm_execute(wa_module_inst, "_am_onTimer", "(i)", wa_args, 1, &success, &error);
        LOG_INFO("call _am_onTimer: %d, %d, %s", success, result, error);
    }
}

bool AntWasmEngine::initWamr() {
    char *error;
    if ((ant_wasm_module_load(wa_module, wasmData, wasmSize, &error)) == ANT_WASM_ERROR) {
        _error = (error ? error : "");
        LOG_ERROR("load wasm error: %s", error);
        free(error);
        return false;
    }

    AliWasmModuleInstanceConfig instanceConfig;
    instanceConfig.stackSize = wasmStackSize;
    instanceConfig.heapSize = wasmHeapSize;

    if ((wa_module_inst = ant_wasm_create_module_instance(wa_module, instanceConfig, &error)) == ANT_WASM_ERROR) {
        _error = (error ? error : "");
        LOG_ERROR("instantiate wasm error: %s", error);
        free(error);
        return false;
    }

    if (memoryEnlarge > 0) {
        bool enlargeOk = ant_wasm_enlarge_memory(wa_module_inst, memoryEnlarge);
        if (!enlargeOk) {
            LOG_ERROR("enlarge wasm memory failed, %d", memoryEnlarge);
        }
    }

    LOG_INFO("initWamr success");
    return true;
}
    
}
