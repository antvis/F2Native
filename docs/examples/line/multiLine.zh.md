---
title: 对比折线图
order: 1
icon: line
---


## 演示
### 数据（jsonString）
下述 jsonData 对应数据
[绘图数据](https://gw.alipayobjects.com/os/bmw-prod/2908c0ad-77d5-4c0f-85f4-ac3ca81c818e.json)



### iOS
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
             <td width="55%">
             F2CanvasView *canvasView = [F2CanvasView canvas:self.frame];<br/>
             F2Chart *chart = [F2Chart chart:canvasView.bounds.size name:NSStringFromClass([self class])];<br/>
             chart.canvas(canvasView).padding(20, 10, 20, 0.f).source(jsonData);<br/>
             chart.scale(@"value", @{@"min": @(0)});<br/>
             chart.scale(@"date", @{@"tickCount": @(3)});<br/>
             chart.axis(@"date", @{@"grid": @(NO), @"label": @{@"textAlign": @"start"}});<br/>
             chart.line().position(@"date*value").color(@"type", @[]).fixedSize(2).fixedShape(@"smooth");<br/>
             chart.legend(@"type", @{@"radius": @(3), @"symbol": @"square"});<br/>
             chart.tooltip(@{});<br/>
             chart.render();<br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*wR9OTrWNjb0AAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" height = "369" />
            </td>
        </tr>
    </table>
</html>


### Android
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
            <td width="55%">
                mChart.setCanvas(canvasView);<br/>
                mChart.padding(10, 0, 0, 0);<br/>
                mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_multilines.json"));<br/>
                mChart.line().position("date*value").color("type")
                                                    .fixedSize(1.f)
                                                    .fixedShape("smooth");<br/>
                mChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                        .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f)));<br/>
                mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(5));<br/>
                mChart.setScale("value", new F2Chart.ScaleConfigBuilder().min(30).max(160));<br/>
                mChart.tooltip(new F2Chart.ToolTipConfigBuilder());<br/>
                mChart.render();<br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*lihuRrkmYp8AAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" />
            </td>
        </tr>
    </table>
</html>




