//
// Created by tianyang on 2020/7/16.
//

#ifndef ANT_WASM_H
#define ANT_WASM_H

#define ANT_WASM_OK 1
#define ANT_WASM_ERROR 0

#ifdef __cplusplus
extern "C" {
#endif

typedef void* AliWasmModule;
typedef void* AliWasmModuleInstance;
typedef void* AliWasmFunction;
typedef void* AliWasmExecEnv;

typedef struct _WasmExport {
    char *name;
    uint8_t kind;
    uint32_t index;
} AliWasmExport;

typedef struct _WasmModuleConfig {
    const char* bizCode = 0;
} AliWasmModuleConfig;

typedef struct _WasmModuleInstanceConfig {
    int stackSize = 64*1024;
    int heapSize = 64*1024;
} AliWasmModuleInstanceConfig;

typedef struct _WasmApi {
    char *apiName = 0;
    void *func_ptr = 0;
    char *signature = 0;
    void *attachment = 0;
} AliWasmApi;

int ant_wasm_init();

AliWasmModule ant_wasm_create_module(AliWasmModuleConfig config);

int ant_wasm_module_load(AliWasmModule module,const char *data,int size,char **error_buf);

AliWasmModuleInstance ant_wasm_create_module_instance(AliWasmModule module,AliWasmModuleInstanceConfig config,char **error_buf);

int ant_wasm_execute(AliWasmModuleInstance moduleInstance,const char* name,const char* sig,int argv[],int argc,int* success,char **error_buf);

int ant_wasm_malloc(AliWasmModuleInstance moduleInstance,int size,void **p_native_addr);

void ant_wasm_free(AliWasmModuleInstance moduleInstance, int ptr);

void ant_wasm_destroy_module(AliWasmModule module);

void ant_wasm_destroy_module_instance(AliWasmModuleInstance moduleInstance);

int ant_wasm_register_api(AliWasmModule module,AliWasmApi *apis,int apiCount);

int ant_wasm_register_raw_api(AliWasmModule module,AliWasmApi *apis,int apiCount);

void ant_wasm_unregister_api(AliWasmModule module);

void * ant_wasm_addr_app_to_native(AliWasmModuleInstance moduleInstance,int app_offset);

AliWasmModuleInstance ant_wasm_get_module_instance(AliWasmExecEnv execEnv);

void * ant_wasm_get_attachment(AliWasmExecEnv execEnv);

int ant_wasm_get_export(AliWasmModule module,AliWasmExport** exp);

bool ant_wasm_enlarge_memory(AliWasmModuleInstance moduleInstance,uint32_t page_count);

AliWasmFunction ant_wasm_lookup_function(AliWasmModuleInstance moduleInstance,const char* name,const char* sig);

bool ant_wasm_heap_stat(AliWasmModuleInstance moduleInstance,uint32_t* stat_info);

#ifdef __cplusplus
}
#endif


#endif //ANT_WASM_H
