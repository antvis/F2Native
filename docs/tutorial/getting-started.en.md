---
title: 快速上手
order: 0
redirect_from:
  - /en/docs/tutorial
---

### iOS
<img width = "260" src = "https://gw.alipayobjects.com/mdn/rms_b413ff/afts/img/A*ZJ-iRby9Gt4AAAAAAAAAAAAAARQnAQ"/>

#### CocoaPod Setup
```pod
pod 'F2'
```

#### Objective-C
```obj-c
#import <F2/F2.h>
@interface F2Demo : UIView
@property (nonatomic, strong) F2CanvasView *canvasView;
@end
@implementation  F2Demo
-(NSString *)jsonData {
    return @"[
      {\"genre\":\"Sports\",\"sold\":275},
      {\"genre\":\"Strategy\",\"sold\":115},
      {\"genre\":\"Action\",\"sold\":120},
      {\"genre\":\"Shooter\",\"sold\":350},
      {\"genre\":\"Other\",\"sold\":150}]";
}
- (void)chartRender {
    self.chart.canvas(self.canvasView);
    self.chart.padding(10, 20, 10, 0.f);
    self.chart.source([self jsonData]);
    self.chart.interval().position(@"genre*sold").color(@"genre", @[]);
    self.chart.scale(@"sold", @{@"min": @(0)});
    self.chart.scale(@"genre", @{@"range": @[@(0.1), @(0.9)]});
    self.chart.render();
}
```

#### Swift
```swift
func chartRender() {
    self.chart!.canvas()(self.canvasView!)
    self.chart!.padding()(10, 20, 10, 0)
    self.chart!.source()(jsonData)
    self.chart!.interval()().position()("genre*sold").color()("genre", [])
    self.chart!.scale()("sold", ["min":0])
    self.chart!.scale()("genre", ["range": [0.1, 0.9]])
    self.chart!.render()();
}
```


### Android
<img width = "260" src = "https://gw.alipayobjects.com/mdn/rms_b413ff/afts/img/A*YTFkT4NizlIAAAAAAAAAAAAAARQnAQ"/>

#### Gradle Setup
```gradle
repositories {
    maven { url 'https://jitpack.io' }
}

dependencies {
      implementation 'com.github.antvis:F2Native:latest.release'
}
```

#### XML
```xml
<com.antgroup.antv.f2.F2CanvasView
    android:id="@+id/canvasView"
    android:layout_width="match_parent"
    android:layout_height="220dp"/>
```

#### Java
```java
F2CanvasView canvasView = findViewById(R.id.canvasView);
canvasView.initCanvasContext();

canvasView.setAdapter(new F2CanvasView.Adapter() {
    private F2Chart mChart;
    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
       if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleIntervalChart_2", canvasView.getWidth(), canvasView.getHeight());
        }
        mChart.setCanvas(canvasView);
        mChart.padding(20, 10, 10, 10);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleIntervalChart_2.json"));
        mChart.interval().position("genre*sold").color("genre");
        mChart.setScale("sold", new F2Chart.ScaleConfigBuilder().min(0));
        mChart.setScale("genre", new F2Chart.ScaleConfigBuilder().range(new double[]{0.1, 0.9}));
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

#### Kotlin
```java
mCanvasView = findViewById(R.id.canvasView)
mCanvasView!!.initCanvasContext()

private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SingleIntervalChart_2",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(20.0, 10.0, 10.0, 10.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_singleIntervalChart_2.json"))
        mChart!!.interval().position("genre*sold").color("genre")
        mChart!!.setScale("sold", ScaleConfigBuilder().min(0.0))
        mChart!!.setScale("genre", ScaleConfigBuilder().range(doubleArrayOf(0.1, 0.9)))
        mChart!!.legend(
            "genre",
            LegendConfigBuild().enable(true).position("top").symbol("circle").setOption("radius", 3)
        )
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
```