#import "ViewController.h"
#import "ChartListViewController.h"
#import "DemoViewController.h"
#import "ListViewController.h"
#import <F2/F2.h>

@interface ViewController () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) UITableView *demoTable;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.navigationController.navigationBar.backgroundColor = [UIColor whiteColor];
    self.title = @"F2Native";
    [self.view addSubview:self.demoTable];
}

- (UITableView *)demoTable {
    if(!_demoTable) {
        _demoTable = [[UITableView alloc] initWithFrame:self.view.bounds];
        _demoTable.delegate = self;
        _demoTable.dataSource = self;
    }
    return _demoTable;
}

- (NSArray *)demoInfo {
    return @[
        @{@"type": @"baseLine", @"name": @"基础折线图", @"view": @"BaseLineUIView"},
        @{@"type": @"multiAxiesLine", @"name": @"基础折线图(双Y轴)", @"view": @"MultiAxiesLineUIView"},
        @{@"type": @"multiLine", @"name": @"对比折线图", @"view": @"MultiLineUIView"},
        @{@"type": @"baseArea", @"name": @"基础面积图", @"view": @"BaseAreaUIView"},
        @{@"type": @"baseArea2", @"name": @"基础面积图2(带负值)", @"view": @"BaseAreaUIView2"},
        @{@"type": @"baseArea3", @"name": @"基础面积图2(x基线不为0)", @"view": @"BaseAreaUIView3"},
        @{@"type": @"multiArea", @"name": @"层叠面积图", @"view": @"MultiAreaUIView"},
        @{@"type": @"baseInterval", @"name": @"基础柱状图", @"view": @"BaseIntervalUIView"},
        @{@"type": @"baseInterval2", @"name": @"基础柱状图2", @"view": @"BaseInterval2UIView"},
        @{@"type": @"multiInterval", @"name": @"分组柱状图", @"view": @"MultintervalUIView"},
        @{@"type": @"multiInterval2", @"name": @"分组柱状图2(带负值)", @"view": @"MultiIntervalUIView2"},
        @{@"type": @"setionInterval", @"name": @"区间柱状图", @"view": @"SetionIntervalUIView"},
        @{@"type": @"Point", @"name": @"散点图", @"view": @"BasePointUIView"},
        @{@"type": @"basestrip", @"name": @"条状图", @"view": @"StripUIView"},
        @{@"type": @"basePie", @"name": @"饼图", @"view": @"BasePieUIView"},
        @{@"type": @"basePie", @"name": @"环形饼图", @"view": @"CyclicPieUIView"},
        @{@"type": @"klineDay", @"name": @"蜡烛图(日K)", @"view": @"KLineUIView"},
        @{@"type": @"timeSharing", @"name": @"股票分时图", @"view": @"TimeSharingUIView"},
    ];
}

#pragma mark - tableView delegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [self demoInfo].count + 1;
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
    if(indexPath.row == [self demoInfo].count) {
        cell.textLabel.text = @"图表列表内复用";
    } else {
        NSDictionary *info = [[self demoInfo] objectAtIndex:indexPath.row];
        cell.textLabel.text = [info objectForKey:@"name"];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if(indexPath.row == [self demoInfo].count) {
        //        ListViewController *listCtr = [[ListViewController alloc] init];
        //        [[self navigationController] pushViewController:listCtr animated:YES];
        ChartListViewController *listCtr = [[ChartListViewController alloc] init];
        [[self navigationController] pushViewController:listCtr animated:YES];
    } else {
        NSDictionary *info = [[self demoInfo] objectAtIndex:indexPath.row];
        DemoViewController *demoContr = [[DemoViewController alloc] initWithInfo:info];
        [[self navigationController] pushViewController:demoContr animated:YES];
    }
}

@end
