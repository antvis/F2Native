//
//  ChartListViewController.swift
//  F2Native
//
//  Created by Westin on 2024-06-09.
//

import UIKit

class ChartListViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {
    
    private lazy var demoTable: UITableView = {
            let table = UITableView(frame: self.view.bounds)
            table.delegate = self
            table.dataSource = self
            table.backgroundColor = .white
            
            #if targetEnvironment(macCatalyst)
            let pan = UIPanGestureRecognizer(target: self, action: #selector(onPan(_:)))
            table.addGestureRecognizer(pan)
            #endif
            
            return table
        }()
    
    private var s0: CGPoint = .zero
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.edgesForExtendedLayout = UIRectEdge.init(rawValue:0);
        if #available(iOS 13.0, *) {
            let appearance = UINavigationBarAppearance.init()
            appearance.configureWithOpaqueBackground()
            self.navigationController?.navigationBar.standardAppearance = appearance
            self.navigationController?.navigationBar.scrollEdgeAppearance = appearance
        }
        self.navigationController?.navigationBar.backgroundColor = .white
        self.title = "ChartList"
        
        self.view.addSubview(self.demoTable)
    }
    
    @objc func onPan(_ pan: UIPanGestureRecognizer) {
        let point = pan.translation(in: self.demoTable)
        if pan.state == .began {
            s0 = demoTable.contentOffset
        } else if pan.state == .changed {
            demoTable.contentOffset = CGPoint(x: s0.x, y: max(s0.y - point.y, 0))
        }
    }
    
    // MARK: - UITableViewDelegate & UITableViewDataSource
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return ViewController.demoInfo().count
    }
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let identifier = "demoCell"
        var cell = tableView.dequeueReusableCell(withIdentifier: identifier)
        if cell == nil {
            cell = UITableViewCell(style: .value1, reuseIdentifier: identifier)
            cell?.selectionStyle = .none
        } else {
            cell?.contentView.viewWithTag(100)?.removeFromSuperview()
        }
        cell?.backgroundColor = .white
        cell?.textLabel?.textColor = .black
        cell?.contentView.addSubview(demoViewWithIndex(indexPath.row))
        return cell!
    }
    
    func demoViewWithIndex(_ index: Int) -> UIView {
        guard let dict = ViewController.demoInfo()[index] as? [String: Any],
              let viewName = dict["view"] as? String else {
            return UIView() // Return an empty view if class not found
        }
        var appName = Bundle.main.object(forInfoDictionaryKey: "CFBundleName") as! String
        appName = appName.replacingOccurrences(of: "-", with: "_")
        let className = appName + "." + viewName;
        let cls = NSClassFromString(className) as! UIView.Type
        let view = cls.init(frame:CGRect(x: 0, y: 0, width: self.view.bounds.width, height: 280))
        view.tag = 100
        return view
    }
    
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return 350.0
    }
}

