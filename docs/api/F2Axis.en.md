---
title: F2Axis
order: 4
---

# F2Axis
Axes configuration. The composition of the coordinate axes of F2Native is as follows: ![](https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*jJ9SR7YqPVUAAAAAAAAAAAAAARQnAQ)

| **Terminology** | **English** |
| --- | --- |
| Axis text | label |
| Coordinate axis | line |
| Axis gridlines | grid |


## API
### - Axis
Set the coordinate axis configuration; the coordinate axis composition of F2Native is as follows
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
chart.setAxis("sales", new F2Chart.AxisConfigBuilder()
      .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5))
      .grid(new F2Chart.AxisGridConfigBuilder().type("dash"))
      .line(new F2Chart.AxisLineConfigBuilder().lineWidth(1).color("#E6E6E6").type("dash").lineDash(new double[]{6, 3}));
````
- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `field`| String | The name of the data represented by the current axis in source|
| `config`| Dictionary | For the specific configuration information of the coordinate axis, see below config |

-config

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `line`| Bool/Dictionary | false - axis hidden <br/> configuration see line definition below |
| `grid`| Bool/Dictionary | false - axis grid lines are hidden <br/> configuration see grid definition below |
| `label`| Bool/Dictionary | false - axis gridlines are hidden <br/> configuration see label definition below

- line

| **Property Name** | **Type** | **Explanation** | **Default Value**
| --- | --- | --- | --- |
| `type`| String | line type | "line"/"dash"
| `color`| String | line color (Hex rgba) | "#999"
| `lineWidth`| float | line width | 1.0f

- grid

| **Property Name** | **Type** | **Explanation** | **Default Value**
| --- | --- | --- | --- |
| `type`| String | grid line type <br/> `line`- line <br/> `dash`- dashed line | "line"
| `stroke`| String | grid line color (Hex rgba) | "#999"
| `lineWidth`| float | grid line width | 1.0f
| `lineDash` | Array&lt;Number&gt;|dash style | [2, 2]

- label

| **Property Name** | **Type** | **Explanation** | **Default Value**
| --- | --- | --- | --- |
| `textColor`| String | font color (Hex rgba) | "#999"
| `textAlign`| String | Alignment | "start"
| `lineDash` | Array&lt;Number&gt;|dash style | [2, 2]

<br/>