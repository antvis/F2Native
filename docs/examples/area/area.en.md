---
title: Basic Area Chart
order: 0
---

## Example


### Data（jsonString）
Prepare jsonData

```json
[{"time": "Jan.","tem": 1000}, {"time": "Feb.","tem": 2200}, 
 {"time": "Mar.","tem": 2000}, {"time": "Apr.","tem": 2600}, 
 {"time": "May.","tem": 2000}, {"time": "Jun.", "tem": 2600},
 {"time": "Jul.","tem": 2800}, {"time": "Aug.","tem": 2000}]
```


### iOS
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
            <td width= 55%>
             F2CanvasView *canvasView =[F2CanvasView canvas:self.frame];<br/>
             canvasView.delegate = self;<br/>
             F2Chart *chart = [F2Chart chart:canvasView.bounds.size name:NSStringFromClass([self class])];<br/>
             chart.canvas(canvasView).padding(20, 10, 20, 0.f).source(jsonData); <br/>
             chart.scale(@"tem", @{@"min": @(0)});<br/>
             chart.axis(@"time", @{@"grid": @(NO), @"label": @{@"textAlign": @"start"}});<br/>
             chart.line().position(@"time*tem").fixedSize(2);<br/>
             chart.area().position(@"time*tem");<br/>
             chart.render(); <br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*GppGQKVTQFwAAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" height = "369" />
            </td>
        </tr>
    </table>
</html>


### Android
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
            <td  width = 55%>
             mChart.setCanvas(canvasView);<br/>
             mChart.padding(10, 0, 0, 0);<br/>
             mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleAreaChart.json"));<br/>
             mChart.setAxis("time", new F2Chart.AxisConfigBuilder() <br/>
                     .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f))
                     .gridHidden());<br/>
             mChart.setAxis("tem", new F2Chart.AxisConfigBuilder() <br/>
                     .grid(new F2Chart.AxisGridConfigBuilder().type("dash")));<br/>
             mChart.setScale("tem", new F2Chart.ScaleConfigBuilder().min(0));<br/>
             mChart.line().position("time*tem");<br/>
             mChart.area().position("time*tem");<br/>
             mChart.render();<br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*g09TSb9u_FMAAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346"/>
            </td>
        </tr>
    </table>
</html>
