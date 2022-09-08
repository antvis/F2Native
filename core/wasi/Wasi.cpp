#include <stdio.h>
#include "../core/graphics/XChart.h"

using namespace xg;

__attribute__((export_name("hello")))
int hello(float width, float height, float scale) {
  std::vector<XSourceItem> data;
  typedef pair<string,string> xData;
  typedef pair<string,int> yData;
  data.push_back({xData("genre", "Sports"), yData{"sold", 275}});
  data.push_back({xData("genre", "Strategy"), yData{"sold", 115}});
  data.push_back({xData("genre", "Action"), yData{"sold", 120}});
  data.push_back({xData("genre", "Shooter"), yData{"sold", 350}});
  data.push_back({xData("genre", "Other"), yData{"sold", 150}});
  XChart chart("wasi-chart", width, height, scale);
  chart.SetCanvasContext("wasi-context");
  chart.Source(data);
  chart.Interval().Position("genre*sold").Color("genre", {});
  chart.Render();
  return 1;
}
