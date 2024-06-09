//
//  GroupBarChart.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import F2

class GroupBarChart: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_multiInterval", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        chart.canvas()(canvasView).padding()(15, 15, 15, 15)
        chart.source()(jsonData)
        chart.axis()("month", ["grid": false])
        chart.scale()("month", ["range": [0.1, 0.9]])
        chart.scale()("value", ["nice": true])
        chart.interval()().adjust()("dodge").position()("month*value").color()("name", [])
        chart.tooltip()([:])
        chart.legend()("name", ["position": "bottom", "align": "center"])
        chart.animate()(true)
        chart.render()()
    }
}

