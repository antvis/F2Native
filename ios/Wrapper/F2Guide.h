#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface F2Guide : NSObject

- (instancetype)initWithGuide:(void *)guide;

/// 创建旗子，config格式如下
/// @{ @"position": @[@(x轴的值), @(y轴的值)], @"content": @"内容",@"padding": @(2) }
- (F2Guide * (^)(NSDictionary *config))flag;

/// 创建文字，config格式如下
/// @{ @"position": @[@"min", @"max"], @"content": @"内容", @"color": @"#808080", @"margin": @[@(-5), @(-13)]
- (F2Guide * (^)(NSDictionary *config))text;

/// 创建线，config格式如下
/// @{ @"position": @[@"min", @"max"], @"orientation": @"horizontal", @"color": @"#808080",  @"lineWdith": @(1), @"dash": @[...]
- (F2Guide * (^)(NSDictionary *config))line;

@end

NS_ASSUME_NONNULL_END
