---
title: F2Interval
order: 6
---

Graphical column object, the user draws the column chart

## Create F2Interval instance
#### iOS
````obj-c
  F2CanvasView *f2CanvasView = [F2CanvasView CANVAS:CGRectMake(0, 0, 280, 280)];
  F2Chart *chart = [F2Chart chart:f2CanvasView.bounds.size name:@"F2chart"];
  chart.interval();
````

#### Android
````java
  chart.interval();
````
<br/>

## Method
### - position
where to draw the column chart
#### iOS
````obj-c
  chart.interval().position(@"date*value");
````
#### Android
````java
  chart.interval().position("date*value");
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `attr`| String | X*Y: The name of the drawn bar in source

<br/>

### - fixedColor
Set column fill color
#### iOS
````obj-c
  chart.interval().position(@"date*value").fixedColor(@"#108EE9");
````
#### Android
````java
  chart.interval().position("date*value").fixedColor("#108EE9");
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `color`| String | Set the color of the area fill (Hex rgb)
<br/>

### - color
Set the color of the grouped column fill
#### iOS
````obj-c
  chart.interval().position(@"date*value").color(@"type", @[@"#108EE9", @"#2FC25B"]);
````
#### Android
````java
  chart.interval().position("date*value").color("type", new String[]{"#108EE9", "#2FC25B"});
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `field`| String | Data grouping field
| `color`| Array&lt;String&gt; | Set the color of the area fill (Hex rgba)
<br/>

### - tag
Set column labels
#### iOS
````obj-c
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
````
#### Android
````java
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `fill`| String | Label color (Hex rgb)
| `textAlign`| String | Alignment<br/>`center`(default) center<br/>`start` top<br/>`end` bottom<br/>`left` left<br />`right` Align right
| `textBaseline`| String | Alignment baseline<br/>`alphabetic` (default) <br/>`top` <br/>`hanging` <br/>`middle` <br/>`ideographic` <br />`bottom`
| `textSize`| Number | Text size
| `offset`| Number | Offset in y direction and column distance