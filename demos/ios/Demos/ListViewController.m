//
//  ListViewController.m
//  F2Native
//
//  Created by XiaLuo on 2021/2/2.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "ListViewController.h"

@interface ListViewController () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) UITableView *demoTable;

@end

@implementation ListViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.edgesForExtendedLayout = UIRectEdgeNone;
    self.navigationController.navigationBar.backgroundColor = [UIColor whiteColor];
    self.title = @"F2Native";
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

- (NSArray *)demoInfo {
    return @[
        @{@"type": @"baseLine", @"name": @"基础折线图", @"view": @"BaseLineUIView"},
        @{@"type": @"multiLine", @"name": @"对比折线图", @"view": @"MultiLineUIView"},
        @{@"type": @"baseArea", @"name": @"基础面积图", @"view": @"BaseAreaUIView"},
        @{@"type": @"baseArea2", @"name": @"基础面积图2(带负值)", @"view": @"BaseAreaUIView2"},
        @{@"type": @"baseArea3", @"name": @"基础面积图3(x基线不为0)", @"view": @"BaseAreaUIView3"},
        @{@"type": @"multiArea", @"name": @"层叠面积图", @"view": @"MultiAreaUIView"},
        @{@"type": @"baseInterval", @"name": @"基础柱状图", @"view": @"BaseIntervalUIView"},
        @{@"type": @"baseInterval2", @"name": @"基础柱状图2", @"view": @"BaseInterval2UIView"},
        @{@"type": @"multiInterval", @"name": @"分组柱状图", @"view": @"MultintervalUIView"},
        @{@"type": @"multiInterval2", @"name": @"分组柱状图2(带负值)", @"view": @"MultiIntervalUIView2"},
        @{@"type": @"sectionInterval", @"name": @"区间柱状图", @"view": @"SetionIntervalUIView"},
        @{@"type": @"Point", @"name": @"散点图", @"view": @"BasePointUIView"},
        @{@"type": @"basestrip", @"name": @"条状图", @"view": @"StripUIView"},
        @{@"type": @"basePie", @"name": @"饼图", @"view": @"BasePieUIView"},
        @{@"type": @"basePie", @"name": @"环形饼图", @"view": @"CyclicPieUIView"},
    ];
}

#pragma mark - tableView delegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [self demoInfo].count;
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
    NSDictionary *dic = [[self demoInfo] objectAtIndex:index];
    Class class = NSClassFromString([dic objectForKey:@"view"]);
    UIView *demoView = [[class alloc] init];
    return demoView;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 300.f;
}

@end
