//
//  F2TestUtil.h
//  F2Tests
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface F2TestUtil : NSObject

+ (BOOL)saveImage:(UIImage *)image;

+ (UIImage *)readImage:(NSString *)imageName;

@end

NS_ASSUME_NONNULL_END
