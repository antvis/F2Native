//
//  DemoViewController.swift
//  Demos-Swift
//
//  Created by weiqing.twq on 2022/6/1.
//

import UIKit

class DemoViewController: UIViewController {
    
    var info: NSDictionary;
    
    required init?(coder: NSCoder) {
        self.info = NSDictionary.init();
        super.init(coder: coder)
    }
    
    required init(info: NSDictionary) {
        self.info = info;
        super.init(nibName: nil, bundle: nil);
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.edgesForExtendedLayout = UIRectEdge.init(rawValue:0);
        if #available(iOS 13.0, *) {
            let appearance = UINavigationBarAppearance.init()
            appearance.configureWithOpaqueBackground()
            self.navigationController?.navigationBar.standardAppearance = appearance
            self.navigationController?.navigationBar.scrollEdgeAppearance = appearance
        }
     
        self.title = self.info.object(forKey: "name") as? String
        self.navigationController?.navigationBar.backgroundColor = UIColor.white
        self.view.backgroundColor = UIColor.white;
        self.view.addSubview(self.demoView())
    }
    
    func demoView() -> UIView {
        var appName = Bundle.main.object(forInfoDictionaryKey: "CFBundleName") as! String
        appName = appName.replacingOccurrences(of: "-", with: "_")
        let viewName = self.info.object(forKey: "view") as! String;
        let className = appName + "." + viewName;
        let cls = NSClassFromString(className) as! UIView.Type
        let view = cls.init(frame:CGRect(x: 0, y: 0, width: self.view.bounds.width, height: 280))
        return view
    }
}
