//
//  BugFixViewController.h
//  F2Native
//
//  Created by ruize on 2022/2/7.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#import "BugFixViewController.h"

@interface BugFixViewController () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) UITableView *demoTable;
@end

@implementation BugFixViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.edgesForExtendedLayout = UIRectEdgeNone;
    self.navigationController.navigationBar.backgroundColor = [UIColor whiteColor];    

    self.title = @"多图列表";
    [self.view addSubview:self.demoTable];
}

- (UITableView *)demoTable {
    if(!_demoTable) {
        _demoTable = [[UITableView alloc] initWithFrame:self.view.frame];
        _demoTable.delegate = self;
        _demoTable.dataSource = self;
        _demoTable.backgroundColor = [UIColor whiteColor];
    }
    return _demoTable;
}

#pragma mark - tableView delegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.buglist.count;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *ideitifier = @"demoCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:ideitifier];
    if(!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:ideitifier];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        [cell.contentView addSubview:[self demoViewWithIndex:indexPath.row]];
        cell.backgroundColor = [UIColor whiteColor];
        cell.textLabel.textColor = [UIColor blackColor];
    }
    return cell;
}

- (UIView *)demoViewWithIndex:(NSInteger)index {
    NSDictionary *dict = [self.buglist objectAtIndex:index];
    Class class = NSClassFromString(dict[@"view"]);
    UIView *demoView = [[class alloc] init];
    return demoView;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 280;
}

- (NSArray *)buglist {
    return @[
//        @{@"type": @"KLineTrendBugFix1", @"name": @"K线趋势图", @"view": @"KLineTrendBugFix1"},
//        @{@"type": @"TimeSharingBugFix1", @"name": @"顶部折线被裁", @"view": @"TimeSharingBugFix1"},
        @{@"type": @"MarketMovingBugFix1", @"name": @"大盘异动旗子布局", @"view": @"MarketMovingBugFix1"},
    ];
}

@end
