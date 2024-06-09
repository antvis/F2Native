//
//  GroupStackBarChart.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import F2

class GroupStackBarChart: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_multiInterval", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        chart.clear()
        chart.canvas()(canvasView).padding()(20, 10, 20, 0)
        chart.source()(jsonData)
        chart.axis()("month", ["grid": false])
        chart.scale()("month", ["range": [0.1, 0.9]])
        chart.scale()("value", ["nice": true])
        chart.interval()().adjust()("stack").position()("month*value").color()("name", [])
        chart.animate()(true)
        chart.render()()
    }
}
