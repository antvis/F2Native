---
title: F2Chart
order: 2
redirect_from:
  - /en s/docs/api/chart
---

The graphics instruction generation engine uses graphics syntax theory to encapsulate the api, and finally generates drawing instructions to the drawing engine for drawing.

## Create F2Chart instance
###iOS
````obj-c
// define
/// Quickly create chart
/// @param size dp is the unit
/// @param name The name will be appended when outputting the log
+ (F2Chart *)chart:(CGSize)size name:(NSString *)name;

// demo
#import <F2/F2.h>
F2Chart *chart = [F2Chart chart:f2Canvas.view.bounds.size name:@"F2chart"];
````
### Android
````java
F2Chart chart = F2Chart.create(context, "ChartName", width, height);
````

- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `size`, `width`, `height`| Size | The size of the canvas to draw the graphics |
| `name`| String | Custom graphic name, mainly used for log output |


## Method
### - canvas
Set the drawing engine instance corresponding to the current graphics command engine
#### iOS
````obj-c
  chart.canvas(f2CanvasView);
````
#### Android
````java
  chart.setCanvas(canvasView);
````
<br/>

### - padding
set padding [left, top, right, bottom]
#### iOS
````obj-c
  chart.padding(10, 0.f, 0.f, 0.f).;
````

#### Android
````java
  chart.padding(10, 0.f, 0.f, 0.f).;
````
<br/>

### - margin
Set graphics margin [left, top, right, bottom]
#### iOS
````obj-c
  chart.margin(10, 0.f, 0.f, 0.f);
````

#### Android
````java
  chart.margin(10, 0.f, 0.f, 0.f);
````
<br/>
 
### - source
set data source
#### iOS
````obj-c
  NSString *jsonString = @"[{\"date\":\"2010-01-10\",\"value\":99.9}{\"date\":\"2010-02-10\",\ "value\":96.7},{\"date\":\"2010-03-10\",\"value\":100.2}]"
  chart.source(jsonString);
````

#### Android
````java
  chart.source(jsonString);
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `json`| String | Drawing data jsonString, must be of type jsonArray |

<br/>

### - scale
set metric
#### iOS
````obj-c
  chart.scale(@"date", @{@"type":@"identity", @"scaleRange": @[@(0.1), @(0.9)] });
  chart.scale(@"value", @{ @"precision": @(1), @"max": @(200), @"min": @(0)});
````

#### Android
````java
  chart.setScale("value", new F2Chart.ScaleConfigBuilder().max(100).min(20));
  chart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(5));
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `field`| String | The name of the data currently to be measured in source|
| `config`| Dictionary | The specific configuration information of the measurement [see here](./F2Scale.md)|

<br/>


### - Coord
set coordinate system
#### iOS
````obj-c
    chart.coord(@{@"type": @"polar", @"transposed": @(YES)});
````

#### Android
````java
// TODO
````
- parameters
Type: Dictionary

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `type`| String | `polar` - polar coordinates <br/> `rect` - plane rectangular coordinates|
| `transposed`| BOOL | whether to flip |

<br/>

### - Axis
Set the coordinate axis configuration; the coordinate axis of F2-Native is composed as follows
#### iOS
````obj-c
 chart.axis(@"date", @{ @"grid": @(NO) });
 chart.axis(@"value", @{
              @"line": @(NO),
              @"grid": @{
                @"type": @"dash",
                @"lineWidth": @(1.0f),
                @"stroke": @"#E6E6E6",
                @"lineDash": @[@(6),@(3)]
             },
              @"label": @{ @"textColor": @"#cccccc" ,@"textAlign":@"end",@"labelOffset":@(5)}
        });
````

#### Android
````java
chart.setAxis("year", new F2Chart.AxisConfigBuilder()
        .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f))
        .gridHidden());
chart.setAxis("sales", new F2Chart.AxisConfigBuilder()
        .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5))
        .grid(new F2Chart.AxisGridConfigBuilder().type("dash"))
        .line(new F2Chart.AxisLineConfigBuilder().lineWidth(1).color("#E6E6E6").type("dash").lineDash(new double[]{6, 3})));
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `field`| String | The name of the data represented by the current axis in source|
| `config`| Dictionary | The specific configuration information of the coordinate axis [see here for details](./F2Axis)|

<br/>

### - Line
Create a line instance; F2Line configuration information [see here](./F2Line)

### - Interval
Create a column instance; F2Interval configuration information [see here](./F2Interval)

### - Area
Create a shadow instance; F2Area configuration information [see here](./F2Area)

### - Guide
Create an auxiliary instance; the configuration information of F2Guide [see here for details](./F2Guide)

<br/>

### - OnTouchEvent
Send interactive gesture information

#### iOS
````obj-c
 - (void)handleGestureInfo:(NSDictionary *)info {
    // do something
    chart.OnTouchEvent(info);
}
````

#### Android
````java
// TODO
````

- return data format

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `info`| Dictionary | If there is no special need, the info information of the callback can be returned directly|


### - legend
legend(filed,config); // set legend

#### iOS
````obj-c
   chart.legend(@"type", @{@"radius": @(3), @"symbol": @"square"});
}
````

#### Android
````java
// TODO
````

- return data format

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `filed`| String | Returns the name of the corresponding field to display the legend|
| `config`| Dictionary | Configuration information [see here](./F2Lengend) |



### - render
render operation
#### iOS
````obj-c
  // ...
  // After the chart configuration and graphic elements are created, start rendering
  chart.render();
````

#### Android
````java
  // ...
  // The chart configuration and graphic elements are created, start rendering
  chart.render();
````
<br/>