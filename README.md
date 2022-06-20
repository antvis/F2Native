![](https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*sK26T47j3tEAAAAAAAAAAAAAARQnAQ)
![](https://img.shields.io/badge/language-c++-red.svg) ![](https://img.shields.io/badge/license-MIT-000000.svg)

[English README](./README_en.md)

F2Native，专注于移动 Native 可视化开发，开箱即用的跨平台高性能的可视化解决方案。完美支持 Android, iOS, 和MacOS 平台，并在中低端机型具备高性能体验。完备的图形语法理论，满足你的各种可视化需求。专业的移动设计指引为你带来最佳的移动端图表体验。

![](https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*kWF0TYboysoAAAAAAAAAAAAAARQnAQ)
> 在此衷心感谢[《The Grammar of Graphics》](https://www.cs.uic.edu/~wilkinson/TheGrammarOfGraphics/GOG.html)的作者 [Leland Wilkinson](https://en.wikipedia.org/wiki/Leland_Wilkinson)，为 F2 的图形语法提供了理论基础！


## 特性

### 专注客户端，极致体验

F2Native 支持 Android，iOS，MacOS 平台。使用跨平台语言 C++ 来生成底层绘制指令, 可以做到 **跨平台，多端体验统一，高性能体验**。

### 轻量化植入，自然呈现
在设计上我们以人为本，追求自然简单易懂。可视化展现和人的自然行为特征有机融合，使图表展示更为自然，并且引入灵活、成本低。

### 图表丰富，组件完备
与传统的图表库不同，抛弃了特图特做的封装思路，基于强大的图形语法理论，以数据驱动，通过图形语法的组合灵活构建各类图表，覆盖各类场景在提供基础的图表可视化能力外，我们还提供了丰富图表功能组件，满足各种功能需求。



## 文档

* [Website](https://f2native.antv.vision/)
* English documents: https://f2native.antv.vision/en
* 中文文档: https://f2native.antv.vision/zh

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
     implementation 'com.github.antvis:F2Native:2.0.15'
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

## 联系我们
![](https://gw.alipayobjects.com/mdn/rms_b413ff/afts/img/A*O-tFT7mMQ74AAAAAAAAAAAAAARQnAQ)

## 如何贡献

如果您在使用的过程中碰到问题，可以先通过 [issues]() 看看有没有类似的 bug 或者建议。

如需提交代码，请遵从我们的[贡献指南](./CONTRIBUTING.md)。

## License
[MIT license](./LICENSE)
