
![](https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*97sBRoNWXoIAAAAAAAAAAAAAARQnAQ)
![](https://img.shields.io/badge/language-c++-red.svg) ![](https://img.shields.io/badge/license-MIT-000000.svg)

[中文 README](./README.zh-CN.md)

F2Native is born for mobile visual development. It is out-of-the-box,cross-platform, high-performance visualization solution . Support Android and iOS perfect,moreover have high-performance experience in low-end mobile. Based on [the grammar of graphics](https://www.cs.uic.edu/~wilkinson/TheGrammarOfGraphics/GOG.html), F2Native provides all the chart types you'll need. Our mobile design guidelines enable better user experience in mobile visualzation projects.

![](https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*kWF0TYboysoAAAAAAAAAAAAAARQnAQ)

> Special thanks to [Leland Wilkinson](https://en.wikipedia.org/wiki/Leland_Wilkinson), the author of [*The Grammar Of Graphics*](https://www.cs.uic.edu/~wilkinson/TheGrammarOfGraphics/GOG.html), whose book served as the foundation for F2Native and F2.

## Installation

### iOS
Supports iOS 9.0+,Xcode 9.0+

1. DownLoad  all content of F2Native. <br/>
2. Execute the following command to pull the associated submodule.  <br/>

```bash
git submodule init
git submodule update 
```
3. Add the project under the iOS folder in F2 to your project. <br/>
4. import <F2/F2.h><br/>

### Android
1. Build GCanvas 
```shell
sh build_gcanvas_android.sh
```

2. Build F2Native
```shell
./gradlew :android:f2native:assemble
```

3. run Demo 
```shell
./gradlew :android:sample:installD
```


## Features
### Focus on the mobile,extreme experience
F2Native supports iOS and Android. Using the cross-platform language C++ to generate the underlying drawing commands，
 can achieve **cross-platform, unified experience, high-performance experience**.

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


## Getting Started

### iOS

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
-(void)drawGraph {
     // step1
     CGRect viewFrame = CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, 280);;
     self.canvasView = [F2CanvasView canvasWithFrame:viewFrame];
     [self addSubview:self.canvasView];
     // step2
     F2Chart *chart = [F2Chart chart:canvas.view.bounds.size withName:@"f2chart"];
     // step3
     chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source([self jsonData]);
     // step4
     chart.interval().position(@"genre*sold");
     // step5
     chart.render();
}
```


### Android

```xml
<com.antgroup.antv.f2.F2CanvasView
    android:id="@+id/canvasView"
    android:layout_width="match_parent"
    android:layout_height="220dp"/>
```

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

## How to Contribute


Please let us know how can we help. Do check out [issues](https://github.com/antvis/f2native/issues) for bug reports or suggestions first.

To become a contributor, please follow our [contributing guide](https://github.com/antvis/f2native/blob/master/CONTRIBUTING.md).

## License
[MIT license](./LICENSE)


