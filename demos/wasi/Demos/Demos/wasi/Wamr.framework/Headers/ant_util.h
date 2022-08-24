//
// Created by tianyang on 2020/7/16.
//

#include <map>
#include "pthread.h"
#include <string>
#ifndef ANT_UTIL_H
#define ANT_UTIL_H

#define EVENT_WASM_INIT_COST "EVENT_WASM_INIT_COST"
#define EVENT_WASM_CREATE_CMODULE_COST "EVENT_WASM_CREATE_CMODULE_COST"

#define ERROR_WASM_INIT "ERROR_WASM_INIT"
#define ERROR_WASM_CREATE_CMODULE "ERROR_WASM_CREATE_CMODULE"
#define ERROR_WASM_CREATE_CMODULE_INSTANCE "ERROR_WASM_CREATE_CMODULE_INSTANCE"
#define ERROR_WASM_EXECUTE_CMODULE "ERROR_WASM_EXECUTE_CMODULE"

#define PARAM_WASM_BIZ "PARAM_WASM_BIZ"
#define PARAM_WASM_WASM_DATA_LENGTH "PARAM_WASM_WASM_DATA_LENGTH"
#define PARAM_WASM_EXECUTE_METHOD "PARAM_WASM_EXECUTE_METHOD"
#define PARAM_WASM_EXECUTE_SIG "PARAM_WASM_EXECUTE_SIG"


pthread_mutex_t *getWasmInstanceLock();
pthread_mutex_t *getWasmApiLock();
pthread_mutex_t getRecursiveMutexThread();


void wasm_log_info(const char *format, ...);
void wasm_log_debug(const char *format, ...);
void wasm_log_error(const char *format, ...);

void wasm_event(const char* event,const char* subEvent, const char* message,std::map<std::string,std::string> param = std::map<std::string,std::string>());
void wasm_error(const char* type,const char* exception,std::map<std::string,std::string> param = std::map<std::string,std::string>());
std::string wasm_config(const char* key);


double getCurrentTime();

#ifdef __ANDROID__
extern bool libIsAttached;
#endif

#define WSLogInfo( message, ...) WSLogI(message, ##__VA_ARGS__);
#define WSLogDebug( message, ...) WSLogD(message, ##__VA_ARGS__);
#define WSLogError( message, ...) WSLogE(message, ##__VA_ARGS__);

#define WSLogI(message, ...)                    \
    do {                                        \
            wasm_log_info(message, ##__VA_ARGS__);  \
    } while (0);
#define WSLogD message, ...)                    \
    do {                                        \
            wasm_log_debug(message, ##__VA_ARGS__); \
    } while (0);
#define WSLogE message, ...)                    \
    do {                                        \
            wasm_log_error(message, ##__VA_ARGS__); \
    } while (0);


#endif //ANT_UTIL_H
