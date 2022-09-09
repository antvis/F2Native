//
//  wasm_canvas_api.h
//  Demos
//
//  Created by weiqing.twq on 9/8/22.
//

#ifndef wasm_canvas_api_h
#define wasm_canvas_api_h

#include <Wasm3Lib/wasm3.h>
#include <F2/Core.h>

extern "C" M3Result m3_LinkCanvas  (IM3Module module, xg::canvas::CanvasContext *context);

#endif /* wasm_canvas_api_h */
