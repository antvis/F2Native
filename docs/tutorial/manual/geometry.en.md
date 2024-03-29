---
title: Geometry Markers
order: 4
---

Geometric markers are what we call geometric figures such as points, lines, and surfaces. F2Native does not have the concept of specific chart types (bar charts, scatter charts, line charts, etc.), but through the graphical syntax, users can draw a certain type of chart, such as a pie chart, or a mixed chart, such as a line. A combination of graph and bar chart.

The types of graphs generated by F2Native are determined by geometric markers. You can intuitively understand what a geometric marker is through the following figure:

![](https://gw.alipayobjects.com/zos/rmsportal/ffXoDNzwnXNHoaxtjbfY.png#width=)

## How to Declare Geometry Tags

After creating the chart object, you can select the type of geometry markup as follows:

````
// This uses a geom of type interval, which returns a geom object
F2Interval *interval = self.chart.interval();
````

## Geometry Tag Type

The types of geometry tags currently supported by F2Native are as follows:

| **Type** | **Description** |
| --- | --- |
| line | Line, the points are connected to form a line according to the x-axis to form a line graph. |
| area | The area map is formed between the filled line graph and the coordinate system, and the upper and lower ranges can also be specified. |
| interval | Use a rectangle or an arc, and use the area to represent a graph of the size relationship, which generally constitutes a bar chart, a pie chart and other charts. |
| point | Use a circle to mark each item of data, generally forming a scatter chart. |


## Geometry Markers And Chart Types

Although F2Native does not have a specific chart type concept, **but still supports drawing of all traditional chart types**.

The following table shows the correspondence between geom geometric marker types in F2Native and traditional charts. For more charts, see [demos](/zh/examples).

| **Geom Type** | **Chart Type** |
| --- | --- | --- |
| line | Line graph, curve graph |
| area | Area Chart (Area Chart), Stacked Area Chart, Percent Stacked Area Chart, Interval Area Chart |
| interval | Column chart, grouped column chart, waterfall chart, stacked column chart, percentage stacked column chart, histogram, Nightingale rose chart, pie chart, bar ring chart (Jade Que chart), funnel chart, etc. |
| piont | Scatter |


## Graphic shape (shape)

The following provides the built-in shape types provided by each geom in F2Native. The usage of shapes will be described in detail in the following chapters on graphics properties.

| **Geom Type** | **Shape Type** | **Interpretation** |
| --- | --- | --- |
| line | 'line', 'smooth', 'dash' | dash: dashed line, smooth: smooth line |
| area | 'area', 'smooth' | area map with filled content |
| interval | 'rect' | |
| point | 'circle' | |