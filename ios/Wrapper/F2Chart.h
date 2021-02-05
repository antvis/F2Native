#import "F2CallbackObj.h"
#import "F2CanvasView.h"
#import "F2Geom.h"
#import "F2Guide.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface F2Chart : NSObject

/// 快捷创建chart
/// @param size dp为单位
/// @param name 输出日志时会附加name
+ (F2Chart *)chart:(CGSize)size withName:(NSString *)name;

///  设置canvas。设置canvas时，需要先清屏，调用[[canvas getContext2D] clearScreen]
///  gcanvas的创建和销毁需要业务自己完成，清理不干净会有内存泄露
- (F2Chart * (^)(F2CanvasView *gcanvas))canvas;

/// 设置chart的padding
- (F2Chart * (^)(CGFloat left, CGFloat top, CGFloat right, CGFloat bottom))padding;

- (F2Chart * (^)(CGFloat left, CGFloat top, CGFloat right, CGFloat bottom))margin;

/// 设置chart的元数据
/// @param json JSON格式的数据
- (F2Chart * (^)(NSString *json))source;

/// 设置度量，
/// @param field 需要设置度量的字段
/// @param config config的具体字段待补充
- (F2Chart * (^)(NSString *field, NSDictionary *config))scale;

/// 设置轴
/// @param field 需要设置度量的字段
/// @param config config的具体字段待补充
- (F2Chart * (^)(NSString *field, NSDictionary *config))axis;

/// 设置图例
/// @param field 需要设置度量的字段
/// @param config config的具体字段待补充
- (F2Chart * (^)(NSString *field, NSDictionary *config))legend;

/// 设置坐标
/// @param config  坐标轴配置信息
- (F2Chart * (^)(NSDictionary *config))coord;

/// 指定交互类型
/// @param type --  pinch 缩放 | pan 平移
- (F2Chart * (^)(NSString *type, NSDictionary *config))interaction;

/// 开启 ToolTIp 功能
/// @param confg    具体字段待补充
- (F2Chart * (^)(NSDictionary *config))tooltip;

/// 开始渲染
- (F2Chart * (^)(void))render;

/// 清除所有元素
- (F2Chart * (^)(void))clear;

/// 重新渲染
- (F2Chart * (^)(void))repaint;
/// 获取渲染的信息
/// @return JSON格式，cmdCount表示渲染指令的个数，-1表示环境有问题，0表示指令数为0，>0表示正常的绘制指令个数
- (NSString * (^)(void))getRenderDumpInfo;

/// 计算某一项数据对应的在坐标系中的绝对坐标
/// @return [x, y]
- (NSArray<NSNumber *> * (^)(NSDictionary *itemData))getPosition;

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

- (void)bindF2CallbackObj:(F2CallbackObj *)callback;

@end

NS_ASSUME_NONNULL_END
