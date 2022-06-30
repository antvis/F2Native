---
title: F2Legend
order: 8
---


The generation of F2Native legends is determined by the graphics attributes in the graphics grammar. We will automatically generate different types of legends according to the graphics attribute mapping and the type of data: color, size, these two graphics attributes, if it is judged that the received parameters are the fields of the data source When , different legends will be automatically generated, or you can set them manually


## Method
### - chart.legend(filed,config)

#### iOS
````obj-c
   chart.legend(@"type", @{
                           @"enable": @(YES)},
                           @"position": @"top"},
                           @"radius": @(3),
                           @"symbol": @"square"},
                           );
}
````

#### Android
````java
// TODO
````

- parameters

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `filed`| String | Returns the name of the corresponding field to display the legend|
| `config`| Dictionary | Configuration Information |

-config

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `enable`| Boolean | Whether to enable the legend, the default is true
| `position`| String | The position of the legend<br/>`top` - top, default is horizontal<br/> `bottom` - bottom, default is horizontal<br/> `left` - left, default for vertical display<br/>`right` - right side, default is vertical display<br/>
| `layout`| String | Layout method <br/>`horizontal` - horizontal layout <br/> `vertical` - vertical layout
| `symbol`| String | Legend shape <br/> `circle` - circle <br/> `square` - square
| `radius`| Number | The side length or radius corresponding to the symbol, the default is 3px
| `lineBottom`| Number | Line spacing for horizontal layout
| `nameStyle` | Dictionary | Text style configuration
| `wordSpace`| Number | The distance between the graphics and the text, the default is 6px
| `itemMarginBottom`| Number | The distance between the legend and the chart, default is 12px
<br/>


- nameStyle

| **Attribute Name** | **Type** | **Explanation** |
| --- | --- | --- |
| `fill`| String | Text color
| `textSize` | Number | Text size
| `textAlign` | String | Alignment <br/> `start` - default <br/> `end` <br/> `center`
| `textBaseline` | String | Baseline position <br/> `top` - default <br/> `bottom` <br/> `middle`