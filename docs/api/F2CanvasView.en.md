---
title: F2CanvasView
order: 1
---

# F2CanvasView
F2CanvasView is the rendering engine of F2Native charts. It decouples the underlying actual drawing engine api by secondary encapsulation and the upper-level drawing instructions, and realizes the design of the bottom-level drawing pluggable architecture. The graphics engine finally draws the chart on the F2CanvasView, and the user can add it to the specified position.

## Create An Instance Of F2CanvasView
### iOS

````obj-c
#import <F2/F2.h>
F2CanvasView *f2CanvasView = [F2CanvasView canvas:CGRectMake(0, 0, self.view.bounds.size.width, 280)];
[self.view addSubview:f2CanvasView];
````
### Android

````xml
<com.antgroup.antv.f2.F2CanvasView
    android:id="@+id/canvasView"
    android:layout_width="match_parent"
    android:layout_height="220dp"/>
````

````java
F2CanvasView canvasView = findViewById(R.id.canvasView);
canvasView.initCanvasContext(new F2CanvasView.ConfigBuilder().asyncRender(true).build());
````

## Gestures
Native View gesture event docking with Chart instance

### iOS

````obj-c
#import <F2/F2.h>

@interface DemoUIView() <F2GestureDelegate>

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
````
### Android

````java
canvasView.setOnCanvasTouchListener(new F2CanvasView.OnCanvasTouchListener() {
    @Override
    public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
        mChart.postTouchEvent(event);
    }
});
````