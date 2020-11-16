---
title: Stacked Area Chart
order: 1
---


### 数据（jsonString）
下述 jsonData 对应数据

```json
[{"name": "London","month": "Jan.","value": 18.9}, 
 {"name": "London","month": "Feb.","value": 28.8}, 
 {"name": "London","month": "Mar.","value": 39.3}, 
 {"name": "London","month": "Apr.","value": 81.4},
 {"name": "London","month": "May.","value": 47}, 
 {"name": "London","month": "Jun.","value": 20.3}, 
 {"name": "London","month": "Jul.","value": 24},
 {"name": "London","month": "Aug.","value": 35.6},
 {"name": "Berlin","month": "Jan.","value": 12.4}, 
 {"name": "Berlin","month": "Feb.","value": 23.2}, 
 {"name": "Berlin","month": "Mar.","value": 34.5},
 {"name": "Berlin","month": "Apr.","value": 99.7},
 {"name": "Berlin","month": "May.","value": 52.6},
 {"name": "Berlin","month": "Jun.","value": 35.5}, 
 {"name": "Berlin","month": "Jul.","value": 37.4}, 
 {"name": "Berlin","month": "Aug.","value": 42.4}]

```


## 演示

### iOS
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
            <td width = 55%>
            F2CanvasView *canvasView =[F2CanvasView canvasWithFrame:self.frame];<br/>
             canvasView.delegate = self;<br/>
             F2Chart *chart = [F2Chart chart:canvasView.bounds.size withName:NSStringFromClass([self class])];<br/>
             chart.canvas(canvasView).padding(20, 10, 20, 0.f).source(jsonData);<br/>
             chart.axis(@"month", @{@"grid": @(NO)});<br/>
             chart.scale(@"month", @{@"range": @[@(0.1), @(0.9)]});<br/>
             chart.interval().adjust(@"dodge")
                             .position(@"month*value")
                             .color(@"name", @[]);<br/>
             chart.render();
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*V4ShQaBIk0sAAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" height = "369" />
            </td>
        </tr>
    </table>
</html>


### Android
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
             <td width = 55%>
                mChart.setCanvas(canvasView);<br/>
                mChart.padding(10, 0, 0, 0);<br/>
                mChart.source(Utils.loadAssetFile(canvasView.getContext(), "jsonData.json"));<br/>
                mChart.interval().position("month*value")
                                 .color("name").adjust("dodge");<br/>
                mChart.setScale("month", new F2Chart.ScaleConfigBuilder().range(new double[]{0.065, 0.935}));<br/>
                mChart.render();<br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*XPqsT4-ImmwAAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" />
            </td>
        </tr>
    </table>
</html>