//
//  BaseBarChart2.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import F2

class BaseBarChart2: BaseLineChart {
    
    override func chartRender() {
        let jsonPath = Bundle.main.path(forResource: "Res/mockData_baseInterval2", ofType: "json")
        guard let jsonString = try? String.init(contentsOfFile: jsonPath!) else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonString)
        self.chart!.canvas()(self.canvasView!)
        self.chart!.padding()(10, 20, 10, 0)
        self.chart!.source()(jsonData)
        self.chart!.interval()().position()("genre*sold").color()("genre", [])
        self.chart!.scale()("sold", ["min":0])
        self.chart!.scale()("genre", ["range": [0.1, 0.9]])
        self.chart!.animate()(true)
        self.chart!.render()();
    }
}
