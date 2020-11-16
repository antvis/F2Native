---
title: 度量
order: 3
---

度量 Scale，是数据空间到图形空间的转换桥梁，负责原始数据到 [0, 1] 区间数值的相互转换工作。针对不同的数据类型对应不同类型的度量。

根据数据的类型，F2Native 支持以下几种度量类型：

- **identity**，常量类型的数值，也就是说数据的某个字段是不变的常量；

- **linear**，连续的数字 [1, 2, 3, 4, 5]；

- **cat**，分类, ['男','女']；

- **timeCat**，时间类型；


## 通用属性

```
chart.scale('fieldName', {
  // 各个属性配置
});
```

下面列出的是通用的属性：

| **属性名** |  **类型** | **解释** | **默认值**
| --- | --- | --- | --- | 
| `type`| String | 度量类型 <br/> `identity` <br/> `linear` <br/> `cat` <br/> `timeCat` | "identity"
| `precision`| Number | 精度 | -
| `range`| Array&lt;Number&gt; | 定义度量取值范围 <br/> range[0] - 定义度量取值范围的最小值 <br/> range[1] - 定义度量取值范围的最大值| 按实际数据计算
| `tickCount`| Number | 定义刻度数量 | -
| `max`| Number | 定义数值的最大值 | -
| `min`| Number | 定义数值的最小值 | -


#### 代码示例

#### iOS
```obj-c
  F2CanvasView *f2CanvasView = [F2CanvasView canvasWithFrame:CGRectMake(0, 0, 280, 280)];
  F2Chart *chart = [F2Chart chart:f2CanvasView.bounds.size withName:@"F2chart"];
 chart.scale(@"date", @{@"type":@"identity" });
 chart.scale(@"value", @{ @"precision": @(1), @"max": @(200), @"min": @(0)});
```

#### Android
```java
mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(3).range(new double[]{0.1, 0.9}));
mChart.setScale("value", new F2Chart.ScaleConfigBuilder().precision(1).max(200).min(0));
```

