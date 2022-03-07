#include "utils/xtime.h"
#include <time.h>

 using namespace xg;
 using namespace std;

namespace unit {

class XTime {
public:
    static bool Parse() {
        time_t rst = DateParserAtSecond("2020-01-01 14:00:00");
        return (1577858400 == rst);
    };

    static bool ParseTM() {
        tm time1 = DateParserAtTM("2021-01-07 15:00:00");
        tm time2 = DateParserAtTM("2021-01-08 09:35:59");
        return (8 == time2.tm_mday) && (7 == time1.tm_mday);
    };

    static bool ParseTimeStamp() {
        //2016-05-29 07:37:27
        tm t = DateParserTimeStamp(1464478647000);
        return (2016 == 1900 + t.tm_year) && (29 == t.tm_mday) && (37 == t.tm_min) && (0 == t.tm_wday);
    };
    
    static bool ParseTimeStampTimeZone() {
        //2016-05-29 07:37:27
        tm t = DateParserTimeStamp(1464478647000, true);
        //区别于上面是 t.tm_mday
        return (2016 == 1900 + t.tm_year) && (28 == t.tm_mday) && (37 == t.tm_min) && (6 == t.tm_wday);
    };
};

}
