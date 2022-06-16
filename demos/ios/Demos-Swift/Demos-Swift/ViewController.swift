//
//  ViewController.swift
//  Demos-Swift
//
//  Created by weiqing.twq on 2022/6/1.
//

import UIKit
import F2

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        
        let jsonPath = Bundle.main.path(forResource: "Res/mockData_baseLine", ofType: "json");
        var jsonData = "";
        do {
            jsonData = try String.init(contentsOfFile: jsonPath!);
        }catch {
            jsonData = "";
        }
        
        let canvas = F2CanvasView.canvas(withFrame: CGRect(x:0, y:0, width: self.view.bounds.size.width, height: 200))
        self.view.addSubview(canvas)
        
        let chart = F2Chart.init(canvas.bounds.size, withName: "swiftdemo")
        chart.canvas()(canvas);
        chart.source()(F2Utils.toJsonObject(jsonData) as! [Any]);
        chart.line()().position()("date*value")
        chart.scale()("date",["tickCount":3])
        chart.render()();
    }

}

