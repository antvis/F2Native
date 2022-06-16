//
//  ViewController.swift
//  Demos-Swift
//
//  Created by weiqing.twq on 2022/6/1.
//

import UIKit


class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {
    var demoTable: UITableView?
    
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

    }
    
    func demoInfo() -> NSArray {
        return [["type": "baseLine", "name": "基础折线图", "view": "BaseLineUIView"],
                ["type": "baseInterval2", "name": "基础柱状图2", "view": "BaseInterval2UIView"]]
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.demoInfo().count;
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
        let info: NSDictionary = self.demoInfo().object(at: indexPath.row) as! NSDictionary
        cell!.textLabel?.text = info.object(forKey: "name") as? String
        return cell!;
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let info: NSDictionary = self.demoInfo().object(at: indexPath.row) as! NSDictionary
        let demoVC = DemoViewController.init(info: info)
        self.navigationController?.pushViewController(demoVC, animated: true)
    }
}

