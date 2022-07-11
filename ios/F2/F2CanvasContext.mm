//
//  F2CoreGraphicsCanvasContext.m
//  F2
//
//  Created by weiqing.twq on 2021/12/8.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "F2CanvasContext.h"
#import "F2Utils.h"

@interface F2CanvasContext()
@property (nonatomic, assign) CGContextRef cgContext;
@end

@implementation F2CanvasContext

- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super init]) {
        CGSize size = CGSizeMake(frame.size.width * F2NativeScale, frame.size.height * F2NativeScale);
        self.cgContext = [self contextWithSize:size];
    }
    return self;
}

- (void)dealloc {
    if (_cgContext) {
        CGContextRelease(_cgContext);
        _cgContext = nil;
    }
}

- (void)setFrame:(CGRect)frame {
    //size被F2NativeScale扩大过了
    CGSize oriSize = CGSizeMake(CGBitmapContextGetWidth(self.cgContext), CGBitmapContextGetHeight(self.cgContext));
    CGSize newSize = CGSizeMake(frame.size.width * F2NativeScale, frame.size.height * F2NativeScale);
    if (!CGSizeEqualToSize(newSize, oriSize)) {
        CGContextRelease(self.cgContext);
        self.cgContext = [self contextWithSize:newSize];
    }
}

- (CGContextRef)contextWithSize:(CGSize)size {
    CGFloat width = MAX(size.width, 1);
    CGFloat height = MAX(size.height, 2);
    CGColorSpaceRef spaceRef = CGColorSpaceCreateDeviceRGB();
    //原点在左下
    CGContextRef context = CGBitmapContextCreate(nil, width, height, 8, 0, spaceRef, kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedFirst);
       
    NSCAssert(context, @"F2CanvasContext#error:CGContextRef is nil");
    if (!context) {
        CGColorSpaceRelease(spaceRef);
        NSLog(@"F2CanvasContext#error:CGContextRef is nil");
        return nil;
    }
    
    //设置rgb空间 否则图像为灰色
    CGContextSetFillColorSpace(context,  spaceRef);
    CGContextSetStrokeColorSpace(context,  spaceRef);
    CGColorSpaceRelease(spaceRef);
    
    //CGContext原点在左下角 翻转到左上角
    CGContextTranslateCTM(context, 0, height);
    CGContextScaleCTM(context, 1, -1);
    return context;
}

-(CGContextRef)context2d {
    return self.cgContext;
}

- (UIImage *)snapshot {
    if (!self.cgContext) {
        return nil;
    }
    CGImageRef imageRef = CGBitmapContextCreateImage(self.cgContext);
    NSCAssert(imageRef, @"F2CanvasContext#snapshot:imageRef is nil");
    UIImage *snapshot = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    return snapshot;
}

- (CGFloat)nativeScale {
    return F2NativeScale;
}
@end
