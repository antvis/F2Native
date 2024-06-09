//
//  StackedAreaChart.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import F2

class StackedAreaChart: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_multiArea", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        
        chart.canvas()(canvasView).padding()(20, 10, 20, 0)
        chart.source()(jsonData)
        chart.scale()("date", ["tickCount": 5])
        chart.scale()("value", ["min": 0])
        chart.axis()("date", ["grid": false, "label": ["textAlign": "start"]])
        chart.line()().adjust()("stack").position()("date*value").color()("city", []).fixedSize()(2)
        chart.area()().adjust()("stack").position()("date*value").color()("city", []).fixedSize()(2)
        chart.tooltip()([:])
        chart.animate()(true)
        chart.render()()
    }
}

