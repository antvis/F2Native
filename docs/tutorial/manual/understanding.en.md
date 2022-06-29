---
title: Chart Composition
order: 1
---

In order to better use F2Native for data visualization, we need to understand the composition of F2Native charts and related terminology.

## Chart Widgets

In general, F2Native's charts include Axis, Geometry, Tooltip, Legend, etc. In addition, it can also include auxiliary markers (Guide), data labels (dataLabels), etc.

The basic components of F2Native are shown in the following figure:

![](https://gw.alipayobjects.com/zos/rmsportal/tpfdzWDYmxzHkquTihJe.png#width=600) ![](https://gw.alipayobjects.com/zos/rmsportal/lUqXwLjgRWhugemcNsqc.png#width= 600)

## The Term
| **Terminology** | **English** | **Description** |
| --- | --- | --- |
| Coordinate axis | Axis | Each chart usually contains two coordinate axes, under the Cartesian coordinate system (Cartesian coordinate system), the x-axis and the y-axis, respectively, under the polar coordinate axis, the angle and the radius are respectively 2 dimensional composition. Each axis consists of a line, a tickLine, a label, and a grid. |
| Legend | Legend | Legend is an auxiliary element of the chart, used to demarcate different data types and data ranges, to assist in reading the chart and to help users filter and filter data in the chart. |
| Geometry Marker | Geometry | That is, the geometric figures we call point, line, and surface. In the graphics grammar, the type of geometric mark determines the type of graph generated. That is, the actual performance of the data after being visualized. Different geometric markers contain corresponding graphical attributes. |
| Graphic attributes | Attribute | Graphic attributes correspond to visual channels in visual coding, and are a very important and flexible part of graphic syntax elements. Different geometric markers have their own graphic attributes. F2Native provides position, color, Four graphic attributes: size and shape. |
| Coordinate System | Coordinate | The coordinate system is a 2-dimensional positioning system that combines two position scales, describing how data is mapped to the plane on which the graph is located. |
| Prompt information | Tooltip | When the mouse hovers over a point, the information of the data corresponding to the current point will be displayed in the form of a prompt box, such as the value of the point, data unit, etc., to help users obtain specific information from the chart Data information. |
| Guide | Guide | Guide is a very useful tool when you need to draw some guide lines, guide boxes or text on the chart, such as adding warning lines, maximum value lines, or marking obvious range areas. |