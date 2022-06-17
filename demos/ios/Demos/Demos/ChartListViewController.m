//
//  ChartListViewController.m
//  F2Native
//
//  Created by 青原 on 2021/5/13.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "ChartListViewController.h"
#import "ViewController.h"

@interface ChartListViewController () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) UITableView *demoTable;
@property (nonatomic, assign) CGPoint s0;
@end

@implementation ChartListViewController

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
#if TARGET_OS_MACCATALYST == 1
        //解决mac上uitableview用鼠标拖动不能滚动
        UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(onPan:)];
        [_demoTable addGestureRecognizer:pan];
#endif
    }
    return _demoTable;
}

-(void)onPan:(UIPanGestureRecognizer *)pan {
    CGPoint point = [pan translationInView:self.demoTable];
    if (pan.state == UIGestureRecognizerStateBegan) {
        self.s0 = self.demoTable.contentOffset;
    } else if (pan.state == UIGestureRecognizerStateChanged){
        self.demoTable.contentOffset = CGPointMake(self.s0.x, MAX(self.s0.y - point.y, 0));
    }
}

#pragma mark - tableView delegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [ViewController demoInfo].count;
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
    }else {
        [[cell.contentView viewWithTag:100] removeFromSuperview];
    }
    cell.backgroundColor = [UIColor whiteColor];
    cell.textLabel.textColor = [UIColor blackColor];
    [cell.contentView addSubview:[self demoViewWithIndex:indexPath.row]];
    return cell;
}

- (UIView *)demoViewWithIndex:(NSInteger)index {
    NSDictionary *dict = [[ViewController demoInfo] objectAtIndex:index];
    Class class = NSClassFromString(dict[@"view"]);
    UIView *demoView = [[class alloc] init];
    demoView.tag = 100;
    return demoView;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 300.f;
}

@end
