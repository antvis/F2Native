//
//  HistogramBarChart.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import F2

class HistogramBarChart: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_setionInterval", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        chart.canvas()(canvasView).padding()(20, 10, 20, 0)
        chart.source()(jsonData)
        chart.axis()("x", ["grid": false])
        chart.axis()("y", ["grid": ["type": "dash", "lineWidth": 2, "lineDash": [6, 2]]])
        chart.scale()("x", ["range": [0.1, 0.9]])
        chart.scale()("y", ["nice": true])
        chart.interval()().position()("x*y")
        chart.animate()(true)
        chart.render()()
    }
}
