//
//  BasePieChart.swift
//  F2Native
//
//  Created by weiqing.twq on 2024-06-09.
//

import F2

class BasePieChart: BaseLineChart {
    
    override func chartRender() {
        let jsonPath = Bundle.main.path(forResource: "Res/mockData_basePie", ofType: "json")
        guard let jsonString = try? String.init(contentsOfFile: jsonPath!) else {
            return
        }
        guard let chart = self.chart else {
            return
        }
        
        guard let canvasView = self.canvasView else {
            return
        }
        
        let jsonData = F2Utils.toJsonArray(jsonString)
        chart.clear()
        chart.canvas()(canvasView)
        chart.source()(jsonData)
        chart.padding()(10, 20, 10, 0)
        chart.axis()("percent", ["line": false, "label": false, "grid": false])
        chart.axis()("a", ["line": false, "label": false, "grid": false])
        chart.coord()(["type": "polar", "transposed": true])
        chart.interval()().position()("a*percent").color()("name", []).adjust()("stack").fixedSize()(1).style()(["lineWidth": 1, "stroke": "#ffffff"])
        chart.animate()(true)
        chart.render()()
    }
}

