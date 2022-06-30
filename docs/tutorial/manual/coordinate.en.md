---
title: Coordinate System
order: 6
---

A coordinate system is a 2-dimensional positioning system that combines two position scales, describing how data is mapped to the plane on which the graphics lie.

F2Native provides two types of Cartesian coordinate system and polar coordinate. Currently, all coordinate systems are 2-dimensional.

## How To Set The Coordinate System

The coordinate system type provided by F2Native by default is Cartesian coordinate system. When you need to switch the coordinate system, you can declare the coordinate system to be used by calling the following syntax:

````
chart.coord(@{@"type": @"polar", @"transposed": @(YES)});
});
````
| **Coordinate System Type** | **Description** |
| --- | --- |
| `rect` | Cartesian coordinate system, currently only supports two-dimensional, consisting of two mutually perpendicular coordinate axes x, y. |
| `polar` | Polar coordinate system, consisting of 2 dimensions, angle and radius. |


## Coordinate System Type And Configuration

The coordinate system can be divided into a Cartesian coordinate system and a non-Cartesian coordinate system. The non-Cartesian coordinate system is polar coordinates, and the position is determined by the two dimensions of angle and radius.

Using polar coordinates can generate pie charts, rose charts, and radar charts, which are more suitable for visualization scenarios of periodic data, such as time and direction data.

A coordinate system type transformation will change the shape of a geometric marker, for example in a polar coordinate system a rectangle will become part of a torus.

### Coordinate System Configuration

#### Cartesian Coordinate System

````
// declare Cartesian coordinate system
// Cartesian coordinate system transpose
chart.coord(@{@"type": @"rect", @"transposed": @(YES)});

````

#### Polar Coordinates

````
chart.coord(@{
  @"type": @"polar", // declare polar coordinate system
  @"transposed": @(YES) // transposed
  });
````

It should be noted here that the default start and end angles of F2Native polar coordinates are shown in the following figure:

![](https://zos.alipayobjects.com/skylark/85950a42-9579-44cb-b656-8dd28c9a014a/attach/2378/d648679184c6977c/image.png#width=)