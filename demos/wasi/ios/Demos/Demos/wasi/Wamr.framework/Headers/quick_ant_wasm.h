//
// Created by tianyang on 2020/8/27.
//
#include "quickjs.h"

#ifndef ANDROID_QUICK_ANT_WASM_H
#define ANDROID_QUICK_ANT_WASM_H

JSValue registerWasmInsideFunc(JSRuntime *runtime,JSContext *pCtx,uint8_t *wasmFm,int wasmFmLength);
void registerWasmFunc(JSRuntime *runtime, JSContext *pCtx);

#endif //ANDROID_QUICK_ANT_WASM_H
