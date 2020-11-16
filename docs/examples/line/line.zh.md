---
title: 基础折线图
order: 0
icon: line
redirect_from:
  - /zh/docs/examples/
---


## 演示

### 数据（jsonString）

下述 jsonData 对应数据
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
            <td width="55%">
             F2CanvasView *canvasView =[F2CanvasView canvasWithFrame:self.frame];<br/>
             canvasView.delegate = self;<br/>
             F2Chart *chart = [F2Chart chart:canvasView.bounds.size withName:NSStringFromClass([self class])];<br/>
             chart.canvas(canvasView).padding(20, 10, 20, 0.f).source(jsonData);<br/>
             chart.scale(@"date", @{@"tickCount": @(5)});<br/>
             chart.scale(@"value", @{@"min": @(10), @"max": @(320)});<br/>
             chart.axis(@"date", @{@"grid": @(NO)});<br/>
             chart.line().position(@"date*value").fixedSize(2);<br/>
             chart.tooltip(@{});<br/>
             chart.interaction(@"pinch");<br/>
             chart.interaction(@"pan");<br/>
             chart.render(); <br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*DbreT5XlEkYAAAAAAAAAAAAAARQnAQ"  style="max-height: 100%" width = "346" height = "369"/>
            </td>
        </tr>
    </table>
</html>


### Android
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
           <td width="55%">
             mChart.setCanvas(canvasView); <br/>
             mChart.padding(10, 0, 0, 0); <br/>
             mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleLineChart.json")); <br/>
             mChart.interaction("pinch"); <br/>
             mChart.tooltip(new F2Chart.ToolTipConfigBuilder()); <br/>
             mChart.line().position("date*value"); <br/>
             mChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                     .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f))
                     .gridHidden()); <br/>
             mChart.setAxis("value", new F2Chart.AxisConfigBuilder()
                     .grid(new F2Chart.AxisGridConfigBuilder().type("dash"))); <br/>
             mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(5)); <br/>
             mChart.setScale("value", new F2Chart.ScaleConfigBuilder().min(10).max(340)); <br/>
             mChart.render(); <br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*4TiyTonsab4AAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" />
            </td>
        </tr>
    </table>
</html>
