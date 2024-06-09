//
//  BaseInterval2UIView.swift
//  Demos-Swift
//
//  Created by weiqing.twq on 2022/6/1.
//

import Foundation
import F2

class BaseBarChart: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_baseInterval", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        chart.canvas()(canvasView).padding()(20, 10, 20, 10)
        chart.source()(jsonData)
        chart.axis()("year", ["grid": false])
        chart.axis()("sales", ["grid": ["type": "dash", "lineWidth": 2, "lineDash": [6, 2]]])
        chart.scale()("year", ["range": [0.1, 0.9]])
        chart.scale()("sales", ["nice": true])
        chart.interval()().position()("year*sales").style()([
            "rounding": [10, 10, 0, 0] // [tl, tr, bl, br]
        ])
        chart.tooltip()([:])
        chart.animate()(true)
        chart.render()()
    }
}
