---
title: F2Native
order: 0
redirect_from:
  - /en/docs/api
---

The two basic classes in F2Native are F2CanvasView and F2Chart. It needs to be initialized before use, and then make other api calls.

## F2CanvasView

[F2CanvasView](./F2CanvasView.md) provides a chart rendering engine, and draws with CoreGraphics and Android Canvas of the underlying docking system. F2CanvasView has a pluggable design and can also be connected to other drawing engines.


## F2Chart
[F2Chart](./chart/F2Chart.md) Graphics instruction generation engine, uses graphic syntax theory to encapsulate api, and finally generates drawing instructions to the drawing engine for drawing.

## Quick start

### iOS

````obj-c
#import <F2/F2.h>

@interface F2Demo : UIView
@property (nonatomic, strong) F2CanvasView *canvasView;
@end
@implementation F2Demo
-(void)drawGraph {
     CGRect viewFrame = CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, chartHeight);
     self.canvasView = [F2CanvasView canvas:viewFrame];
     [self.view addSubview:self.canvasView];
     F2Chart *chart = [F2Chart chart:canvas.view.bounds.size name:@"f2chart"];
     chart.canvas(self.canvasView).padding(10, 0, 0, 0.f).source([self jsonData]);
     chart.line().position(@"date*value").color(@"type", @[]).fixedSize(1).fixedShape(@"smooth");
     chart.render();
}
-(NSString *)jsonData {
    return @"[{\"date\":\"2010-01-10\",\"type\":\"Energy\",\"value\":99.9},{\"date\":\ "2010-01-10\",\"type\":\"Metal\",\"value\":96.6},{\"date\":\"2010-01-10\",\"type \":\"Agricultural and sideline products\",\"value\":96.2},{\"date\":\"2010-02-10\",\"type\":\"Energy\",\" value\":96.7},{\"date\":\"2010-02-10\",\"type\":\"Metal\",\"value\":91.1},{\"date\ ":\"2010-02-10\",\"type\":\"Agricultural and sideline products\",\"value\":93.4},{\"date\":\"2010-03-10\" ,\"type\":\"Energy\",\"value\":100.2},{\"date\":\"2010-03-10\",\"type\":\"Metal\" ,\"value\":99.4},{\"date\":\"2010-03-10\",\"type\":\"Agricultural and sideline products\",\"value\":91.7}]" ;
}

````

### Android

````java
F2CanvasView canvasView = findViewById(R.id.canvasView);
canvasView.initCanvasContext(new F2CanvasView.ConfigBuilder().asyncRender(true).build());

canvasView.setAdapter(new F2CanvasView.Adapter() {
    private F2Chart mChart;
    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
       if (mChart == null) {
           mChart = F2Chart.create(canvasView.getContext(), "SingleLineChart_1", canvasView.getWidth(), canvasView.getHeight());
       }
       mChart.setCanvas(canvasView);
       mChart.padding(10, 0, 0, 0);
       mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleLineChart.json"));
       mChart.line().position("date*value");
       mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(3));
       mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
});
````