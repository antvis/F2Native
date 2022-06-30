---
title: Data
order: 2
---

Data is the most basic part of drawing a chart. The data format supported by F2Native is jsonString:

```` json
[{"genre":"Sports", "sold":275},
 {"genre":"Strategy","sold":115},
 {"genre":"Action","sold":120},
 {"genre":"Shooter","sold":350},
 {"genre":"Other","sold":150}]
````


```` obj-c
  NSString *jsonString = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
  chart.source(jsonString);
````

## How to Load Data

After the chart instance is created, load the data by calling the following interface:

````
chart.source(data);
````

## How To Update Data


1. Update the chart data.

````
chart.source(data);
````

2. Call the redraw method when the chart needs to be updated.

````
chart.source(newData); // update the data source

// do something

chart.repaint(); // Update the chart!
````

3. When updating data, you can redefine the graphics syntax, change the chart type and various configurations.

````
chart.source(newData); // load new data
chart.interval().position('x*y').color('z'); // redefine the graph syntax
chart.repaint();
````

## Data Description For Special Charts

### Pie Chart

When drawing a pie chart, each record in the dataset must contain a constant field (and must be of type string) as follows:

````json
[
  { "name": "Youth", "percent": 0.4, "a": "1" },
  { "name": "The Legend of the Demon Cat", "percent": 0.2, "a": "1" },
  { "name": "Blood of the Machine", "percent": 0.18, "a": "1"},
  { "name": "Psychological Sin", "percent": 0.15, "a": "1" },
  { "name": "Dream Travel", "percent": 0.05, "a": "1" },
  { "name": "other", "percent": 0.02, "a": "1" }
]
````

See pie chart [example](/zh/examples/pie/basic).