
![](https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*97sBRoNWXoIAAAAAAAAAAAAAARQnAQ)
![](https://img.shields.io/badge/language-c++-red.svg) ![](https://img.shields.io/badge/license-MIT-000000.svg)

[中文 README](./README_cn.md)

F2Native is born for mobile visual development. It is out-of-the-box,cross-platform, high-performance visualization solution . Support Android, iOS and MacOS perfect,moreover have high-performance experience in low-end mobile. Based on [the grammar of graphics](https://www.cs.uic.edu/~wilkinson/TheGrammarOfGraphics/GOG.html), F2Native provides all the chart types you'll need. Our mobile design guidelines enable better user experience in mobile visualzation projects.

![](https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*kWF0TYboysoAAAAAAAAAAAAAARQnAQ)

> Special thanks to [Leland Wilkinson](https://en.wikipedia.org/wiki/Leland_Wilkinson), the author of [*The Grammar Of Graphics*](https://www.cs.uic.edu/~wilkinson/TheGrammarOfGraphics/GOG.html), whose book served as the foundation for F2Native and F2.


## Features
### Focus on the mobile,extreme experience
F2Native supports Android, iOS And MacOS. Using the cross-platform language C++ to generate the underlying drawing commands，
 can achieve **cross-platform, unified experience, high-performance experience**. After packaging on Android and iOS, the APK and IPA size are **250KB and 500KB** respectively.

### Light and natural
make data more alive and chart interactions more natural.

### All the chart types you want
With the power of grammar of graphics, F2Native including classical charts such as line, column/bar chart, pie chart. Additionally, F2 also provides feature-riched chart components to meet various needs.


## Links

* [Website](https://f2native.antv.vision/)
* English documents: https://f2native.antv.vision/en
* 中文文档: https://f2native.antv.vision/zh



## Demos

[Chart Demos](https://f2native.antv.vision/en/docs/examples/line/line)
[Video Demos](./demos/videos/ios_demo.mp4)

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

## How to Contribute


Please let us know how can we help. Do check out [issues](https://github.com/antvis/f2native/issues) for bug reports or suggestions first.

To become a contributor, please follow our [contributing guide](https://github.com/antvis/f2native/blob/master/CONTRIBUTING.md).

## License
[MIT license](./LICENSE)


