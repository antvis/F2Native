---
title: F2Chart
order: 2
redirect_from:
  - /en s/docs/api/chart
---

图形指令生成引擎,使用图形语法理论封装 api，并最终生成绘制指令交给绘制引擎进行绘制。

## 创建 F2Chart 实例
### iOS
```obj-c
// 定义
/// 快捷创建chart
/// @param size dp为单位
/// @param name 输出日志时会附加name
+ (F2Chart *)chart:(CGSize)size withName:(NSString *)name;

// demo
#import <F2/F2.h>
F2Chart *chart = [F2Chart chart:f2Canvas.view.bounds.size withName:@"F2chart"];
```
### Android
```java
F2Chart chart = F2Chart.create(context, "ChartName", width, height);
```

- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `size`, `width`, `height `| Size | 绘制图形的画布大小 |
| `name`| String | 自定义图形名称，主要用做日志输出 |


## 方法
### - canvas
设置当前图形指令引擎对应的绘制引擎实例
#### iOS
```obj-c
  chart.canvas(f2CanvasView);
```
#### Android
```java
  chart.setCanvas(canvasView);
```
<br/>

### - padding
设置padding [left, top, right, bottom]
#### iOS
```obj-c
  chart.padding(10, 0.f, 0.f, 0.f).;
```

#### Android
```java
  chart.padding(10, 0.f, 0.f, 0.f).;
```
<br/>

### - margin
设置图形margin [left, top, right, bottom]
#### iOS
```obj-c
  chart.margin(10, 0.f, 0.f, 0.f);
```

#### Android
```java
  chart.margin(10, 0.f, 0.f, 0.f);
```
<br/>
 
### - source
设置数据源
#### iOS
```obj-c
  NSString *jsonString = @"[{\"date\":\"2010-01-10\",\"value\":99.9}{\"date\":\"2010-02-10\",\"value\":96.7},{\"date\":\"2010-03-10\",\"value\":100.2}]"
  chart.source(jsonString);
```

#### Android
```java
  chart.source(jsonString);
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `json`| String | 绘图数据 jsonString, 必须为 jsonArray 类型 |

<br/>

### - scale
设置度量
#### iOS
```obj-c
  chart.scale(@"date", @{@"type":@"identity", @"scaleRange": @[@(0.1), @(0.9)] });
  chart.scale(@"value", @{ @"precision": @(1), @"max": @(200), @"min": @(0)});
```

#### Android
```java
  chart.setScale("value", new F2Chart.ScaleConfigBuilder().max(100).min(20));
  chart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(5));
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `field`| String | 当前需要度量的数据在 source 中的名称|
| `config`| Dictionary | 度量的具体配置信息[详见这里](./F2Scale.md)|

<br/>


### - coord
设置坐标系
#### iOS
```obj-c
    chart.coord(@{@"type": @"polar", @"transposed": @(YES)});
```

#### Android
```java
// TODO
```
- 参数 
类型：Dictionary

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `type`| String | `polar` - 极坐标 <br/> `rect` - 平面直角坐标|
| `transposed`| BOOL | 是否翻转|

<br/>

### - axis
设置坐标轴配置；F2-Native的坐标轴组成如下
#### iOS
```obj-c
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
```

#### Android
```java
chart.setAxis("year", new F2Chart.AxisConfigBuilder()
        .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f))
        .gridHidden());
chart.setAxis("sales", new F2Chart.AxisConfigBuilder()
        .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5))
        .grid(new F2Chart.AxisGridConfigBuilder().type("dash"))
        .line(new F2Chart.AxisLineConfigBuilder().lineWidth(1).color("#E6E6E6").type("dash").lineDash(new double[]{6, 3})));
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `field`| String | 当前坐标轴所代表的数据在 source 中的名称|
| `config`| Dictionary | 坐标轴的具体配置信息[详见这里](./F2Axis)|

<br/>

### - line
创建线实例；F2Line 的配置信息[详见这里](./F2Line)

### - interval
创建柱实例；F2Interval 的配置信息[详见这里](./F2Interval)

### - area
创建阴影实例；F2Area的配置信息[详见这里](./F2Area)

### - guide
创建辅助实例；F2Guide的配置信息[详见这里](./F2Guide)

<br/>

### - OnTouchEvent
发送交互手势信息

#### iOS
```obj-c
 - (void)handleGestureInfo:(NSDictionary *)info {
    // do something
    chart.OnTouchEvent(info);
}
```

#### Android
```java
// TODO
```

- 返回数据格式

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `info`| Dictionary | 如果没有特殊需要可将回调的 info 信息直接返回|


### - legend
legend(filed,config);  // 设置图例

#### iOS
```obj-c
   chart.legend(@"type", @{@"radius": @(3), @"symbol": @"square"});
}
```

#### Android
```java
// TODO
```

- 返回数据格式

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `filed`| Stirng | 返回需要展示图例的对应字段名称|
| `config`| Dictionary | 配置信息[详见这里](./F2Lengend) |



### - render
渲染操作
#### iOS
```obj-c
  // ... 
  // chart 配置及图形元素创建完毕, 开始渲染
  chart.render();
```

#### Android
```java
  // ... 
  // chart 配置及图形元素创建完毕, 开始渲染
  chart.render();
```
<br/>

### - getRenderDumpInfo
获取渲染完成后的指令信息

#### iOS
```obj-c
  chart.render()
  NSString *jsonString = chart.getRenderDumpInfo();
  // {"cmdCount":495}
```

#### Android
```java
  chart.render()
  String *jsonString = chart.getRenderDumpInfo();
  // {"cmdCount":495}
```

- 返回数据格式

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `cmdCount`| Number | 渲染指令的个数<br/>-1表示环境有问题<br/> 0表示指令数为0<br/>>0表示正常的绘制指令个数|
