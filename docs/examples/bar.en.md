---
title: Bar Chart
order: 0
---

## Example

### Data（jsonString）
Prepare jsonData like this

```json
[{"year": "1951 年","sales": 38}, {"year": "1952 年","sales": 52}, 
 {"year": "1956 年","sales": 61}, {"year": "1957 年","sales": 145}, 
 {"year": "1958 年","sales": 48}, {"year": "1959 年","sales": 38}, 
 {"year": "1960 年","sales": 38}, { "year": "1962 年","sales": 38}]
```

### iOS
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
            <td width= 55%>
            F2CanvasView *canvasView =[F2CanvasView canvas:self.frame];<br/>
             F2Chart *chart = [F2Chart chart:canvasView.bounds.size name:NSStringFromClass([self class])];<br/>
             chart.coord(@{@"transposed":@(YES)});<br/>
             chart.scale(@"sales",@{@"min":@(0)});<br/>
             chart.scale(@"year", @{ @"range": @[@(0.1), @(0.9)] });<br/>
             chart.interval().position(@"year*sales");<br/>
             chart.render();<br/>
            </td>
            <td>
               <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*TwwKTJZTPdoAAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" height = "369"/>
            </td>
        </tr>
    </table>
</html>


### Android
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
            <td width= 55%>
              mChart.setCanvas(canvasView);<br/>
              mChart.padding(10, 0, 0, 0);<br/>
              mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleIntervalChart.json"));<br/>
              mChart.interval().position("year*sales");<br/>
              mChart.setAxis("year", new F2Chart.AxisConfigBuilder()<br/>
                      .label(new F2Chart.AxisLabelConfigBuilder()<br/>
                      .labelOffset(5.f)).gridHidden());<br/>
              mChart.setCoord(new F2Chart.CoordConfigBuilder()<br/>
              .type("rect").transposed(true));<br/>
              mChart.setAxis("sales", new F2Chart.AxisConfigBuilder()<br/>
                      .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5))<br/>
                      .grid(new F2Chart.AxisGridConfigBuilder().type("dash"))<br/>
                      .line(new F2Chart.AxisLineConfigBuilder()<br/>
                      .lineWidth(1).color("#E6E6E6").type("dash")));<br/>
              mChart.setAxis("", new F2Chart.AxisConfigBuilder().hidden(true));<br/>
              mChart.setScale("year", new F2Chart.ScaleConfigBuilder().range(new double[]{0.1, 0.9}));<br/>
              mChart.render();<br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*P3sqTLH9nO4AAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width="346"/>
            </td>
        </tr>
    </table>
</html>
