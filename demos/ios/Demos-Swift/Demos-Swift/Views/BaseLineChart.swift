//
//  BaseLineUIView.swift
//  Demos-Swift
//
//  Created by weiqing.twq on 2022/6/1.
//

import UIKit
import F2

class BaseLineChart: UIView {
    
    public var canvasView:F2CanvasView?
    public var chart:F2Chart?
    
    required override init(frame: CGRect) {
        super.init(frame: frame)
        self.canvasView = F2CanvasView.canvas(frame)
        self.addSubview(self.canvasView!)
        self.chart = F2Chart.init(self.canvasView!.bounds.size, name: "swiftdemo")
        self.chartRender();
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    func chartRender() {
        let jsonPath = Bundle.main.path(forResource: "Res/mockData_baseLine", ofType: "json")
        guard let jsonString = try? String.init(contentsOfFile: jsonPath!) else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonString)
        self.chart!.canvas()(self.canvasView!)
        self.chart?.padding()(10, 20, 10, 0)
        self.chart!.source()(jsonData)
        self.chart!.line()().position()("date*value")
        self.chart!.scale()("date", ["tickCount":3])
        let itemBlock = {(param: Dictionary<AnyHashable, Any>) -> Dictionary<AnyHashable, Any> in
            let index = param["index"] as? Int
            if (index! % 2 != 0) {
                return ["textColor": "#DC143C"]
            }
            return param
        } as ItemCallback
        let itemCallback = F2Callback.init(itemBlock);
        self.chart!.axis()("date",
                           ["grid": false,
                           "label":["item": itemCallback]]);
        self.chart!.animate()(true)
        self.chart!.render()();
    }
}
