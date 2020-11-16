---
title: 数据
order: 2
---

数据是绘制一张图表最基本的部分。F2Native 支持的数据格式为 jsonString：

``` json
[{"genre":"Sports", "sold":275},
 {"genre":"Strategy","sold":115},
 {"genre":"Action","sold":120},
 {"genre":"Shooter","sold":350},
 {"genre":"Other","sold":150}]
```

## 如何装载数据

当 chart 实例创建完毕之后，通过调用以下接口装载数据：

```
chart.source(data);
```

## 如何更新数据


1. 更新图表数据。

```
chart.source(data);
```

2.在需要更新图表的时机调用重绘方法。

```
chart.source(newData); // 更新数据源

// do something

chart.repaint();  // 更新图表！
```

3. 更新数据时还可以重新定义图形语法，改变图表类型和各种配置。

```
chart.source(newData); // 加载新数据
chart.interval().position('x*y').color('z'); // 重新定义图形语法组合
chart.repaint();
```

## 特殊图表的数据说明

### 饼图

绘制饼图时，数据集中的每一条记录中**必须包含一个常量字段（并且必须是字符串类型）**，如下所示：

```json
[
  { "name": "芳华", "percent": 0.4, "a": "1" },
  { "name": "妖猫传", "percent": 0.2, "a": "1" },
  { "name": "机器之血", "percent": 0.18, "a": "1"},
  { "name": "心理罪", "percent": 0.15, "a": "1" },
  { "name": "寻梦环游记", "percent": 0.05, "a": "1" },
  { "name": "其他", "percent": 0.02, "a": "1" }
]
```

详见饼图[示例](/zh/examples/pie/basic)。
