//
//  ContrastLineChart.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import Foundation
import F2

class ContrastLineChart: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_multiLines", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        chart.clear()
        chart.canvas()(canvasView).padding()(20, 10, 20, 0)
        chart.source()(jsonData)
        chart.scale()("value", ["nice": true])
        chart.scale()("date", ["tickCount": 5])
        chart.axis()("date", ["label": ["textAlign": "center"]])
        chart.line()().position()("date*value").color()("type", []).fixedSize()(2).fixedShape()("smooth")
        chart.legend()("type", ["radius": 3, "symbol": "square"])
        chart.tooltip()([:])
        chart.animate()(true)
        chart.render()()
    }
}
