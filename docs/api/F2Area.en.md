---
title: F2Area
order: 5
---

Graphics area object, used to draw area extents

## Create F2Area Instance
#### iOS
````obj-c
   chart.area();
````

#### Android
````java
   chart.area();
````
<br/>

## Method
### - Position
draw area location
#### iOS
````obj-c
   chart.area().position(@"date*value");
````
#### Android
````java
   chart.area().position("date*value");
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `attr`| String | X*Y: The corresponding name of the drawn area in the source

<br/>

### - FixedColor
Set the color of the uniform area fill
#### iOS
````obj-c
   chart.area().fixedColor(@"#108EE9");
````
#### Android
````java
   chart.area().fixedColor("#108EE9");
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `attr`| String | Set the color of the area fill (Hex rgba)