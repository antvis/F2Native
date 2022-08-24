//
// Created by Hogan Liu on 2020/7/14.
//

#ifndef ANTMATION_MAC_ENGINE_H
#define ANTMATION_MAC_ENGINE_H

#include <string>

namespace AntMation {

class AssetProvider;

class CanvasContext;

#define ENGINE_DEFAULT_STACK_SIZE (4*1024)
#define ENGINE_DEFAULT_HEAP_SIZE (16*1024)

class Engine {
public:
    Engine() : _asset(nullptr) {};
    virtual ~Engine() {};

    inline void setAssetProvider(AssetProvider *provider) { _asset = provider; };

    virtual bool create(int stackSize, int heapSize, int memEnlarge) = 0;
    virtual void destroy() = 0;

    virtual bool initCanvasBindings(CanvasContext *ctx) = 0;

    virtual bool loadFromFile(const char *path, const char *checkSum) = 0;
    virtual bool loadFromBytes(const char *bytes, int len, const char *checkSum) = 0;

    // call
    virtual bool callInit() = 0;
    virtual void callResize(int w, int h) = 0;
    virtual bool callDrawFrame(int idx) = 0;
    virtual void callUpdateParams() = 0;
    virtual void callUnInit() = 0;

    virtual void callOnCreateImage(int idx, bool success) = 0;
    virtual void callOnPreloadImages(bool success) = 0;

    virtual void callOnEvent(const char *name, const char*extra) = 0;
    virtual void callOnTimerCallback(int tid) = 0;

public:
    inline std::string &getError() { return _error; };

protected:
    AssetProvider *_asset;
    std::string _error;
};

}

#endif //ANTMATION_MAC_ENGINE_H
