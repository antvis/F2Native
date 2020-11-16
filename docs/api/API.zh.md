---
title: F2Native
order: 0
redirect_from:
  - /zh/docs/api
---

F2Native 中两个基础的类是 F2CanvasView 和 F2Chart。在使用前需要先初始化，然后在再进行其他 api 调用。

## F2CanvasView

[F2CanvasView](./F2CanvasView.md) 提供图表渲染引擎，底层对接 [GCanvans](https://github.com/alibaba/GCanvas) 进行绘制。F2CanvasView 进行了可插拔设计，也可对接其他绘制引擎。


## F2Chart
[F2Chart](./chart/F2Chart.md) 图形指令生成引擎,使用图形语法理论封装 api，并最终生成绘制指令交给绘制引擎进行绘制。

## 快速开始

### iOS

```obj-c
#import <F2/F2.h>

@interface F2Demo : UIView
@property (nonatomic, strong) F2CanvasView *canvasView;
@end
@implementation  F2Demo
-(void)drawGraph {
     CGRect viewFrame = CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, chartHeight);
     self.canvasView = [F2CanvasView canvasWithFrame:viewFrame];
     [self.view addSubview:self.canvasView];
     F2Chart *chart = [F2Chart chart:canvas.view.bounds.size withName:@"f2chart"];
     chart.canvas(self.canvasView).padding(10, 0, 0, 0.f).source([self jsonData]);
     chart.line().position(@"date*value").color(@"type", @[]).fixedSize(1).fixedShape(@"smooth");
     chart.render();
}
-(NSString *)jsonData {
    return @"[{\"date\":\"2010-01-10\",\"type\":\"能源\",\"value\":99.9},{\"date\":\"2010-01-10\",\"type\":\"金属\",\"value\":96.6},{\"date\":\"2010-01-10\",\"type\":\"农副产品\",\"value\":96.2},{\"date\":\"2010-02-10\",\"type\":\"能源\",\"value\":96.7},{\"date\":\"2010-02-10\",\"type\":\"金属\",\"value\":91.1},{\"date\":\"2010-02-10\",\"type\":\"农副产品\",\"value\":93.4},{\"date\":\"2010-03-10\",\"type\":\"能源\",\"value\":100.2},{\"date\":\"2010-03-10\",\"type\":\"金属\",\"value\":99.4},{\"date\":\"2010-03-10\",\"type\":\"农副产品\",\"value\":91.7}]";
}

```

### Android

```java
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
```
