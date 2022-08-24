//
// Created by Hogan Liu on 2020/7/26.
//

#ifndef ANTMATION_MAC_ANTWASMENGINE_H
#define ANTMATION_MAC_ANTWASMENGINE_H

#include "Engine.h"
#include "ant_wasm.h"


namespace AntMation {

class AntWasmEngine : public Engine {
public:
    AntWasmEngine() : Engine(),
            wasmStackSize(ENGINE_DEFAULT_STACK_SIZE), wasmHeapSize(ENGINE_DEFAULT_HEAP_SIZE), memoryEnlarge(0),
            wasmSize(0), wasmData(nullptr), wa_module(nullptr), wa_module_inst(nullptr),
            _symbolsRuntime(nullptr), _symbolsCanvas(nullptr), _symbolsPlatform(nullptr) {}

    bool create(int stackSize, int heapSize, int memEnlarge) override;
    void destroy() override;

    bool initCanvasBindings(CanvasContext *ctx) override;
    bool initPlatformBindings(PlatformContext *ctx) override;
    bool initRuntimeBindings(RuntimeBase *ctx) override;

    bool loadFromFile(const char* path, const char *checkSum) override;
    bool loadFromBytes(const char* bytes, int len, const char *checkSum) override;

    bool callInit() override;
    void callResize(int w, int h) override;
    bool callDrawFrame(int idx) override;
    void callUpdateParams() override;
    void callUnInit() override;

    void callOnCreateImage(int idx, bool success) override;
    void callOnPreloadImages(bool success) override;

    void callOnEvent(const char *name, const char*extra) override;
    void callOnTimerCallback(int tid) override;

private:
    bool initWamr();

private:
    int wasmStackSize;
    int wasmHeapSize;
    int memoryEnlarge;
    int wasmSize;
    char *wasmData;

    AliWasmModule wa_module;
    AliWasmModuleInstance wa_module_inst;

    AliWasmApi *_symbolsRuntime;
    AliWasmApi *_symbolsPlatform;
    AliWasmApi *_symbolsCanvas;
};
}


#endif //ANTMATION_MAC_ANTWASMENGINE_H
