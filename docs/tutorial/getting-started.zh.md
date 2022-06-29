---
title: 快速上手
order: 0
redirect_from:
  - /zh/docs/tutorial
---
## 一分钟上手

在 F2 引入工程后，我们就已经做好了创建第一个图表的准备了。

下面是以一个基础的柱状图为例开始我们的第一个图表创建。

主要通过以下几个步骤进行创建：

* step1:  初始化 F2CanvasView 图形视图；<br/>
* step2:  创建 F2Chart 图形实例 <br/>
* step3:  F2Chart 图形实例关联渲染视图和数据等配置项 <br/>
* step4:  使用图形语法 API 完成图表描述 <br/>
* step5:  渲染图表 <br/>

## 快速开始

### iOS
<img width = "260" src = "https://gw.alipayobjects.com/mdn/rms_b413ff/afts/img/A*ZJ-iRby9Gt4AAAAAAAAAAAAAARQnAQ"/>

#### CocoaPod设置
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

#### Gradle设置
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