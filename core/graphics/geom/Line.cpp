#include "Line.h"
#include "../XChart.h"

using namespace xg;

void geom::Line::BeforeMapping(XChart &chart, XDataGroup &dataArray) {
    if(!styleConfig_.is_object() || styleConfig_.empty())
        return;

    auto &xScale = chart.GetScale(GetXScaleField());

    for(std::size_t index = 0; index < dataArray.size(); ++index) {

        auto &groupData = dataArray[index];
        std::size_t start = 0, end = groupData.size() - 1;
        if(scale::IsCategory(xScale.GetType())) {
            start = fmax(start, xScale.min);
            end = fmin(end, xScale.max);
        }

        for(std::size_t position = start; position <= end; ++position) {
            auto &item = groupData[position];
            item._style = styleConfig_;
        }
    }
}
