---
title: F2CanvasView
order: 1
---

# F2CanvasView
F2CanvasView 是 F2Native 图表的渲染引擎，对底层实际绘制引擎 api 进行二次封装与上层绘制指令进行解耦，实现底层绘制可插拔架构设计。图形引擎最终将图表绘制在 F2CanvasView 上，使用者根据添加到指定位置即可。

## 创建 F2CanvasView 实例
### iOS

```obj-c
#import <F2/F2.h>
F2CanvasView  *f2CanvasView = [F2CanvasView canvas:CGRectMake(0, 0, self.view.bounds.size.width, 280)];
[self.view addSubview:f2CanvasView];
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
```

## 手势
原生 View 手势事件对接 Chart 实例

### iOS

```obj-c
#import <F2/F2.h>

@interface DemoUIView () <F2GestureDelegate>

@property (nonatomic, strong) F2CanvasView *canvasView;
@property (nonatomic, strong) F2Chart *chart;

@end
@implementation DemoUIView

-(void)initView {
self.canvasView = [F2CanvasView canvas:CGRectMake(0, 0, self.view.bounds.size.width, 280)];
[self.view addSubview:f2CanvasView];

[self.canvasView addGestureListener:@"longPress" callback:^(NSDictionary * _Nonnull info) {
            F2StrongSelf;
            strongSelf.handleGestureInfo(info);
        }];
}

- (void)handleGestureInfo:(NSDictionary *)info {
    self.chart.postTouchEvent(info);
}

@end
```
### Android

```java
canvasView.setOnCanvasTouchListener(new F2CanvasView.OnCanvasTouchListener() {
    @Override
    public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
        mChart.postTouchEvent(event);
    }
});
```


