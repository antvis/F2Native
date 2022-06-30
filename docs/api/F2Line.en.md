---
title: F2Line
order: 5
---

Graphic line object, used to draw line graphs

## Create F2Line Instance
#### iOS
````obj-c
  F2CanvasView *f2CanvasView = [F2CanvasView canvas:CGRectMake(0, 0, 280, 280)];
  F2Chart *chart = [F2Chart chart:f2CanvasView.bounds.size name:@"F2chart"];
  chart.line();
````

#### Android
````java
  chart.line();
````
<br/>

## Method
### - position
where to draw the column chart
#### iOS
````obj-c
  chart.line().position(@"date*value");
````
#### Android
````java
chart.line().position("date*value");
````
- parameters

| **Parameter name** | **Type** | **Explanation** |
| --- | --- | --- |
| `attr`| String | X*Y: The name of the drawn bar in source, this attribute is required.
<br/>

### - color
Set the color of a set of lines
#### iOS
````obj-c
  chart.line().position(@"date*value").color(@"type", @[ @"#1890FF", @"#2FC25B", @"#FACC14" ]);
````
#### Android
````java
  chart.line().position(@"date*value").color("type", new String[]{"#1890FF", "#2FC25B", "#FACC14"});
````
- parameters

| **Parameter name** | **Type** | **Explanation** |
| --- | --- | --- |
| `field`| String | grouping field
| `color`| Array&lt;String&gt; | Set the color value corresponding to different `field` at one time, you can directly pass an empty array
<br/>

### - fixedColor
set uniform color
#### iOS
````obj-c
  chart.line().position(@"date*value").fixedColor(@"#1890FF");
````
#### Android
````java
  chart.line().position("date*value").fixedColor("#1890FF");
````
- parameters

| **Parameter name** | **Type** | **Explanation** |
| --- | --- | --- |
| `color`| String | All `field` correspond to a uniform color value
<br/>

### - fixedSize
Set uniform line width
#### iOS
````obj-c
  chart.line().position(@"date*value").fixedSize(1);
````
#### Android
````java
  chart.line().position("date*value").fixedSize(1);
````
- parameters

| **Parameter name** | **Type** | **Explanation** |
| --- | --- | --- |
| `size`| float | All `field`s set a uniform line width (dp)
<br>

### - size
set line width
#### iOS
````obj-c
  chart.line().position(@"date*value").size(@"type", @[@(1), @(2), @(3)]);
````
#### Android
````java
  chart.line().position("").size("", new float[]{1, 2, 3});
````
- parameters

| **Parameter name** | **Type** | **Explanation** |
| --- | --- | --- |
| `field`| String | grouping field
| `size`| Array&lt;float&gt;| Corresponding line width values ​​after grouping by `field` field
<br>

### - fixedShape
Set uniform type for all lines
#### iOS
````obj-c
  chart.line().position(@"date*value").fixedShape(@"smooth");
````
#### Android
````java
  chart.line().position("date*value").fixedShape("smooth");
````
- parameters

| **Parameter name** | **Type** | **Explanation** |
| --- | --- | --- |
| `shape`| String | `smooth` - smooth curve, `line` - straight line (default), `dash` - dashed line
<br>


### - shape
Set the type of each group of lines
#### iOS
````obj-c
  chart.line().position(@"date*value").shape(@"type", @[@"smooth", @"smooth", @"smooth"]);
````
#### Android
````java
  chart.line().position("date*value").shape("type", new String[]{"smooth", "smooth", "smooth"});
````
- parameters

| **Parameter name** | **Type** | **Explanation** |
| --- | --- | --- |
| `field`| String | grouping field
| `shape`| Array&lt;String&gt; | `smooth` - smooth curve, `line` - straight line (default), `dash` - dashed line
<br>