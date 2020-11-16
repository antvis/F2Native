---
title: F2Interval
order: 6
---

图形柱对象，用户绘制柱图

## 创建 F2Interval 实例
#### iOS
```obj-c
  F2CanvasView *f2CanvasView = [F2CanvasView canvasWithFrame:CGRectMake(0, 0, 280, 280)];
  F2Chart *chart = [F2Chart chart:f2CanvasView.bounds.size withName:@"F2chart"];
  chart.interval();
```

#### Android
```java
  chart.interval();
```
<br/>

## 方法
### - position 
绘制柱图位置
#### iOS
```obj-c
  chart.interval().position(@"date*value");
```
#### Android
```java
  chart.interval().position("date*value");
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `attr`| String | X*Y：绘制的柱在 source 中对应的名称

<br/>

### - fixedColor 
设置柱填充的颜色
#### iOS
```obj-c
  chart.interval().position(@"date*value").fixedColor(@"#108EE9");
```
#### Android
```java
  chart.interval().position("date*value").fixedColor("#108EE9");
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `color`| String | 设置区域填充的颜色 （Hex rgb）
<br/>

### - color 
设置分组柱填充的颜色
#### iOS
```obj-c
  chart.interval().position(@"date*value").color(@"type", @[@"#108EE9", @"#2FC25B"]);
```
#### Android
```java
  chart.interval().position("date*value").color("type", new String[]{"#108EE9", "#2FC25B"});
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `field`| String | 数据分组字段
| `color`| Array&lt;String&gt; | 设置区域填充的颜色 （Hex rgba)
<br/>

### - tag
设置柱标签
#### iOS
```obj-c
  chart.interval().position(@"date*value").tag(
        @{
          @"fill": @"#999999",
          @"textAlign":@"center",
          @"textBaseline":@"bottom"
          @"textSize": @(10),
          @"offset":@(-12)
        }
  )
;
```
#### Android
```java
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `fill`| String | 标签颜色 （Hex rgb）
| `textAlign`| String | 对齐方式 <br/>`center`(默认)居中对齐  <br/>`start` 顶部对齐 <br/>`end` 底部对齐 <br/>`left` 左对齐 <br/>`right` 右对齐
| `textBaseline`| String | 对齐基准线 <br/>`alphabetic`(默认)  <br/>`top`  <br/>`hanging` <br/>`middle` <br/>`ideographic` <br/>`bottom`
| `textSize`| Number | 文字大小
| `offset`| Number | y 方向和柱距离的偏移
