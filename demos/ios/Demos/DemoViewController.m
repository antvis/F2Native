
#import "DemoViewController.h"

@interface DemoViewController ()
{
    NSString *_type;
    NSString *_name;
    NSString *_viewName;
}
@end

@implementation DemoViewController


-(instancetype)initWithInfo:(NSDictionary *)info {
    if (self = [super init]) {
        _type = [info objectForKey:@"type"];
        _name = [info objectForKey:@"name"];
        _viewName = [info objectForKey:@"view"];
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.edgesForExtendedLayout = UIRectEdgeNone;
    self.navigationController.navigationBar.backgroundColor = [UIColor whiteColor];
    self.view.backgroundColor = [UIColor whiteColor];
    if (@available(iOS 13, *)) {
        UINavigationBarAppearance *appearance = [[UINavigationBarAppearance alloc] init];
        [appearance configureWithOpaqueBackground];
        self.navigationController.navigationBar.standardAppearance = appearance;
        self.navigationController.navigationBar.scrollEdgeAppearance = appearance;
    }
    self.title = _name;
    [self.view addSubview:[self demoView]];
}


- (UIView *)demoView {
    Class class = NSClassFromString(_viewName);
    UIView *demoView = [[class alloc] init];
    return demoView;
}

@end
