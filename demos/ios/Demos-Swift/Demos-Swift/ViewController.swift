//
//  ViewController.swift
//  Demos-Swift
//
//  Created by weiqing.twq on 2022/6/1.
//

import UIKit


class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {
    var demoTable: UITableView?
    // Lazy initialization of the UIBarButtonItem in Swift
      lazy var rightButton: UIBarButtonItem = {
          let button = UIBarButtonItem(title: "ChartList", style: .done, target: self, action: #selector(onMultiChart))
          return button
      }()
    
    required init() {
        self.demoTable = nil;
        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.title = "F2Native"
        self.demoTable = UITableView.init(frame: self.view.bounds)
        self.demoTable?.dataSource = self
        self.demoTable?.delegate = self
        self.demoTable?.backgroundColor = UIColor.white;
        self.demoTable?.separatorColor = UIColor.gray;
        self.view.addSubview(self.demoTable!)
        self.navigationItem.rightBarButtonItem = rightButton
    }
    
    static func demoInfo() -> NSArray {
        return [["type": "baseLine", "name": "Line Chart", "view": "BaseLineChart"],
                ["type": "multiAxiesLine", "name":  "Line Chart(Dual Y-axis)", "view": "BaseLineChart2"],
                ["type": "multiLine", "name": "ContrastLineChart", "view": "ContrastLineChart"],
                ["type": "baseInterval", "name":"Bar Chart", "view": "BaseBarChart"],
                ["type": "baseInterval2", "name": "Bar Chart2", "view": "BaseBarChart2"],
                ["type": "multiInterval", "name": "Group Bar Chart", "view": "GroupBarChart"],
                ["type": "multiInterval2", "name": "Group Bar Chart2 (with negative value)", "view": "GroupBarChart2"],
                ["type": "multiInterval3", "name":"Group Stack Bar Chart", "view": "GroupStackBarChart"],
                ["type": "setionInterval", "name": "Histogram Bar Chart", "view": "HistogramBarChart"],
                ["type": "Point", "name": "Base Point Chart", "view": "BasePointChart"],
                ["type": "baseArea", "name": "Area Chart", "view": "BaseAreaChart"],
                ["type": "baseArea2", "name": "Area Chart2", "view": "BaseAreaChart2"],
                ["type": "baseArea3", "name": "Area Chart3(x baseline is not 0)", "view": "BaseAreaChart3"],
                ["type": "multiArea", "name": "Stacked Area Chart", "view": "StackedAreaChart"],
                ["type": "radarArea","name": "Radar Area Chart","view": "RadarAreaChart"],
                ["type": "basePie", "name": "Pie Chart", "view": "BasePieChart"],
                ["type": "klineDay", "name": "Candle Chart", "view": "CandleChart"],
        ]
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return ViewController.demoInfo().count;
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let ideitifier = "demoCell";
        var cell = tableView.dequeueReusableCell(withIdentifier: ideitifier)
        if((cell == nil)) {
            cell = UITableViewCell.init(style: UITableViewCell.CellStyle.value1, reuseIdentifier: ideitifier)
            cell!.backgroundColor = UIColor.white
            cell!.textLabel?.textColor = UIColor.black;
            cell!.selectionStyle = UITableViewCell.SelectionStyle.none
        }
        let info: NSDictionary = ViewController.demoInfo().object(at: indexPath.row) as! NSDictionary
        cell!.textLabel?.text = info.object(forKey: "name") as? String
        return cell!;
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let info: NSDictionary = ViewController.demoInfo().object(at: indexPath.row) as! NSDictionary
        let demoVC = DemoViewController.init(info: info)
        self.navigationController?.pushViewController(demoVC, animated: true)
    }
    
    @objc func onMultiChart() {
        let listCtr = ChartListViewController() // Assuming ChartListViewController is correctly imported
        navigationController?.pushViewController(listCtr, animated: true)
    }
}

