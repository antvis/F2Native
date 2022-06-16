//
//  AppDelegate.swift
//  Demos-Swift
//
//  Created by weiqing.twq on 2022/6/1.
//

import UIKit

@main
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        // Override point for customization after application launch.
        self.window = UIWindow.init(frame: UIScreen.main.bounds)
        let viewController = ViewController.init();
        self.window!.rootViewController = UINavigationController.init(rootViewController: viewController)
        self.window!.makeKeyAndVisible()
        return true
    }


}

