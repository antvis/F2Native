#import <TargetConditionals.h>
#import <F2/F2.h>
#import "ViewController.h"
#import "ChartListViewController.h"
#import "DemoViewController.h"
#import "BugFixViewController.h"

@interface ViewController () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) UITableView *demoTable;
@property (nonatomic, assign) CGPoint s0;
@property (nonatomic, strong) UIBarButtonItem *leftButton;
@property (nonatomic, strong) UIBarButtonItem *rightButton;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.navigationController.navigationBar.backgroundColor = [UIColor whiteColor];
    self.navigationItem.leftBarButtonItem = self.leftButton;
    self.navigationItem.rightBarButtonItem = self.rightButton;
    self.title = @"F2Native";
    [self.view addSubview:self.demoTable];
}

- (UITableView *)demoTable {
    if(!_demoTable) {
        _demoTable = [[UITableView alloc] initWithFrame:self.view.bounds];
        _demoTable.delegate = self;
        _demoTable.dataSource = self;
        _demoTable.backgroundColor = [UIColor whiteColor];
        _demoTable.separatorColor = [UIColor grayColor];
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

- (UIBarButtonItem *)leftButton {
    if (!_leftButton) {
        _leftButton = [[UIBarButtonItem alloc] initWithTitle:@"Bug Fix" style:UIBarButtonItemStyleDone target:self action:@selector(onBugFix)];
    }
    return _leftButton;
}

- (UIBarButtonItem *)rightButton {
    if (!_rightButton) {
        _rightButton = [[UIBarButtonItem alloc] initWithTitle:@"多图列表" style:UIBarButtonItemStyleDone target:self action:@selector(onMultiChart)];
    }
    return _rightButton;
}

- (void)onBugFix {
    BugFixViewController *listCtr = [[BugFixViewController alloc] init];
    [[self navigationController] pushViewController:listCtr animated:YES];
}

- (void)onMultiChart {
    ChartListViewController *listCtr = [[ChartListViewController alloc] init];
    [[self navigationController] pushViewController:listCtr animated:YES];
}

+ (NSArray *)demoInfo {
    return @[
        @{@"type": @"baseLine", @"name": @"基础折线图", @"view": @"BaseLineUIView"},
        @{@"type": @"multiAxiesLine", @"name": @"基础折线图(双Y轴)", @"view": @"MultiAxiesLineUIView"},
        @{@"type": @"multiLine", @"name": @"对比折线图", @"view": @"MultiLineUIView"},
        @{@"type": @"baseArea", @"name": @"基础面积图", @"view": @"BaseAreaUIView"},
        @{@"type": @"baseArea2", @"name": @"基础面积图2(带负值)", @"view": @"BaseAreaUIView2"},
        @{@"type": @"baseArea3", @"name": @"基础面积图2(x基线不为0)", @"view": @"BaseAreaUIView3"},
        @{@"type": @"multiArea", @"name": @"层叠面积图", @"view": @"MultiAreaUIView"},
        @{@"type": @"radarArea", @"name": @"雷达面积图", @"view": @"RadarAreaUIView"},
        @{@"type": @"baseInterval", @"name": @"基础柱状图", @"view": @"BaseIntervalUIView"},
        @{@"type": @"baseInterval2", @"name": @"基础柱状图2", @"view": @"BaseInterval2UIView"},
        @{@"type": @"multiInterval", @"name": @"分组柱状图", @"view": @"MultintervalUIView"},
        @{@"type": @"multiInterval2", @"name": @"分组柱状图2(带负值)", @"view": @"MultiIntervalUIView2"},
        @{@"type": @"multiInterval3", @"name": @"分组层叠柱状图", @"view": @"MultiIntervalUIView3"},
        @{@"type": @"setionInterval", @"name": @"区间柱状图", @"view": @"SetionIntervalUIView"},
        @{@"type": @"Point", @"name": @"散点图", @"view": @"BasePointUIView"},
        @{@"type": @"basestrip", @"name": @"条状图", @"view": @"StripUIView"},
        @{@"type": @"basePie", @"name": @"饼图", @"view": @"BasePieUIView"},
        @{@"type": @"basePie", @"name": @"环形饼图", @"view": @"CyclicPieUIView"},
        @{@"type": @"klineDay", @"name": @"蜡烛图(日K)", @"view": @"KLineUIView"},
        @{@"type": @"timeSharing", @"name": @"股票分时图", @"view": @"TimeSharingUIView"},
        @{@"type": @"marketMoving", @"name": @"大盘异动", @"view": @"MarketMovingUIView"},
        @{@"type": @"baseGuide", @"name": @"辅助组件", @"view": @"BaseGuideUIView"},
    ];
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
        cell.backgroundColor = [UIColor whiteColor];
        cell.textLabel.textColor = [UIColor blackColor];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }

    NSDictionary *info = [[ViewController demoInfo] objectAtIndex:indexPath.row];
    cell.textLabel.text = [info objectForKey:@"name"];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    NSDictionary *info = [[ViewController demoInfo] objectAtIndex:indexPath.row];
    DemoViewController *demoContr = [[DemoViewController alloc] initWithInfo:info];
    [[self navigationController] pushViewController:demoContr animated:YES];
}

@end
