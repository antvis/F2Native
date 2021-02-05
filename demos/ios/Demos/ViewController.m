#import "ViewController.h"
#import "DemoViewController.h"
#import <F2/F2.h>

@interface ViewController () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) F2CanvasView *canvas;
@property (nonatomic, strong) F2Chart *chart;
@property (nonatomic, strong) UITableView *demoTable;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.edgesForExtendedLayout = UIRectEdgeNone;
    self.navigationController.navigationBar.backgroundColor = [UIColor redColor];
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
        @{@"type": @"multiAxiesLine", @"name": @"基础折线图(双Y轴)", @"view": @"MultiAxiesLineUIView"},
        @{@"type": @"multiLine", @"name": @"对比折线图", @"view": @"MultiLineUIView"},
        @{@"type": @"baseArea", @"name": @"基础面积图", @"view": @"BaseAreaUIView"},
        @{@"type": @"baseArea2", @"name": @"基础面积图2(带负值)", @"view": @"BaseAreaUIView2"},
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
        @{@"type": @"klineDay", @"name": @"蜡烛图(日K)", @"view": @"KLineUIView"}
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
    NSDictionary *info = [[self demoInfo] objectAtIndex:indexPath.row];
    cell.textLabel.text = [info objectForKey:@"name"];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    NSDictionary *info = [[self demoInfo] objectAtIndex:indexPath.row];
    DemoViewController *demoContr = [[DemoViewController alloc] initWithInfo:info];
    [[self navigationController] pushViewController:demoContr animated:YES];
}

@end
