//
//  BaseAreaChart2.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import F2

class BaseAreaChart2: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_singleAreaChart_2", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        
        chart.canvas()(canvasView).padding()(20, 10, 20, 0).source()(jsonData)
        chart.axis()("tem", ["grid": ["stroke": "#000"]])
        chart.line()().position()("month*value")
        chart.area()().position()("month*value").fixedColor()("#1890FF0F")        
        chart.scale()("value", ["nice": true])
        chart.animate()(true)
        chart.render()()
    }
}
