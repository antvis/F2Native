---
title: 快速上手
order: 0
redirect_from:
  - /zh/docs/tutorial
---

## 安装

### 倒入依赖
1. 执行 git submodule init && git submodule update 拉取关联仓库源码
2. android 编译需配置好本地 NDK 环境

### iOS
iOS 支持 iOS 9.0+,Xcode 9 及以上

1. 将 F2 内的 iOS 文件夹下工程添加到你的工程 <br/>

2. 导入 <F2/F2.h><br/>

### Android

1. 构建 GCanvas 依赖 
```shell
sh build_gcanvas_android.sh
```

2. 构建 F2Native
```shell
./gradlew :android:f2native:assemble
```

3. 运行 Demo 程序
```shell
./gradlew :android:sample:installD
```

## 一分钟上手

在 F2 引入工程后，我们就已经做好了创建第一个图表的准备了。

下面是以一个基础的柱状图为例开始我们的第一个图表创建。

主要通过以下几个步骤进行创建：

* step1:  初始化 F2CanvasView 图形视图；<br/>
* step2:  创建 F2Chart 图形实例 <br/>
* step3:  F2Chart 图形实例关联渲染视图和数据等配置项 <br/>
* step4:  使用图形语法 API 完成图表描述 <br/>
* step5:  渲染图表 <br/>

### iOS

```obj-c
#import <F2/F2.h>
@interface F2Demo : UIView
@property (nonatomic, strong) F2CanvasView *canvasView;
@end
@implementation  F2Demo

// F2Native 对数据源格式的要求，仅仅是 JSON 数组，数组的每个元素是一个标准 JSON 对象。
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
           mChart = F2Chart.create(canvasView.getContext(), "SingleIntervalChart_2", canvasView.getWidth(), canvasView.getHeight());
       }
        mChart.setCanvas(canvasView);
        mChart.padding(20, 10, 10, 10);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleIntervalChart_2.json"));
        mChart.interval().position("genre*sold").color("genre");
        mChart.setScale("sold", new F2Chart.ScaleConfigBuilder().min(0));
        mChart.setScale("genre", new F2Chart.ScaleConfigBuilder().range(new double[]{0.1, 0.9}));
        mChart.legend("genre", new F2Chart.LegendConfigBuild().enable(true).position("top").symbol("circle").setOption("radius", 3));
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
<br/>
通过上述步骤一张柱状图就绘制完成了：<br/><br/>
<center class="half">
    <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*dwaWQIxq7VsAAAAAAAAAAAAAARQnAQ" width = "300"/>
     &nbsp;&nbsp;
    <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*PUiPR7F9auYAAAAAAAAAAAAAARQnAQ" width = "300" />
</center>