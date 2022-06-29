---
title: F2Guide
order: 7
---

Graphical helper object for dimensioning at specific points

## Create F2Guide Instance
#### iOS
````obj-c
  chart.guide();
````

#### Android
````java
  chart.guide();
````
<br/>

## method
### - flag
Drawing flag auxiliary graphics
#### iOS
````obj-c
  chart.guide().flag(
                     @{
                        @"color":@"#108EE9",
                        @"position": @[@(1), @(99.9)],
                        @"content": @"flagDemo",
                        @"padding": @[@(5), @(3), @(5), @(3)] ,
                        @"backgroundColor":@"#C7E7FF",
                        @"textColor":@"#108EE9",
                        @"textSize": @(10.f)
                      });
````
#### Android
````java
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `color`| String | The color of the flag auxiliary flag (Hex rgb)
| `position`| Array&lt;Number&gt; | The position where the auxiliary mark is drawn <br/>padding[0] indicates the position of the X-axis <br/> padding[1] indicates the position of the Y-axis
| `padding`| Array&lt;Number&gt; | The position where the auxiliary logo is drawn<br/>padding[0] left<br/> padding[1] top<br/>padding[2] right<br/> padding[3] Down
| `backgroundColor`| String | The background color of the flag auxiliary logo (Hex rgb)
| `textColor`| String | Text color
| `textSize` | Number | Text size
<br/>

### - text
Drawing text auxiliary graphics
#### iOS
````obj-c
chart.guide().text(
               @{
                 @"position": @[@(1), @(99.9)],
                 @"content": @"flagDemo",
                 @"padding": @[@(5), @(3), @(5), @(3)] ,
                 @"textColor":@"#108EE9",
                 @"textSize": @(10.f)
                 });
````

#### Android
````java
````

- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `position`| Array&lt;Number&gt; | The position where the auxiliary mark is drawn <br/>padding[0] indicates the position of the X-axis <br/> padding[1] indicates the position of the Y-axis
| `padding`| Array&lt;Number&gt; | The position where the auxiliary logo is drawn<br/>padding[0] left<br/> padding[1] top<br/>padding[2] right<br/> padding[3] Down
| `textColor`| String | Text color
| `textSize` | Number | Text size
<br/>