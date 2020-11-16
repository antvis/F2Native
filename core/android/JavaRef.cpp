//
// Created by yuantong on 2018/6/28.
//

#include "JavaRef.h"
#include "JNIUtil.h"

#include <assert.h>
namespace xg {
namespace jni {

JavaRef<jobject>::JavaRef() : obj_(NULL) {}

JavaRef<jobject>::JavaRef(JNIEnv *env, jobject obj) : obj_(obj) {
    if(obj) {
        assert(env && env->GetObjectRefType(obj) == JNILocalRefType);
    }
}

JavaRef<jobject>::~JavaRef() = default;

JNIEnv *JavaRef<jobject>::SetNewLocalRef(JNIEnv *env, jobject obj) {
    if(!env) {
        env = GetJniEnvSafe();
    } else {
        // assert(env == AttachCurrentThread());  // Is |env| on correct thread.
    }
    if(obj)
        obj = env->NewLocalRef(obj);
    if(obj_)
        env->DeleteLocalRef(obj_);
    obj_ = obj;
    return env;
}

void JavaRef<jobject>::SetNewGlobalRef(JNIEnv *env, jobject obj) {
    if(!env) {
        env = GetJniEnvSafe();
    } else {
        // assert(env == AttachCurrentThread());  // Is |env| on correct thread.
    }
    if(obj)
        obj = env->NewGlobalRef(obj);
    if(obj_)
        env->DeleteGlobalRef(obj_);
    obj_ = obj;
}

void JavaRef<jobject>::ResetLocalRef(JNIEnv *env) {
    if(obj_) {
        // assert(env == AttachCurrentThread());  // Is |env| on correct thread.
        env->DeleteLocalRef(obj_);
        obj_ = NULL;
    }
}

void JavaRef<jobject>::ResetGlobalRef() {
    if(obj_) {
        JNIEnv *env = GetJniEnvSafe();
        if(env != nullptr) {
            env->DeleteGlobalRef(obj_);
        }
        obj_ = NULL;
    }
}

jobject JavaRef<jobject>::ReleaseInternal() {
    jobject obj = obj_;
    obj_ = NULL;
    return obj;
}
} // namespace jni

} // namespace xg