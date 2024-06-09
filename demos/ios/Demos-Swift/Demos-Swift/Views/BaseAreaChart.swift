//
//  BaseAreaChart.swift
//  F2Native
//
//  Created by weiqing.twq on 2024-06-09.
//

import F2

class BaseAreaChart: BaseLineChart {
    
    override func chartRender() {
        let jsonPath = Bundle.main.path(forResource: "Res/mockData_baseArea", ofType: "json")
        guard let jsonString = try? String.init(contentsOfFile: jsonPath!) else {
            return
        }
        guard let chart = self.chart else {
            return
        }
        
        guard let canvasView = self.canvasView else {
            return
        }
        
        let height = canvasView.frame.size.height * UIScreen.main.scale
        let jsonData = F2Utils.toJsonArray(jsonString)
        chart.canvas()(canvasView)
        self.chart!.source()(jsonData)
        chart.padding()(10, 20, 10, 0)
        chart.scale()("tem", ["min": 0])
        chart.axis()("time", ["grid": false, "label": ["textAlign": "start"]])
        chart.line()().position()("time*tem").fixedSize()(2).fixedShape()("smooth")
        chart.area()().position()("time*tem").fixedShape()("smooth").fixedColor()(F2Utils.toJsonString([
            "colorStops": [
                ["offset": 0.0, "color": "#ff0000"],
                ["offset": 1.0, "color": "#ffffff"]
            ],
            "position": [0, 0, 0, height]
        ]))
        chart.point()().position()("time*tem").fixedColor()("#000000").style()([
            "custom": F2Callback { param in
                if param["_index"] as? Int == 3 {
                    return ["color": "#FFB6C1", "size": 0]
                }
                return [:]
            }
        ])
        chart.animate()(true)
        chart.tooltip()([:])
        chart.render()();
    }
}
