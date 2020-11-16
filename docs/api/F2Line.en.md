---
title: F2Line
order: 5
---

图形线对象，用于绘制线图

## 创建 F2Line 实例
#### iOS
```obj-c
  F2CanvasView *f2CanvasView = [F2CanvasView canvasWithFrame:CGRectMake(0, 0, 280, 280)];
  F2Chart *chart = [F2Chart chart:f2CanvasView.bounds.size withName:@"F2chart"];
  chart.line();
```

#### Android
```java
  chart.line();
```
<br/>

## 方法
### - position 
绘制柱图位置
#### iOS
```obj-c
  chart.line().position(@"date*value");
```
#### Android
```java
chart.line().position("date*value");
```
- 参数

| **参数名** |  **类型** | **解释** |
| --- | --- | --- |
| `attr`| String | X*Y：绘制的柱在 source 中对应的名称, 此属性为必须.
<br/>

### - color
设置一组线的颜色
#### iOS
```obj-c
  chart.line().position(@"date*value").color(@"type"，@[ @"#1890FF", @"#2FC25B", @"#FACC14" ]);
```
#### Android
```java
  chart.line().position(@"date*value").color("type"，new String[]{"#1890FF", "#2FC25B", "#FACC14"});
```
- 参数

| **参数名** |  **类型** | **解释** |
| --- | --- | --- |
| `field`| String | 分组字段
| `color`| Array&lt;String&gt; | 一次设置不同 `field` 对应的色值, 可直接传空数组
<br/>

### - fixedColor
设置统一的颜色
#### iOS
```obj-c
  chart.line().position(@"date*value").fixedColor(@"#1890FF");
```
#### Android
```java
  chart.line().position("date*value").fixedColor("#1890FF");
```
- 参数

| **参数名** |  **类型** | **解释** |
| --- | --- | --- |
| `color`| String | 所有 `field` 对应统一的色值
<br/>

### - fixedSize
设置统一的线宽度  
#### iOS
```obj-c
  chart.line().position(@"date*value").fixedSize(1);
```
#### Android
```java
  chart.line().position("date*value").fixedSize(1);
```
- 参数

| **参数名** |  **类型** | **解释** |
| --- | --- | --- |
| `size`| float | 所有的 `field` 设置统一的线宽度 (dp)
<br>

### - size 
设置线宽度  
#### iOS
```obj-c
  chart.line().position(@"date*value").size(@"type", @[@(1), @(2), @(3)]);
```
#### Android
```java
  chart.line().position("").size("", new float[]{1, 2, 3});
```
- 参数

| **参数名** |  **类型** | **解释** |
| --- | --- | --- |
| `field`| String | 分组字段 
| `size`| Array&lt;float&gt;| 按 `field` 字段分组后分别对应的线宽值
<br>

### - fixedShape
设置所有线的统一类型
#### iOS
```obj-c
  chart.line().position(@"date*value").fixedShape(@"smooth");
```
#### Android
```java
  chart.line().position("date*value").fixedShape("smooth");
```
- 参数

| **参数名** |  **类型** | **解释** |
| --- | --- | --- |
| `shape`| String | `smooth` - 平滑曲线, `line` - 直线(默认值), `dash` - 虚线
<br>


### - shape 
设置各组线的类型
#### iOS
```obj-c
  chart.line().position(@"date*value").shape(@"type", @[@"smooth", @"smooth", @"smooth"]);
```
#### Android
```java
  chart.line().position("date*value").shape("type", new String[]{"smooth", "smooth", "smooth"});
```
- 参数

| **参数名** |  **类型** | **解释** |
| --- | --- | --- |
| `field`| String | 分组字段
| `shape`| Array&lt;String&gt; | `smooth` - 平滑曲线, `line` - 直线(默认值), `dash` - 虚线
<br>


