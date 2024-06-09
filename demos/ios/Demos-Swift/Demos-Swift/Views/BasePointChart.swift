//
//  BasePointChart.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import F2

class BasePointChart: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_singlePointChart", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        chart.clear()
        chart.canvas()(canvasView).padding()(20, 10, 20, 0)
        chart.source()(jsonData)
        chart.point()().position()("x*y").size()("z", [3, 10]).fixedShape()("circle")
        chart.animate()(true)
        chart.render()()
    }
}

