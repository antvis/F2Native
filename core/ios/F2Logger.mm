#include "F2Logger.h"
#include <utils/xtime.h>
#if defined(PRODUCT_WALLET)
#include <Foundation/Foundation.h>
#endif

namespace xg {
namespace ios {

void InnerLog(int level, const std::string &traceId, const char *fmt, ...) {
    std::string _tag = "F2Native|" + traceId;

    char msg[2048] = {0};
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, 2048, fmt, args);
    va_end(args);
    
#if defined(PRODUCT_WALLET)
    
#if defined(DEBUG)
    printf("%s %.0lf %s\n", _tag.c_str(), CFAbsoluteTimeGetCurrent() * 1000, msg);
#else
    NSString *tagStr = [NSString stringWithUTF8String:_tag.c_str()];
    NSString *msgStr = [NSString stringWithUTF8String:msg];
    APLogInfo(tagStr, @"%@", msgStr);
#endif
    
#else
    printf("%s %.0lld %s\n", _tag.c_str(), xg::CurrentTimestampAtMM(), msg);
#endif
}

}
}
