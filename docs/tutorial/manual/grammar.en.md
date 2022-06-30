---
title: Grammar of Graphics
order: 0
---

F2Native, like G2, is based on the graphics theory presented in the book "The Grammar of Graphics" by Leland Wilkinson. The theory is a set of grammatical rules used to describe the deep properties of all statistical graphics. The grammar answers the question "what is a statistical graphics", organizing the most basic elements to form higher-level elements in a bottom-up manner.

So for F2Native, there is no specific concept of chart type, all charts are formed by combining different graphic syntax elements, these graphic syntax elements include:

- The most basic part is the [data](./data) you want to visualize and a series of mappings that map the variables in the data to the [graph attributes](./attribute);

- [Geometry tag](./geometry), which can be understood as the graphic elements you actually see in the chart, such as points, lines, polygons, etc. Each geometry tag object contains multiple graphic attributes. The core of F2Native graphics syntax is Establish the mapping of a series of variables in the data to the graph attributes;

- [metrics](./scale), as a conversion bridge from data space to graphic attribute space, each graphic attribute corresponds to one or more metrics;

- [Coordinate system](./coordinate), which describes how the data is mapped to the plane where the graphic is located. A geometric marker will behave differently in different coordinate systems. At present, F2Native provides two coordinate systems: Cartesian coordinate system and polar coordinate system;

- Auxiliary elements are to enhance the readability and comprehensibility of the chart. The auxiliary elements in F2Native include Axis, Legend Legend, Tooltip for tooltip, and Guide for static auxiliary markers.


Therefore, in F2Native, we usually describe a graph as follows: a graph is a mapping from data to the graphical properties of geometric marker objects, in addition, the graph may also contain statistical transformations of the data, and finally drawn at a specific coordinate in the department.

For more information, please read the following chapters:

- [graph composition](./understanding)

- [data](./data)

- [metric](./scale)

- [geometry tag](./geometry)

- [graphic attributes](./attribute)

- [coordinate system](./coordinate)