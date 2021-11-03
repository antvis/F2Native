#include "F2Logger.h"
#include <Foundation/Foundation.h>

namespace xg {
namespace ios {

void InnerLog(int level, const std::string &traceId, const char *fmt, ...) {
    std::string _tag = "ios|" + traceId;

    char msg[2048] = {0};
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, 2048, fmt, args);
    va_end(args);
    
#ifdef PRODUCT_WALLET
    
#if defined(DEBUG)
    printf("%s %s\n", _tag.c_str(), msg);
#else
    NSString *tagStr = [NSString stringWithUTF8String:_tag.c_str()];
    NSString *msgStr = [NSString stringWithUTF8String:msg];
    APLogInfo(tagStr, @"%@", msgStr);
#endif
    
#else
    printf("%s %s\n", _tag.c_str(), msg);
#endif
}

}
}
