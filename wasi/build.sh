#for WASI
rm -rf build && mkdir build && cd build
emcmake cmake  -DEMSCRIPTEN_FORCE_COMPILERS=OFF .. -DCMAKE_CXX_FLAGS="-DNDEBUG"
emmake make 