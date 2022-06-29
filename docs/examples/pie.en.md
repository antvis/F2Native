---
title: Pie Chart
order: 4
---

## Example

### 数据（jsonString）
Prepare jsonData like this

```json
[{"name": "芳华","percent": 0.4,"a": "1"}, 
 {"name": "妖猫传","percent": 0.2,"a": "1"}, 
 {"name": "机器之血","percent": 0.18,"a": "1"}, 
 { "name": "心理罪","percent": 0.15,"a": "1"}, 
 {"name": "寻梦环游记","percent": 0.05,"a": "1"}, 
 {"name": "其他","percent": 0.12,"a": "1"}]
```

### iOS
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
             <td width = 55%>
            F2CanvasView *canvasView =[F2CanvasView canvas:self.frame];<br/>
             F2Chart *chart = [F2Chart chart:canvasView.bounds.size name:NSStringFromClass([self class])];<br/>
             chart.axis(@"percent", @{@"line": @(NO), @"label": @(NO)});<br/>
             chart.axis(@"a", @{@"line": @(NO), @"label": @(NO)});<br/>
             chart.coord(@{@"type": @"polar", @"transposed": @(YES)});<br/>
             chart.interval().style(@{@"lineWidth": @(1)}).position(@"a*percent").color(@"name", @[]).adjust(@"stack");<br/>
             chart.render();<br/>
            </td>
            <td>
                   <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*9j63SZTVV4oAAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" />
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
             mChart.padding(10, 20, 0, 0);<br/>
             mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_basePie.json"));<br/>
             mChart.setAxis("percent", new F2Chart.AxisConfigBuilder()<br/>
                     .lineHidden()
                     .labelHidden()
                     .gridHidden());<br/>
             mChart.setAxis("a", new F2Chart.AxisConfigBuilder()<br/>
             .labelHidden().lineHidden().gridHidden());<br/>
             mChart.setCoord(new F2Chart.CoordConfigBuilder().type("polar").transposed(true));<br/>
             mChart.interval().position("a*percent")<br/>
             .style(new F2Config.Builder()<br/>
             .setOption("lineWidth", 3).build()).color("name").adjust("stack");<br/>
             mChart.render();<br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*gzPBQop30zgAAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" />
            </td>
        </tr>
    </table>
</html>
