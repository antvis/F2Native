//
//  GroupBarChart2.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import F2

class GroupBarChart2: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_singleIntervalChart_under_zero", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        chart.canvas()(canvasView).padding()(20, 10, 20, 0)
        chart.source()(jsonData)
        chart.axis()("tem", ["grid": ["stroke": "#000"]])
        chart.interval()().position()("time*tem").color()("city", []).adjust()("dodge")
        chart.scale()("time", ["range": [0.1, 0.9]])
        chart.scale()("tem", ["nice": true])
        chart.tooltip()([:])
        chart.animate()(true)
        chart.render()()
    }
}


