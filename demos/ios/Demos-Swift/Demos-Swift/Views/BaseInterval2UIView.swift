//
//  BaseInterval2UIView.swift
//  Demos-Swift
//
//  Created by weiqing.twq on 2022/6/1.
//

import Foundation
import F2

class BaseInterval2UIView: BaseLineUIView {
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
        self.chart!.render()();
    }
}
