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
        _rightButton = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"ChartList", @"ChartList") style:UIBarButtonItemStyleDone target:self action:@selector(onMultiChart)];
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
        @{@"type": @"baseLine", @"name": NSLocalizedString(@"BaseLineChart", "BaseLineChart"), @"view": @"BaseLineChart"},
        @{@"type": @"multiAxiesLine", @"name": NSLocalizedString(@"BaseLineChart2", "BaseLineChart(Dual Y-axis)"), @"view": @"BaseLineChart2"},
        @{@"type": @"multiLine", @"name": NSLocalizedString(@"ContrastLineChart", "ContrastLineChart"), @"view": @"ContrastLineChart"},
        @{@"type": @"baseArea", @"name": NSLocalizedString(@"BaseAreaChart", "BaseAreaChart"), @"view": @"BaseAreaChart"},
        @{@"type": @"baseArea2", @"name": NSLocalizedString(@"BaseAreaChart2", "BaseAreaChart2(with negative value)"), @"view": @"BaseAreaChart2"},
        @{@"type": @"baseArea3", @"name": NSLocalizedString(@"BaseAreaChart3", "BaseAreaChart3(x baseline is not 0)"), @"view": @"BaseAreaChart3"},
        @{@"type": @"multiArea", @"name": NSLocalizedString(@"StackedAreaChart", "StackedAreaChart"), @"view": @"StackedAreaChart"},
        @{@"type": @"baseInterval", @"name": NSLocalizedString(@"BaseBarChart", "BaseBarChart"), @"view": @"BaseBarChart"},
        @{@"type": @"baseInterval2", @"name": NSLocalizedString(@"BaseBarChart2", "BaseBarChart2"), @"view": @"BaseBarChart2"},
        @{@"type": @"multiInterval", @"name": NSLocalizedString(@"GroupBarChart", "GroupBarChart"), @"view": @"GroupBarChart"},
        @{@"type": @"multiInterval2", @"name": NSLocalizedString(@"GroupBarChart2", "GroupBarChart2(with negative value)"), @"view": @"GroupBarChart2"},
        @{@"type": @"multiInterval3", @"name": NSLocalizedString(@"GroupStackBarChart", "GroupStackBarChart"), @"view": @"GroupStackBarChart"},
        @{@"type": @"setionInterval", @"name": NSLocalizedString(@"HistogramBarChart", "HistogramBarChart"), @"view": @"HistogramBarChart"},
        @{@"type": @"Point", @"name": NSLocalizedString(@"BasePointChart", "BasePointChart"), @"view": @"BasePointChart"},
        @{@"type": @"basestrip", @"name": NSLocalizedString(@"StripChart", "StripChart"), @"view": @"StripChart"},
        @{@"type": @"basePie", @"name": NSLocalizedString(@"BasePieChart", "BasePieChart"), @"view": @"BasePieChart"},
        @{@"type": @"basePie", @"name": NSLocalizedString(@"CyclicPieChart", "CyclicPieChart"), @"view": @"CyclicPieChart"},
        @{@"type": @"radarArea", @"name": NSLocalizedString(@"RadarAreaChart", "RadarAreaChart"), @"view": @"RadarAreaChart"},
        @{@"type": @"klineDay", @"name": NSLocalizedString(@"CandleChart", "CandleChart"), @"view": @"CandleChart"},
        @{@"type": @"timeSharing", @"name":NSLocalizedString(@"TimeSharingChart", "TimeSharingChart"), @"view": @"TimeSharingChart"},
        @{@"type": @"marketMoving", @"name": NSLocalizedString(@"MarketMovingChart", "MarketMovingChart"), @"view": @"MarketMovingChart"},
        @{@"type": @"baseGuide", @"name": NSLocalizedString(@"BaseGuideChart", "BaseGuideChart"), @"view": @"BaseGuideChart"},
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
