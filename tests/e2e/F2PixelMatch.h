//
//  PixelMatch.h
//  F2Tests
//
//  Created by weiqing.twq on 2022/1/13.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface F2PixelMatch : NSObject

+(bool)match:(UIImage *)image1 image2:(UIImage *)image2 outImage:(UIImage **)outImage;

@end

NS_ASSUME_NONNULL_END
