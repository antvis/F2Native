//
// Created by Hogan Liu on 2020/8/5.
//

#include "AssetProvider.h"
#include <cstdio>

namespace AntMation {

int AssetProvider::getLength(const char *path) {
    if (path == nullptr) return 0;

    FILE *file = fopen(getFullPath(path).c_str(), "rb");
    if (!file) return 0;

    int length = 0;
    int seekRet = fseek(file, 0, SEEK_END);
    if (seekRet == 0) {
        length = (int) ftell(file);
    }
    fclose(file);

    return length;
}

bool AssetProvider::readFile(const char *path, char **buffer, int* length) {
    if (path == nullptr || buffer == nullptr) return false;

    FILE *file = fopen(getFullPath(path).c_str(), "rb");
    if (!file) return false;

    bool readSuccess = false;
    int seekRet = fseek(file, 0, SEEK_END);
    if (seekRet == 0) {
        int totalLen = (int) ftell(file);
        if (totalLen > 0) {
            seekRet = fseek(file, 0, SEEK_SET);
            if (seekRet == 0) {
                char *readBuffer = new char[totalLen + 1];
                int readLen = fread(readBuffer, 1, totalLen, file);
                if (readLen != totalLen) {
                    delete[] readBuffer;
                } else {
                    readBuffer[totalLen] = 0;

                    *buffer = readBuffer;
                    *length = totalLen;
                    readSuccess = true;
                }
            }
        }
    }

    fclose(file);
    return readSuccess;
}

std::string AssetProvider::getFullPath(const char* path) {
    if (AntMation::Utils::hasPrefix(path, "/")) {
        return path;
    }
    return _sandboxPath + path;
}

}