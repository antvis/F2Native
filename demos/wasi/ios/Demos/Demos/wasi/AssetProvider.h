//
// Created by Hogan Liu on 2020/8/5.
//

#ifndef ANTMATION_MAC_ASSETPROVIDER_H
#define ANTMATION_MAC_ASSETPROVIDER_H

#include <string>

namespace AntMation {

class AssetProvider {
public:
    explicit AssetProvider(const char *sandboxPath) : _sandboxPath(sandboxPath) {
//        if (!Utils::hasSuffix(_sandboxPath, "/")) {
//            _sandboxPath += "/";
//        }
    }
    inline const std::string &getSandboxPath() const { return _sandboxPath; }

    virtual int getLength(const char *path);
    virtual bool readFile(const char *path, char **buffer, int* length);
    virtual std::string getFullPath(const char* path);

protected:
    std::string _sandboxPath;
};

}

#endif //ANTMATION_MAC_ASSETPROVIDER_H
