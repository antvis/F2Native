//
//  CandleChart.swift
//  F2Native
//
//  Created by weiqing.twq on 2024-06-09.
//

import F2

class CandleChart: UIView {
    
    public var canvasView:F2CanvasView?
    public var candleChart:F2Chart?
    public var subChart:F2Chart?
    
    required override init(frame: CGRect) {
        super.init(frame: frame)
        self.canvasView = F2CanvasView.canvas(frame)
        self.addSubview(self.canvasView!)
        var size = self.canvasView!.bounds.size
        self.candleChart = F2Chart.init(CGSizeMake(size.width, size.height * 0.75), name: "KLineChart#Candle")
        self.subChart = F2Chart.init(CGSizeMake(size.width, size.height * 0.25), name: "KLineChart#Sub")
        self.chartRender();
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_klineDay", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let candleChart = self.candleChart,
              let subChart = self.subChart else {
            return
        }

        let jsonData = F2Utils.toJsonArray(jsonDataString)
        
        // Setup for the main candlestick chart
        candleChart.canvas()(canvasView)
        candleChart.source()(jsonData)
        candleChart.padding()(15, 10, 15, 0)
        candleChart.scale()("date", ["type": "kline-day", "domain": [40, 80]])
        candleChart.scale()("values", ["nice": true])
        candleChart.scale()("m5", ["assign": "values"])
        candleChart.scale()("m10", ["assign": "values"])
        candleChart.scale()("m20", ["assign": "values"])
        
        candleChart.axis()("m5", ["hidden": true])
        candleChart.axis()("m10", ["hidden": true])
        candleChart.axis()("m20", ["hidden": true])
        
        candleChart.candle()().position()("date*values")
        candleChart.line()().position()("date*m5").fixedColor()("#A46FFE")
        candleChart.line()().position()("date*m10").fixedColor()("#46ACFF")
        candleChart.line()().position()("date*m20").fixedColor()("#FF8C53")
        
        candleChart.interaction()("pan", [:])
        candleChart.interaction()("pinch", [:])
        candleChart.tooltip()([:])
        candleChart.animate()(true)
        candleChart.render()()

        // Setup for the volume chart
        subChart.adjustScale()(false);
        subChart.canvas()(canvasView)
        subChart.source()(jsonData)
        subChart.margin()(0, 210, 0, 0)
        subChart.padding()(15, 10, 15, 10)
        subChart.scale()("date", ["type": "kline-day", "domain": [40, 80]])
        subChart.scale()("amount", ["nice": true, "tickCount": 2, "tick": F2Callback { param in
            return ["content": "1 M"] // million
        }])
        
        subChart.axis()("date", ["hidden": true])
        subChart.interval()().position()("date*amount")
        
        subChart.interaction()("pan", [:])
        subChart.interaction()("pinch", [:])
        subChart.tooltip()([:])
        subChart.animate()(true)
        subChart.render()()
        
        // Add multiple gesture listeners and distribute events
        canvasView.addGestureListener("longPress", callback: handleGestureInfo)
        canvasView.addGestureListener("pinch", callback: handleGestureInfo)
        canvasView.addGestureListener("pan", callback: handleGestureInfo)

        func handleGestureInfo(_ info: [AnyHashable: Any]) {
            candleChart.postTouchEvent()(info)
            subChart.postTouchEvent()(info)
        }
    }

}

