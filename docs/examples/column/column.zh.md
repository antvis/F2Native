---
title: 基础柱状图
order: 0
---


## 演示


### 数据（jsonString）
下述 jsonData 对应数据

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
             <td width="53%">
            F2CanvasView *canvasView =[F2CanvasView canvasWithFrame:self.frame];<br/>
             canvasView.delegate = self;<br/>
             F2Chart *chart = [F2Chart chart:canvasView.bounds.size withName:NSStringFromClass([self class])];<br/>
             chart.axis(@"year", @{@"grid": @(NO)});
             chart.axis(@"sales",@{
             @"grid":@{
                @"type":@"dash",
                @"lineWidth":@(2),
                @"lineDash":@[@(6),@(2)]
             }
              });<br/>
            chart.scale(@"year", @{ @"range": @[@(0.1), @(0.9)] });<br/>
            chart.scale(@"sales",@{ @"min": @(1)});<br/>
            chart.interval().position(@"year*sales");<br/>
            chart.render();
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*PN9kR59onjMAAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" height = "369" />
            </td>
        </tr>
    </table>
</html>


### Android
<html>
    <table style="margin-left: auto; margin-right: auto;">
        <tr>
             <td width="53%">
             mChart.setCanvas(canvasView); <br/>
             mChart.padding(10, 0, 0, 0);<br/>
             mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleIntervalChart.json"));<br/>
             mChart.interval().position("year*sales");<br/>
             mChart.setAxis("year", new F2Chart.AxisConfigBuilder()
                     .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f))
                     .gridHidden());<br/>
             mChart.setAxis("sales", new F2Chart.AxisConfigBuilder()
                     .grid(new F2Chart.AxisGridConfigBuilder().type("dash")));<br/>
             mChart.setScale("year", new F2Chart.ScaleConfigBuilder().range(new double[]{0.1, 0.9}));<br/>
             mChart.setScale("sales", new F2Chart.ScaleConfigBuilder().max(160));<br/>
            </td>
            <td>
             <img src="https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*zpuaQIjEAm8AAAAAAAAAAAAAARQnAQ" style="max-height: 100%" width = "346" />
            </td>
        </tr>
    </table>
</html>
