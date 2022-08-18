rm -rf build && mkdir build && cd build
emcmake cmake ..
emmake make 

cp -rf f2wasm.js ../../demos/webassembly/f2wasm.js
cp -rf f2wasm.wasm ../../demos/webassembly/f2wasm.wasm
