//
//  RadarAreaChart.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import F2

class RadarAreaChart: BaseLineChart {
    
    override func chartRender() {
        guard let jsonPath = Bundle.main.path(forResource: "Res/mockData_radarArea", ofType: "json"),
              let jsonDataString = try? String(contentsOfFile: jsonPath),
              let canvasView = self.canvasView,
              let chart = self.chart else {
            return
        }
        let jsonData = F2Utils.toJsonArray(jsonDataString)
        chart.canvas()(canvasView).padding()(20, 10, 20, 0)
        chart.source()(jsonData)
        chart.line()().position()("item*score").color()("user", [])
        chart.area()().position()("item*score").color()("user", [])
        chart.point()().position()("item*score").color()("user", [])
        chart.coord()(["type": "polar"])
        chart.scale()("score", ["type": "linear", "min": 0, "max": 120, "nice": false, "tickCount": 4])
        chart.scale()("item", ["type": "cat"])
        chart.axis()("score", ["label": ["labelOffset": 20], "grid": ["type": "line", "fill": ["#E8E8E82A", "#ffffff"], "stroke": "#E8E8E8"]])
        chart.axis()("item", ["label": ["labelOffset": 10], "grid": ["type": "dash", "stroke": "#E8E8E8", "dash": [4, 2]]])
        chart.animate()(true)
        chart.legend()("user", ["position": "top"])
        chart.render()()
        
        let center = chart.getCoord()().getCenter()()
        let design = chart.getPosition()(["item": "Design", "score": 70])
        let technology = chart.getPosition()(["item": "Technology", "score": 70])
        let test = chart.getPosition()(["item": "Test", "score": 70])
        let marketing = chart.getPosition()(["item": "Marketing", "score": 60])
        
        var textAlign = design.x > center.x ? "start" : (design.x < center.x ? "end" : "center")
        var textBaseline = design.y > center.y ? "top" : (design.y < center.y ? "bottom" : "middle")
        chart.guide()().text()(["content": "70", "position": ["Design", 70], "textAlign": textAlign, "textBaseline": textBaseline, "margin": [0, 5]])
        
        textAlign = technology.x > center.x ? "start" : (technology.x < center.x ? "end" : "center")
        textBaseline = technology.y > center.y ? "top" : (technology.y < center.y ? "bottom" : "middle")
        chart.guide()().text()(["content": "70", "position": ["Technology", 70], "textAlign": textAlign, "textBaseline": textBaseline, "margin": [5, 0]])
        
        textAlign = test.x > center.x ? "start" : (test.x < center.x ? "end" : "center")
        textBaseline = test.y > center.y ? "top" : (test.y < center.y ? "bottom" : "middle")
        chart.guide()().text()(["content": "70", "position": ["Test", 70], "textAlign": textAlign, "textBaseline": textBaseline, "margin": [0, -5]])
        
        textAlign = marketing.x > center.x ? "start" : (marketing.x < center.x ? "end" : "center")
        textBaseline = marketing.y > center.y ? "top" : (marketing.y < center.y ? "bottom" : "middle")
        chart.guide()().text()(["content": "60", "position": ["Marketing", 60], "textAlign": textAlign, "textBaseline": textBaseline, "margin": [-5, 0]])
        
        chart.repaint()()
    }
}
