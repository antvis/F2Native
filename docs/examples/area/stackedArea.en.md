---
title: Stacked Area Chart
order: 1
---


### 数据（jsonString）
[下述 jsonData 对应数据](https://gw.alipayobjects.com/os/bmw-prod/a1808820-395f-44de-b069-b923edfdbf75.json)


## 演示

### iOS
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
            <td width=55%>
             F2CanvasView *canvasView =[F2CanvasView canvasWithFrame:self.frame];<br/>
             canvasView.delegate = self;<br/>
             F2Chart *chart = [F2Chart chart:canvasView.bounds.size withName:NSStringFromClass([self class])];<br/>
             chart.canvas(canvasView).padding(20, 10, 20, 0.f).source(jsonData);<br/>
             chart.scale(@"date", @{@"tickCount": @(5)});<br/>
             chart.scale(@"value", @{@"min": @(0)});<br/>
             chart.axis(@"date", @{@"grid": @(NO), @"label": @{@"textAlign": @"start"}});<br/>
             chart.line().adjust(@"stack").position(@"date*value").color(@"city", @[]).fixedSize(2);<br/>
             chart.area().adjust(@"stack").position(@"date*value").color(@"city", @[]).fixedSize(2);<br/>
             chart.render();<br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*SVaSRpL57o4AAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" />
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
             mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_multiAreasChart.json"));<br/>
             mChart.area().position("date*value").color("city").adjust("stack");<br/>
             mChart.line().position("date*value").color("city").adjust("stack");<br/>
             mChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                     .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f)));<br/>
             mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(5));<br/>
             mChart.render();<br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*P9PTSY6NDRIAAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" />
            </td>
        </tr>
    </table>
</html>