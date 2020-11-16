---
title: F2Legend
order: 8
---


F2Native 图例的生成是由图形语法中的图形属性决定的，我们会根据图形属性映射以及数据的类型自动生成不同类型的图例：color, size 这两个图形属性如果判断接收的参数是数据源的字段时，会自动生成不同的图例,也可手动进行设置


## 方法
### - chart.legend(filed,config)

#### iOS
```obj-c
   chart.legend(@"field", @{
                           @"enable": @(YES)},
                           @"position": @"top"},
                           @"radius": @(3),
                           @"symbol": @"square"},
                           );
}
```

#### Android
```java
chart.legend("field", new F2Chart.LegendConfigBuild().enable(true).position("top").symbol("circle").setOption("radius", 3));
```

- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `filed`| Stirng | 返回需要展示图例的对应字段名称|
| `config`| Dictionary | 配置信息 |

- config

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `enable`| Boolean | 是否启用图例，默认为 true
| `position`| String | 图例的位置 <br/>`top` - 顶部，默认为水平展示  <br/> `bottom` - 底部，默认为水平展示 <br/> `left` - 左侧，默认为垂直展示 <br/>`right` - 右侧，默认为垂直展示 <br/>
| `layout`| String |  布局方式 <br/>`horizontal` - 水平布局  <br/> `vertical` - 垂直布局
| `symbol`| String |  图例形状 <br/> `circle` - 圆形  <br/> `square` - 方形
| `radius`| Number | symbol 对应的边长或半径 ，默认为3px
| `lineBottom`| Number | 水平布局时的行间距
| `nameStyle` | Dictionary | 文字样式配置
| `wordSpace`| Number | 图形与文字之间的距离，默认为 6px
| `itemMarginBottom`| Number | 图例与图表之间的距离，默认为 12px
<br/>


- nameStyle

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `fill`| String | 文字颜色
| `textSize` | Number | 文字大小
| `textAlign` | String | 对齐方式 <br/> `start` - 默认值 <br/> `end` <br/>  `center`
| `textBaseline` | String | 基准线位置 <br/> `top` - 默认值 <br/> `bottom` <br/> `middle`