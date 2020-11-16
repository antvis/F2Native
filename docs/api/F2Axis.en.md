---
title: F2Axis
order: 4
---

# F2Axis
坐标轴配置。F2Native 的坐标轴的组成如下:![](https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*jJ9SR7YqPVUAAAAAAAAAAAAAARQnAQ)

| **术语** | **英文** |
| --- | --- |
| 坐标轴文本 | label |
| 坐标轴线 | line |
| 坐标轴网格线 | grid |


## API
### - axis
设置坐标轴配置；F2Native 的坐标轴组成如下
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
chart.setAxis("sales", new F2Chart.AxisConfigBuilder()
      .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5))
      .grid(new F2Chart.AxisGridConfigBuilder().type("dash"))
      .line(new F2Chart.AxisLineConfigBuilder().lineWidth(1).color("#E6E6E6").type("dash").lineDash(new double[]{6, 3}));
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `field`| String | 当前坐标轴所代表的数据在 source 中的名称|
| `config`| Dictionary | 坐标轴的具体配置信息 见下面 config |

- config

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `line`| Bool/Dictionary  | false - 坐标轴线隐藏 <br/>  配置见下面 line 定义|
| `grid`| Bool/Dictionary | false - 坐标轴网格线隐藏 <br/> 配置见下面 grid 定义|
| `label`| Bool/Dictionary | false - 坐标轴网格线隐藏 <br/> 配置见下面 label 定义 

- line

| **属性名** |  **类型** | **解释** | **默认值**
| --- | --- | --- | --- | 
| `type`| String | 线类型| "line"/"dash"
| `color`| String | 线颜色 （Hex rgba）| "#999"
| `lineWidth`| float | 线宽度 | 1.0f

- grid

| **属性名** |  **类型** | **解释** | **默认值**
| --- | --- | --- | --- | 
| `type`| String | 网格线类型 <br/> `line`- 直线 <br/> `dash`- 虚线| "line"
| `stroke`| String | 网格线颜色 （Hex rgba）| "#999"
| `lineWidth`| float | 网格线宽度 | 1.0f
| `lineDash` | Array&lt;Number&gt;|虚线样式 | [2, 2]

- label

| **属性名** |  **类型** | **解释** | **默认值**
| --- | --- | --- | --- | 
| `textColor`| String | 字体颜色 （Hex rgba）| "#999"
| `textAlign`| String | 对齐方式  | "start"
| `lineDash` | Array&lt;Number&gt;|虚线样式 | [2, 2]

<br/>