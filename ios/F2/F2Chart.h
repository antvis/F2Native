#import "F2Callback.h"
#import "F2Geom.h"
#import "F2Guide.h"
#import "F2Coordinate.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class F2CanvasView;

@interface F2Chart : NSObject

/// 快捷创建chart
/// @param size dp为单位
/// @param name 输出日志时会附加name
+ (F2Chart *)chart:(CGSize)size name:(NSString *)name;

/// 设置chart的画布
- (F2Chart * (^)(F2CanvasView *canvas))canvas;

/// 设置chart的padding
- (F2Chart * (^)(CGFloat left, CGFloat top, CGFloat right, CGFloat bottom))padding;

/// 设置chart的margin
- (F2Chart * (^)(CGFloat left, CGFloat top, CGFloat right, CGFloat bottom))margin;

/// 设置chart的元数据
/// data json数组格式的数据
- (F2Chart * (^)(NSArray *data))source;

/// 设置度量，
/// field 需要设置度量的字段
/// config config的具体字段待补充
- (F2Chart * (^)(NSString *field, NSDictionary *config))scale;

/// 设置轴
/// field 需要设置的轴
/// config配置如下
/// @{
/// @"label":@{@"labelOffset":@(10)},
/// @"grid":@{@"type":@"line", @"fill":@[@"#E8E8E82A", @"white"], @"stroke" :@"#E8E8E82A", @"dash": @[@(4), @(2)]},
/// @"line":@{@"type":@"line", @"lineWidth":@(0.6}, @"stroke":"#E8E8E8"}
/// label为轴上文字的配置信息
/// grid为网格线的配置信息
/// line为轴线的配置
- (F2Chart * (^)(NSString *field, NSDictionary *config))axis;

/// 设置图例
/// field 需要设置度量的字段
/// config config的具体字段待补充
- (F2Chart * (^)(NSString *field, NSDictionary *config))legend;

/// 设置坐标
/// config  坐标轴配置信息
- (F2Chart * (^)(NSDictionary *config))coord;

/// 指定交互类型
/// type --  pinch 缩放 | pan 平移
- (F2Chart * (^)(NSString *type, NSDictionary *config))interaction;

/// 开启 ToolTIp 功能
/// confg    具体字段待补充
- (F2Chart * (^)(NSDictionary *config))tooltip;

/// 配置动画功能
/// config    @(false) 为关闭动画
///       @{@"type":@"GroupWaveIn", @"duration": @(800), @"erasing":@"linear"}
///       type 动画类型  GroupWaveIn GroupScaleInXY GroupScaleInX GroupScaleInY
///       duration 时间 毫秒
///       earsing 动画效果 linear quadraticIn quadraticOut quadraticInOut cubicIn cubicOut elasticIn elasticOut elasticInOut backIn backOut backInOut bounceOut bounceIn bounceInOut
- (F2Chart * (^)(id config))animate;

/// 开始渲染
- (F2Chart * (^)(void))render;

/// 清除所有元素
- (F2Chart * (^)(void))clear;

/// 重新渲染
- (F2Chart * (^)(void))repaint;

/// 创建直线或折线
- (F2Line * (^)(void))line;

/// 创建柱
- (F2Interval * (^)(void))interval;

/// 创建阴影
- (F2Area * (^)(void))area;

/// 散点图
- (F2Point * (^)(void))point;

/// 蜡烛图
- (F2Candle * (^)(void))candle;

/// 创建辅助对象
- (F2Guide * (^)(void))guide;

///  发送手势信息
- (F2Chart * (^)(NSDictionary *config))postTouchEvent;

/// 设置config, 用法是把json转换成config，然后一次性设置到C++中
- (F2Chart * (^)(NSDictionary *config))config;

/// 当使用renderConfig方法时，在config方法中的functionId及param会通过这个callback回调出来
- (F2Chart * (^)(FunctionItemCallback callback))callback;

#pragma mark 改变数据 改变Size
/// 改变画布和chart的大小，改变后需要重新设置并渲染
- (F2Chart * (^)(CGSize size))changeSize;

/// 改变引擎中的数据，改变后可以直接渲染，会使用上次的配置进行渲染
- (F2Chart * (^)(NSArray *data))changeData;

#pragma mark 关于度量Scale的一些设置
///设置当geom中有interval的时候，是否调整max, min, range三个参数, 默认是true
///adjust 是否调整，默认是调整的，可设置false 关闭
- (F2Chart * (^)(BOOL adjust))adjustScale;

///是否同步多个y轴的最值，默认为true
///sync 可设置false关闭
- (F2Chart * (^)(BOOL adjust))syncYScale;

#pragma mark 获取引擎中的参数和对象
/// 获取坐标系, 需要在render后才会生成F2Coordinate对象
- (F2Coordinate * (^)(void))getCoord;

/// 获取对应度量的 ticks 信息
/// @return [{@"text":"-3.00";@"tickValue": "-3";@"value": 0}]
- (NSArray<NSDictionary *> *(^)(NSString *field))getScaleTicks;

/// 计算某一项数据对应的在坐标系中的绝对坐标
/// @return [x, y]
- (CGPoint (^)(NSDictionary *itemData))getPosition;


#pragma mark 内部使用的方法 非必要不要调用
/// 把callback缓存到chart中
- (void)bindCallback:(F2Callback *)callback;

/// 内部分发functionId的方法 外部请勿调用
/// @param functionId 方法名字
/// @param param 参数 json格式
/// @return const char * json格式
- (const char *)execute:(const char *)functionId param:(const char *)param;

@end

NS_ASSUME_NONNULL_END
