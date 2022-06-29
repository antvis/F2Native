---
title: measure
order: 3
---

The metric Scale is the conversion bridge from data space to graphics space, and is responsible for the mutual conversion of original data to values ​​in the [0, 1] interval. There are different types of metrics for different data types.

Depending on the type of data, F2Native supports the following measurement types:

- **identity**, a value of constant type, that is to say, a field of the data is an invariant constant;

- **linear**, consecutive numbers [1, 2, 3, 4, 5];

- **cat**, category, ['male', 'female'];

- **timeCat**, time type;


## Common Properties

````
chart.scale('fieldName', {
  // each property configuration
});
````

Listed below are the generic properties:

| **Property Name** | **Type** | **Explanation** | **Default Value**
| --- | --- | --- | --- |
| `type`| String | metric type <br/> `identity` <br/> `linear` <br/> `cat` <br/> `timeCat` | "identity"
| `precision`| Number | precision | -
| `range`| Array&lt;Number&gt; | Defines the range of metric values<br/> range[0] - defines the minimum value of the metric's value range<br/> range[1] - defines the maximum value of the metric's value range| Calculated according to actual data
| `tickCount`| Number | Defines the number of ticks | -
| `max`| Number | Defines the maximum value of the value | -
| `min`| Number | Defines the minimum value of the value | -


#### Code Example

#### iOS
````obj-c
  F2CanvasView *f2CanvasView = [F2CanvasView canvas:CGRectMake(0, 0, 280, 280)];
  F2Chart *chart = [F2Chart chart:f2CanvasView.bounds.size name:@"F2chart"];
 chart.scale(@"date", @{@"type":@"identity" });
 chart.scale(@"value", @{ @"precision": @(1), @"max": @(200), @"min": @(0)});
````

#### Android
````java
mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(3).range(new double[]{0.1, 0.9}));
mChart.setScale("value", new F2Chart.ScaleConfigBuilder().precision(1).max(200).min(0));
````