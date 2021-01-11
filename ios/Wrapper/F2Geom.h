#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface F2Geom : NSObject

- (instancetype)initWithGeom:(void *)geom;

/// 设置几何对象的x轴*y轴
- (F2Geom * (^)(NSString *field))position;

/// 设置某一个对象的颜色，数组匹配，支付宝95版本前及财富支持支持RGB格式（#528EFF），支付宝95及版本后支持RGBA格式（#528EFFFF)
- (F2Geom * (^)(NSString *field, NSArray<NSString *> *colors))color;

/// 设置对象的颜色，常量颜色，支付宝95版本前及财富支持支持RGB格式（#528EFF），支付宝95及版本后支持RGBA格式（#528EFFFF)
- (F2Geom * (^)(NSString *color))fixedColor;

/// size('field', [ ${min}, ${max} ])，指定映射到 size 字段外，还提供了 size
/// 的最大值和最小值范围；sizes为空默认使用使用内置的默认大小范围为 [4, 10]；size 单位dp
- (F2Geom * (^)(NSString *field, NSArray<NSNumber *> *sizes))size;

///直接指定像素大小。单位dp
- (F2Geom * (^)(float size))fixedSize;

/// shape('field', shapes)，用户自己提供 shapes 数据，来进行数据映射；
- (F2Geom * (^)(NSString *field, NSArray<NSString *> *shapes))shape;

/// shape('circle')，指定常量，将所有数据值映射到固定的 shape。
- (F2Geom * (^)(NSString *shape))fixedShape;

/// 设置数据是否进行累加处理  适合饼图 层叠柱状图
- (F2Geom * (^)(NSString *adjust))adjust;

- (F2Geom * (^)(NSDictionary *config))style;

- (void *)getGeom;

@end

@interface F2Line : F2Geom

@end

@interface F2Interval : F2Geom

- (F2Interval * (^)(NSDictionary *config))tag;

@end

@interface F2Area : F2Geom

@end

@interface F2Point : F2Geom

@end

NS_ASSUME_NONNULL_END
