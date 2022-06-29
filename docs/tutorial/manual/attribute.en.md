---
title: Graphic Properties
order: 5
---

Graphical attributes correspond to visual channels in visual coding, and are a very important and flexible part of the graphic grammar elements. Each geometric mark has its own graphic attributes. The graphics properties supported in F2Native are as follows:

1. **position**: position, mapped to the x-axis and y-axis in the two-dimensional coordinate system;

2. **color**: color, including hue, saturation and brightness;

3. **size**: size, different geometric markers have different definitions of size;

4. **shape**: Shape, the shape of the geometric mark determines the representation of a specific chart type, for example, a line diagram can have a polyline, a curve, a dotted line, etc.;


## How To Use

First of all, it needs to be clear: **Graphic attributes belong to each geometry tag geom (Geometry)**, so we must declare the geometry tag first, and then map the graphic attributes on the geometry tag object. The code is as follows:

````
chart.<geomType>().<attrType>(fields,config);
````

in:

- geomType, geometry marker type, please read the [Geometry Marker](./geometry) chapter for specific supported types;

- attrType, graphic attribute type, corresponding to the visual channel;

- fields, the fields that participate in the mapping of a single visual channel, which can be a single field or multiple fields, and multiple fields are separated by `*`

- config, configuration information may be string or array


Syntax example:

````
chart.line().position(@"a*b").fixedColor(@"#ffffff");
chart.interval().adjust(@"dodge")
                .position(@"month*value")
                .color(@"name", @[@"#ffffff",@"#1890FF"]);

````

F2Native's parsing rules for the parameter `fields` of each graphics property are as follows:

- If it is a single word, such as `color(@"a")`, it will judge whether the attribute is a field attribute of the input data source, if not, it will be parsed as a constant;

- If it is a mapping of multiple attributes, you need to use `*` to connect, F2Native will parse and map these fields in turn, such as `position(@"cut*price")`;


<a name="4933d1a9"></a>
## position position property

The mapping of the position attribute is used to determine which fields in the data determine the position of the data in the plane coordinate system. In layman's terms, the data fields that determine the x-axis and y-axis.

Taking the following statement as an example, on the position attribute, two attributes are mapped: 'cut' and 'price', which respectively indicate that the 'cut' data value is mapped to the x-axis coordinate point, and the 'price' data value is mapped to the y-axis. Coordinate points.

````
chart.line().position(@"cut*price");
````

<a name="color"></a>
##color

From the perspective of visual coding, color can be divided into three visual channels: brightness, saturation and hue. The first two can be considered as visual channels for encoding quantitative and ordinal data, while hue belongs to encoding qualitative Visual channel for data. In F2Native, the distinction is not so detailed, and the color method is uniformly used for mapping configuration.

The mapping syntax supported by color is as follows:

- `fixedColor(color)` color is the set color value, this method sets the color attribute for the graphics of a single type of data source that does not involve classification;

- `color('field', colors)`, which maps data values ​​to the specified color values ​​colors (which are arrays), which are usually used to map categorical data;



### Colormap For Categorical Data

Map the data value of the `city` property to the specified color to distinguish different cities.

`.color(@"city", @[ @"#40A9FF", @"#FF7875", @"#FFC069" ])`

![](https://gw.alipayobjects.com/zos/rmsportal/FPXEtZTmKGVleSoVTDSL.png#align=left&display=inline&height=260&originHeight=500&originWidth=720&status=done&width=375)


## Shape

Different geometric markers have different shapes. shape This visual channel is affected by several other visual channels, such as: line shape can be configured as `smooth`-smooth curve `dash`-dashed line .

- `fixedShape(shape)`, this method sets shape attributes for graphics of a single type of data source that do not involve classification;

- `shape('field', shapes)`, which maps the data value to the specified shape shapes (which is an array), which is used to map categorical data in general;



## Size

The meanings of different geometric markers are not exactly the same:

- For line lines, size corresponds to the thickness of the line;

- For interval histograms, size corresponds to the width of the bars.


So from the perspective of visualization, size is a complex visual channel.

In F2Native, the following mapping syntaxes are supported:

- `fixedSize(size)`, this method sets the size attribute for graphics of a single type of data source that does not involve classification;

- `size('field', sizes)`, which maps data values ​​to the specified size sizes (which are arrays), which is used to map categorical data in general;


## Geom Supported Graphics Properties

As mentioned earlier, the visual channels supported by each geometric marker are different, and the mapping relationship between data and visual channels is not exactly the same. The following table lists the support of each graph attribute for each geom geometry tag:

| **geometry mark** | **position** | **color** | **size** | **shape** |
| --- | --- | --- | --- | --- |
| line | support | support | support | support |
| area | Supported | Supported | `Not supported` | `Not supported` |
| interval | supported | supported | supported | `not supported` |
| point | support | support | support | support |