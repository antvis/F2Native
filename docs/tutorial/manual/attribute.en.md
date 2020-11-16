---
title: 图形属性
order: 5
---

图形属性对应视觉编码中的视觉通道，是图形语法元素中非常重要和灵活的一部分，每种几何标记都拥有自己的图形属性。F2Native 中支持的图形属性有下面几种：

1. **position**：位置，二维坐标系内映射至 x 轴、y 轴；

2. **color**：颜色，包含了色调、饱和度和亮度；

3. **size**：大小，不同的几何标记对大小的定义有差异；

4. **shape**：形状，几何标记的形状决定了某个具体图表类型的表现形式，例如线图可以有折线、曲线、点线等表现形式；


## 如何使用

首先需要明确一点：**图形属性是属于每一个几何标记 geom（Geometry) 的**，所以我们先要声明几何标记，然后再在该几何标记对象上进行图形属性的映射，代码如下：

```
chart.<geomType>().<attrType>(fields,config);
```

其中：

- geomType，几何标记类型，具体支持的类型请阅读[几何标记](./geometry)章节；

- attrType，图形属性类型，对应视觉通道；

- fields，参与单个视觉通道映射的字段，可以是单个字段也可以是多个字段，多个字段使用 `*`分割

- config， 配置信息可能为 string 或者 array


语法示例：

```
chart.line().position(@"a*b").fixedColor(@"#ffffff");
chart.interval().adjust(@"dodge")
                .position(@"month*value")
                .color(@"name", @[@"#ffffff",@"#1890FF"]);

```

F2Native 对于每个图形属性的参数 `fields` 的解析规则如下：

- 如果是单个单词，如 `color(@"a")` 会判断该属性是否是输入数据源的字段属性，如果不是则会将其解析为一个常量；

- 如果是多个属性的映射，需要使用 `*` 进行连接，F2Native 会依次对这些字段进行解析和映射，如 `position(@"cut*price")`；


<a name="4933d1a9"></a>
## position 位置属性

position 位置属性的映射，用于确定由数据中的哪几个字段来确定数据在平面坐标系的位置。通俗地解释，即确定 x 轴和 y 轴的数据字段。

以下面的语句为例，在 position 属性上，映射了两个属性： 'cut' 和 'price'，分别表示将 'cut' 数据值映射至 x 轴坐标点，'price' 数据值映射至 y 轴坐标点。

```
chart.line().position(@"cut*price");
```

<a name="color"></a>
## color

从可视化编码的角度对颜色进行分析，可以将颜色分为亮度、饱和度和色调三个视觉通道，其中前两个可以认为是用于编码定量和定序数据的视觉通道，而色调属于编码定性数据的视觉通道。而在 F2Native 中并不如此详细区分，统一使用 color 方法进行映射配置。

color 支持的映射语法如下：

- `fixedColor(color)` color 为设置的色值，此方法给不涉及分类的单一类型数据源的图形进行颜色属性设置；

- `color('field', colors)`，将数据值映射至指定的颜色值 colors（是数组），此时用于通常映射分类数据；



### 分类数据的颜色映射

将 `city` 属性的数据值映射至制定的颜色来区分不同的城市。

`.color(@"city", @[ @"#40A9FF", @"#FF7875", @"#FFC069" ])`

![](https://gw.alipayobjects.com/zos/rmsportal/FPXEtZTmKGVleSoVTDSL.png#align=left&display=inline&height=260&originHeight=500&originWidth=720&status=done&width=375)


## shape

不同的几何标记有不同的 shape（图形形状）。shape 这个视觉通道受其他几个视觉通道影响，比如：line shape可以配置成`smooth`-平滑曲线  `dash`- 虚线 。

- `fixedShape(shape)`，此方法给不涉及分类的单一类型数据源的图形进行形状属性设置；

- `shape('field', shapes)`，将数据值映射至指定的形状 shapes（是数组），此时用于通常映射分类数据；



## size

对于不同的几何标记含义不完全一致：

- 对于 line 线来说，size 对应着线的粗细；

- 对于 interval 柱状图来说，size 对应着柱子的宽度。


所以从可视化的角度分析，大小（size)是一个复杂的视觉通道。

在 F2Native 中，支持如下几种方式的映射语法：

- `fixedSize(size)`，此方法给不涉及分类的单一类型数据源的图形进行size属性设置；

- `size('field', sizes)`，将数据值映射至指定的size sizes（是数组），此时用于通常映射分类数据；


## Geom 支持的图形属性

前面提到过，每种几何标记支持的视觉通道有所差异，数据和视觉通道的映射关系也不完全相同。 下表列出了各个 geom 几何标记对各个图形属性的支持情况：

| **几何标记** | **position** | **color** | **size** | **shape** |
| --- | --- | --- | --- | --- |
| line | 支持 | 支持 | 支持 | 支持 |
| area | 支持 | 支持 | `不支持` | `不支持` |
| interval | 支持 | 支持 | 支持 | `不支持` |
