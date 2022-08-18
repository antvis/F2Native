#include <stdio.h>
#include "../core/graphics/XChart.h"

using namespace xg;

int main() {
  printf("Hello World!\n");

  XChart chart("wasi-chart", 400, 400, 1.0f);
  chart.Render();
  return 0;
}
