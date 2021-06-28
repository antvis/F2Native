//
//  ChartListViewController.m
//  F2Native
//
//  Created by 青原 on 2021/5/13.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "ChartListViewController.h"

@interface ChartListViewController () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) UITableView *demoTable;
@end

@implementation ChartListViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.edgesForExtendedLayout = UIRectEdgeNone;
    self.navigationController.navigationBar.backgroundColor = [UIColor redColor];
    self.title = @"ChatList";
    [self.view addSubview:self.demoTable];
}

- (UITableView *)demoTable {
    if(!_demoTable) {
        _demoTable = [[UITableView alloc] initWithFrame:self.view.frame];
        _demoTable.delegate = self;
        _demoTable.dataSource = self;
    }
    return _demoTable;
}

#pragma mark - tableView delegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 10;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *ideitifier = @"demoCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:ideitifier];
    if(!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:ideitifier];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    [cell.contentView addSubview:[self demoViewWithIndex:indexPath.row]];
    return cell;
}

- (UIView *)demoViewWithIndex:(NSInteger)index {
    //    NSDictionary *dic = [[self demoInfo] objectAtIndex:index];
    Class class = NSClassFromString(@"BaseLineUIView");
    UIView *demoView = [[class alloc] init];
    return demoView;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 300.f;
}

@end
