//
// Created by Hogan Liu on 2020/7/25.
//

#ifndef ANTMATION_MAC_ANTWASMBINDING_H
#define ANTMATION_MAC_ANTWASMBINDING_H

#include "CanvasContext.h"
#include "PlatformContext.h"
#include "RuntimeBase.h"
#include "ant_wasm.h"

#define GET_CANVAS_CONTEXT(ctx) \
    AntMation::CanvasContext* ctx = (AntMation::CanvasContext*) ant_wasm_get_attachment(exec_env);

#define GET_PLATFORM_CONTEXT(ctx) \
    AntMation::PlatformContext* ctx = (AntMation::PlatformContext*) ant_wasm_get_attachment(exec_env);

#define GET_RUNTIME_CONTEXT(ctx) \
    AntMation::RuntimeBase* ctx = (AntMation::RuntimeBase*) ant_wasm_get_attachment(exec_env);

int copyString2WasmInstance(AliWasmModuleInstance module_inst, const char *str) {
    int size = (str == NULL ? 0 : strlen(str));

    char *retStr = NULL;
    int retOffset = ant_wasm_malloc(module_inst, size + 1, (void **) &retStr);
    if (retStr != NULL) {
        if (str != NULL) {
            strcpy(retStr, str);
        }
        retStr[size] = '\0';
        return retOffset;
    }

    return 0;
}

int copyString2Wasm(AliWasmExecEnv exec_env, const char *str) {
    AliWasmModuleInstance module_inst = ant_wasm_get_module_instance(exec_env);
    return copyString2WasmInstance(module_inst, str);
}

void _am_memFree(AliWasmExecEnv exec_env, int ptr) {
    AliWasmModuleInstance module_inst = ant_wasm_get_module_instance(exec_env);
    ant_wasm_free(module_inst, ptr);
}
int _am_memEnlarge(AliWasmExecEnv exec_env, int page) {
    AliWasmModuleInstance module_inst = ant_wasm_get_module_instance(exec_env);
    return ant_wasm_enlarge_memory(module_inst, page) ? 1 : 0;
}

// platform
void _am_logMsg(AliWasmExecEnv exec_env, const char *message) {
    GET_PLATFORM_CONTEXT(ctx)
    ctx->logMsg(message);
}

void _am_deleteObj(AliWasmExecEnv exec_env, int idx) {
    GET_PLATFORM_CONTEXT(ctx)
    ctx->destroyObject(idx);
}

float _am_pixelRatio(AliWasmExecEnv exec_env) {
    GET_PLATFORM_CONTEXT(ctx)
    return ctx->screenPixelRatio();
}

int _am_clientVersion(AliWasmExecEnv exec_env) {
    GET_PLATFORM_CONTEXT(ctx)
    const char *str = ctx->getClientVersion();
    return copyString2Wasm(exec_env, str);
}

int _am_getPlatform(AliWasmExecEnv exec_env) {
    GET_PLATFORM_CONTEXT(ctx)
    const char *str = ctx->getPlatform();
    return copyString2Wasm(exec_env, str);
}
int _am_getDeviceModel(AliWasmExecEnv exec_env) {
    GET_PLATFORM_CONTEXT(ctx)
    const char *str = ctx->getDeviceModel();
    return copyString2Wasm(exec_env, str);
}
int _am_getOsVersion(AliWasmExecEnv exec_env) {
    GET_PLATFORM_CONTEXT(ctx)
    const char *str = ctx->getOsVersion();
    return copyString2Wasm(exec_env, str);
}

float _am_screenPixelRatio(AliWasmExecEnv exec_env) {
    GET_PLATFORM_CONTEXT(ctx)
    return ctx->screenPixelRatio();
}
int _am_screenWidth(AliWasmExecEnv exec_env) {
    GET_PLATFORM_CONTEXT(ctx)
    return ctx->screenWidth();
}
int _am_screenHeight(AliWasmExecEnv exec_env) {
    GET_PLATFORM_CONTEXT(ctx)
    return ctx->screenHeight();
}

// file
int _am_getFileLength(AliWasmExecEnv exec_env, const char *path) {
    GET_PLATFORM_CONTEXT(ctx)
    return ctx->getFileLength(path);
}
int _am_readFile(AliWasmExecEnv exec_env, const char *path, char *buffer, int size) {
    GET_PLATFORM_CONTEXT(ctx)
    return ctx->readFile(path, buffer, size);
}

// image
void _image_create(AliWasmExecEnv exec_env, int idx, const char *path) {
    GET_PLATFORM_CONTEXT(ctx)
    ctx->createImage(idx, path);
}
void _image_release(AliWasmExecEnv exec_env, int idx) {
    GET_PLATFORM_CONTEXT(ctx)
    ctx->releaseImage(idx);
}
unsigned int _image_texture(AliWasmExecEnv exec_env, int idx) {
    GET_PLATFORM_CONTEXT(ctx)
    return ctx->getImageTexture(idx);
}
int _image_getWidth(AliWasmExecEnv exec_env, int idx) {
    GET_PLATFORM_CONTEXT(ctx)
    return ctx->getImageWidth(idx);
}
int _image_getHeight(AliWasmExecEnv exec_env, int idx) {
    GET_PLATFORM_CONTEXT(ctx)
    return ctx->getImageHeight(idx);
}

// platform
void _am_onInit(AliWasmExecEnv exec_env, int success, const char *msg) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->onEngineInit(success, msg);
}

void _am_preloadImages(AliWasmExecEnv exec_env, const char *config) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->preloadImages(config);
}

void _am_onRenderSizeChanged(AliWasmExecEnv exec_env, int width, int height) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->onRenderSizeChanged(width, height);
}

void _am_onRenderError(AliWasmExecEnv exec_env, const char *msg, int fatal) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->onRenderError(msg, fatal == 1);
}

void _am_setBackgroundImage(AliWasmExecEnv exec_env, const char *path) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->setBackgroundImage(path);
}

int _am_getRenderWidth(AliWasmExecEnv exec_env) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->getRenderWidth();
}

int _am_getRenderHeight(AliWasmExecEnv exec_env) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->getRenderHeight();
}

long _am_getTimestamp(AliWasmExecEnv exec_env) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->getTimestamp();
}
int _am_setTimeout(AliWasmExecEnv exec_env, int milliseconds) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->setTimeout(milliseconds);
}
void _am_clearTimeout(AliWasmExecEnv exec_env, int tid) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->clearTimeout(tid);
}
int _am_setInterval(AliWasmExecEnv exec_env, int milliseconds) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->setInterval(milliseconds);
}
void _am_clearInterval(AliWasmExecEnv exec_env, int tid) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->clearInterval(tid);
}

// dynamic property
int _am_getIntProp(AliWasmExecEnv exec_env, const char *propId, int defaultVal) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->getIntProp(propId, defaultVal);
}
float _am_getFloatProp(AliWasmExecEnv exec_env, const char *propId, float defaultVal) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->getFloatProp(propId, defaultVal);
}
int _am_getBooleanProp(AliWasmExecEnv exec_env, const char *propId, int defaultVal) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->getBooleanProp(propId, 1 == defaultVal);
}
int _am_getStringProp(AliWasmExecEnv exec_env, const char *propId, const char *defaultVal) {
    GET_RUNTIME_CONTEXT(ctx)
    const char *str = ctx->getStringProp(propId, defaultVal);
    return copyString2Wasm(exec_env, str);
}
int _am_getConfigValue(AliWasmExecEnv exec_env, const char *key) {
    GET_RUNTIME_CONTEXT(ctx)
    std::string str = ctx->getConfigValue(key);
    return copyString2Wasm(exec_env, str.empty() ? "" : str.c_str());
}

// batch render
int _batch_create(AliWasmExecEnv exec_env) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->_batch_create() ? 1 : 0;
}

void _batch_draw(AliWasmExecEnv exec_env, int vertices, int vertexCnt, int state) {
    GET_RUNTIME_CONTEXT(ctx)
    AliWasmModuleInstance module_inst = ant_wasm_get_module_instance(exec_env);
    auto *verticesAddr = (AntMation::OpenGLVertex *) ant_wasm_addr_app_to_native(module_inst, vertices);
    auto *stateAddr = (AntMation::OpenGLRenderState *) ant_wasm_addr_app_to_native(module_inst, state);
    ctx->_batch_draw(verticesAddr, vertexCnt, stateAddr);
}

void _batch_destroy(AliWasmExecEnv exec_env) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->_batch_destroy();
}

void _ant3d_addPath(AliWasmExecEnv exec_env, const char *resourcesPath) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->_ant3d_addPath(resourcesPath);
}

int _ant3d_create(AliWasmExecEnv exec_env, const char *scenePath, float fov) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->_ant3d_create(scenePath, fov);
}

int _ant3d_draw(AliWasmExecEnv exec_env) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->_ant3d_draw() ? 1 : 0;
}

void _ant3d_destroy(AliWasmExecEnv exec_env) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->_ant3d_destroy();
}

void _ant3d_animPlay(AliWasmExecEnv exec_env, const char *animationName, int repeat, float speed, float blendWeight) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().animPlay(animationName, repeat, speed, blendWeight);
}
void _ant3d_animPause(AliWasmExecEnv exec_env, const char *animationName) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().animPause(animationName);
}
void _ant3d_animStop(AliWasmExecEnv exec_env, const char *animationName) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().animStop(animationName);
}

void _ant3d_rotate(AliWasmExecEnv exec_env, float x, float y, float z) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().rotate(x, y, z);
}
void _ant3d_translate(AliWasmExecEnv exec_env, float x, float y, float z) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().translate(x, y, z);
}
void _ant3d_scale(AliWasmExecEnv exec_env, float x, float y, float z) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().scale(x, y, z);
}

void _ant3d_rotateNode(AliWasmExecEnv exec_env, const char *nodeId, float x, float y, float z, int worldSpace) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().rotateNode(nodeId, x, y, z, 1 == worldSpace);
}
void _ant3d_translateNode(AliWasmExecEnv exec_env, const char *nodeId, float x, float y, float z, int worldSpace) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().translateNode(nodeId, x, y, z, 1 == worldSpace);
}
void _ant3d_scaleNode(AliWasmExecEnv exec_env, const char *nodeId, float x, float y, float z, int worldSpace) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().scaleNode(nodeId, x, y, z, 1 == worldSpace);
}

static int copyVec2Wasm(AliWasmExecEnv exec_env, AntMation::Vec3 &ret) {
    AliWasmModuleInstance module_inst = ant_wasm_get_module_instance(exec_env);
    AntMation::Vec3 *retPtr = nullptr;
    int retOffset = ant_wasm_malloc(module_inst, sizeof(AntMation::Vec3), (void **) &retPtr);
    if (retPtr != nullptr) {
        retPtr->x = ret.x;
        retPtr->y = ret.y;
        retPtr->z = ret.z;
    }
    return retOffset;
}

int _ant3d_getPosition(AliWasmExecEnv exec_env, const char *nodeId, int worldSpace) {
    GET_RUNTIME_CONTEXT(ctx)
    AntMation::Vec3 ret = ctx->getAnt3DRender().getPosition(nodeId, 1 == worldSpace);
    return copyVec2Wasm(exec_env, ret);
}
int _ant3d_getRotation(AliWasmExecEnv exec_env, const char *nodeId, int worldSpace) {
    GET_RUNTIME_CONTEXT(ctx)
    AntMation::Vec3 ret = ctx->getAnt3DRender().getRotation(nodeId, 1 == worldSpace);
    return copyVec2Wasm(exec_env, ret);
}
int _ant3d_getScale(AliWasmExecEnv exec_env, const char *nodeId, int worldSpace) {
    GET_RUNTIME_CONTEXT(ctx)
    AntMation::Vec3 ret = ctx->getAnt3DRender().getScale(nodeId, 1 == worldSpace);
    return copyVec2Wasm(exec_env, ret);
}

int _ant3d_isVisible(AliWasmExecEnv exec_env, const char *nodeId) {
    GET_RUNTIME_CONTEXT(ctx)
    return ctx->getAnt3DRender().isVisible(nodeId) ? 1 : 0;
}
void _ant3d_setVisible(AliWasmExecEnv exec_env, const char *nodeId, int isVisible) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().setVisible(nodeId, 1 == isVisible);
}

void _ant3d_setStatic(AliWasmExecEnv exec_env, const char *nodeId, int active) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().setStatic(nodeId, 1 == active);
}
void _ant3d_setBillBoard(AliWasmExecEnv exec_env, const char *nodeId, int active) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().setBillBoard(nodeId, 1 == active);
}
void _ant3d_setFixed(AliWasmExecEnv exec_env, const char *nodeId, int active) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().setFixed(nodeId, 1 == active);
}

void _ant3d_setTexture(AliWasmExecEnv exec_env, const char *nodeId, const char *imgPath, int meshPart) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().setTexture(nodeId, imgPath, meshPart);
}
void _ant3d_removeTexture(AliWasmExecEnv exec_env, const char *imgPath) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().removeTexture(imgPath);
}

void _ant3d_particleEmit(AliWasmExecEnv exec_env, const char *nodeId, int active) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().particleEmit(nodeId, 1 == active);
}
void _ant3d_setAmbientColor(AliWasmExecEnv exec_env, float r, float g, float b) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().setAmbientColor(r, g, b);
}
void _ant3d_setLightColor(AliWasmExecEnv exec_env, const char *nodeId, float r, float g, float b) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().setLightColor(nodeId, r, g, b);
}
void _ant3d_modulateAlpha(AliWasmExecEnv exec_env, const char *nodeId, float alpha, int meshPart) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().modulateAlpha(nodeId, alpha, meshPart);
}
void _ant3d_modulateColor(AliWasmExecEnv exec_env, const char *nodeId, float r, float g, float b, float a, int meshPart) {
    GET_RUNTIME_CONTEXT(ctx)
    ctx->getAnt3DRender().modulateColor(nodeId, r, g, b, a, meshPart);
}

// HTMLCanvasElement
int _canvas_getWidth(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return ctx->getCanvasWidth();
}
int _canvas_getHeight(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return ctx->getCanvasHeight();
}

// TextMetrics

// CanvasRenderingContext2D

// Drawing rectangles
void _2d_clearRect(AliWasmExecEnv exec_env, float x, float y, float w, float h) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_clearRect(x, y, w, h);
}
void _2d_fillRect(AliWasmExecEnv exec_env, float x, float y, float w, float h) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_fillRect(x, y, w, h);
}
void _2d_strokeRect(AliWasmExecEnv exec_env, float x, float y, float w, float h) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_strokeRect(x, y, w, h);
}

// Drawing text
void _2d_fillText(AliWasmExecEnv exec_env, const char *text, float x, float y, float maxWidth) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_fillText(text, x, y, maxWidth);
}
void _2d_strokeText(AliWasmExecEnv exec_env, const char *text, float x, float y, float maxWidth) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_strokeText(text, x, y, maxWidth);
}
float _2d_measureText(AliWasmExecEnv exec_env, const char *text) {
    GET_CANVAS_CONTEXT(ctx)
    return ctx->_2d_measureText(text);
}

// Line styles
void _2d_setLineWidth(AliWasmExecEnv exec_env, float v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setLineWidth(v);
}
float _2d_getLineWidth(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return ctx->_2d_getLineWidth();
}
void _2d_setLineCap(AliWasmExecEnv exec_env, const char *v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setLineCap(v);
}
int _2d_getLineCap(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return copyString2Wasm(exec_env, ctx->_2d_getLineCap());
}
void _2d_setLineJoin(AliWasmExecEnv exec_env, const char *v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setLineJoin(v);
}
int _2d_getLineJoin(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return copyString2Wasm(exec_env, ctx->_2d_getLineJoin());
}
void _2d_setMiterLimit(AliWasmExecEnv exec_env, float v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setMiterLimit(v);
}
float _2d_getMiterLimit(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return ctx->_2d_getMiterLimit();
}
void _2d_setLineDashOffset(AliWasmExecEnv exec_env, float v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setLineDashOffset(v);
}
float _2d_getLineDashOffset(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return ctx->_2d_getLineDashOffset();
}

// Text styles
int _2d_loadFont(AliWasmExecEnv exec_env, const char *path, const char *name) {
    GET_CANVAS_CONTEXT(ctx)
    if (ctx->_cb == NULL) {
        return 0;
    }
    std::string fullPath = ctx->_cb->getFullPath(path);
    if (fullPath.empty()) {
        return 0;
    }
    LOG_INFO("load font: %s", fullPath.c_str());
    return ctx->_2d_loadFont(fullPath.c_str(), name) ? 1 : 0;
}
void _2d_setFont(AliWasmExecEnv exec_env, const char *v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setFont(v);
}
int _2d_getFont(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return copyString2Wasm(exec_env, ctx->_2d_getFont());
}
void _2d_setTextAlign(AliWasmExecEnv exec_env, const char *v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setTextAlign(v);
}
int _2d_getTextAlign(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return copyString2Wasm(exec_env, ctx->_2d_getTextAlign());
}
void _2d_setTextBaseline(AliWasmExecEnv exec_env, const char *v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setTextBaseline(v);
}
int _2d_getTextBaseline(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return copyString2Wasm(exec_env, ctx->_2d_getTextBaseline());
}

// Fill and stroke styles
void _2d_setStrokeStyle_color(AliWasmExecEnv exec_env, const char *color) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setStrokeStyle_color(color);
}
int _2d_getStrokeStyle_color(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return copyString2Wasm(exec_env, ctx->_2d_getStrokeStyle_color());
}
void _2d_setFillStyle_color(AliWasmExecEnv exec_env, const char *color) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setFillStyle_color(color);
}
int _2d_getFillStyle_color(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return copyString2Wasm(exec_env, ctx->_2d_getFillStyle_color());
}

void _2d_setStrokeStyle_obj(AliWasmExecEnv exec_env, int idx) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setStrokeStyle_obj(idx);
}
void _2d_setFillStyle_obj(AliWasmExecEnv exec_env, int idx) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setFillStyle_obj(idx);
}

// Gradients and patterns
void _2d_createLinearGradient(AliWasmExecEnv exec_env, int idx, float x0, float y0, float x1, float y1) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_createLinearGradient(idx, x0, y0, x1, y1);
}
void _2d_createRadialGradient(AliWasmExecEnv exec_env, int idx, float x0, float y0, float r0, float x1, float y1, float r1) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_createRadialGradient(idx, x0, y0, r0, x1, y1, r1);
}
void _2d_createPattern(AliWasmExecEnv exec_env, int idx, int imgIdx, const char *repetition) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_createPattern(idx, imgIdx, repetition);
}

void _gradient_addColorStop(AliWasmExecEnv exec_env, int idx, float offset, const char *color) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_gradient_addColorStop(idx, offset, color);
}
void _pattern_setTransform(AliWasmExecEnv exec_env, int idx, float a, float b, float c, float d, float e, float f) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_pattern_setTransform(idx, a, b, c, d, e, f);
}

// Shadows
void _2d_setShadowOffsetX(AliWasmExecEnv exec_env, float v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setShadowOffsetX(v);
}
float _2d_getShadowOffsetX(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return ctx->_2d_getShadowOffsetX();
}
void _2d_setShadowOffsetY(AliWasmExecEnv exec_env, float v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setShadowOffsetY(v);
}
float _2d_getShadowOffsetY(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return ctx->_2d_getShadowOffsetY();
}
void _2d_setShadowBlur(AliWasmExecEnv exec_env, float v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setShadowBlur(v);
}
float _2d_getShadowBlur(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return ctx->_2d_getShadowBlur();
}
void _2d_setShadowColor(AliWasmExecEnv exec_env, const char *v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setShadowColor(v);
}
int _2d_getShadowColor(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return copyString2Wasm(exec_env, ctx->_2d_getShadowColor());
}

// Paths
void _2d_beginPath(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_beginPath();
}
void _2d_closePath(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_closePath();
}
void _2d_moveTo(AliWasmExecEnv exec_env, float x, float y) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_moveTo(x, y);
}
void _2d_lineTo(AliWasmExecEnv exec_env, float x, float y) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_lineTo(x, y);
}
void _2d_quadraticCurveTo(AliWasmExecEnv exec_env, float cpx, float cpy, float x, float y) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_quadraticCurveTo(cpx, cpy, x, y);
}
void _2d_bezierCurveTo(AliWasmExecEnv exec_env, float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
}
void _2d_arcTo(AliWasmExecEnv exec_env, float x1, float y1, float x2, float y2, float radius) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_arcTo(x1, y1, x2, y2, radius);
}
void _2d_rect(AliWasmExecEnv exec_env, float x, float y, float w, float h) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_rect(x, y, w, h);
}
void _2d_arc(AliWasmExecEnv exec_env, float x, float y, float radius, float startAngle, float endAngle, int anticlockwise) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_arc(x, y, radius, startAngle, endAngle, anticlockwise);
}

// Drawing paths
void _2d_fill(AliWasmExecEnv exec_env, const char *fillRule) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_fill(fillRule);
}
void _2d_stroke(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_stroke();
}
void _2d_clip(AliWasmExecEnv exec_env, const char *fillRule) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_clip(fillRule);
}

// Transformations
void _2d_scale(AliWasmExecEnv exec_env, float x, float y) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_scale(x, y);
}
void _2d_rotate(AliWasmExecEnv exec_env, float angle) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_rotate(angle);
}
void _2d_translate(AliWasmExecEnv exec_env, float x, float y) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_translate(x, y);
}
void _2d_transform(AliWasmExecEnv exec_env, float a, float b, float c, float d, float e, float f) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_transform(a, b, c, d, e, f);
}
void _2d_setTransform(AliWasmExecEnv exec_env, float a, float b, float c, float d, float e, float f) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setTransform(a, b, c, d, e, f);
}
void _2d_resetTransform(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_resetTransform();
}

// Compositing & Image smoothing
void _2d_setGlobalAlpha(AliWasmExecEnv exec_env, float v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setGlobalAlpha(v);
}
float _2d_getGlobalAlpha(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return ctx->_2d_getGlobalAlpha();
}
void _2d_setGlobalCompositeOperation(AliWasmExecEnv exec_env, const char *v) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_setGlobalCompositeOperation(v);
}
int _2d_getGlobalCompositeOperation(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    return copyString2Wasm(exec_env, ctx->_2d_getGlobalCompositeOperation());
}

// Drawing images
void _2d_drawImage_2(AliWasmExecEnv exec_env, int idx, float dx, float dy) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_drawImage_2(idx, dx, dy);
}
void _2d_drawImage_4(AliWasmExecEnv exec_env, int idx, float dx, float dy, float dw, float dh) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_drawImage_4(idx, dx, dy, dw, dh);
}
void _2d_drawImage_8(AliWasmExecEnv exec_env, int idx, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_drawImage_8(idx, sx, sy, sw, sh, dx, dy, dw, dh);
}

// The canvas state
void _2d_save(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_save();
}
void _2d_restore(AliWasmExecEnv exec_env) {
    GET_CANVAS_CONTEXT(ctx)
    ctx->_2d_restore();
}

#endif //ANTMATION_MAC_ANTWASMBINDING_H
