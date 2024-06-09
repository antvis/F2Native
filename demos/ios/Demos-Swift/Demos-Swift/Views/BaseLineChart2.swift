//
//  BaseLineChart2.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import Foundation
import F2

class BaseLineChart2: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_multiAxiesLine", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        chart.clear()
        chart.syncYScale()(false)
        chart.canvas()(canvasView).padding()(20, 10, 20, 0).source()(jsonData)
        chart.scale()("date", ["tickCount": 5])
        chart.scale()("value", ["nice": true])
        chart.scale()("offset", ["nice": true])
        
        chart.axis()("value", ["grid": ["type": "dash", "dash": [15, 15, 5, 5]]])
        chart.axis()("offset", ["label": ["labelOffset": 10, "textAlign": "start"]])
        chart.axis()("date", ["label": ["labelOffset": 5]])
        
        chart.line()().position()("date*value").fixedSize()(2).fixedColor()("#1890FF")
        chart.line()().position()("date*offset").fixedSize()(2).fixedColor()("#2FC25B")
        chart.tooltip()([:])
        chart.animate()(true)
        chart.render()()
    }
}
