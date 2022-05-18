//
//  F2CoreGraphicsCanvasContext.m
//  F2
//
//  Created by weiqing.twq on 2021/12/8.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "F2CoreGraphicsCanvasContext.h"
#import "F2Utils.h"
#import "xtime.h"

@interface F2CoreGraphicsCanvasContext()
@property (nonatomic, strong) UIView *contextView;
@property (nonatomic, assign) CGContextRef cgContext;

@end

@implementation F2CoreGraphicsCanvasContext

- (void)createContextWithFrame:(CGRect)frame complete:(void (^)(F2CanvasContext *))callback {
    long long start = xg::CurrentTimestampAtMM();
    self.contextView = [[UIView alloc] initWithFrame:frame];
    self.contextView.layer.masksToBounds = YES;
    
    CGColorSpaceRef spaceRef = CGColorSpaceCreateDeviceRGB();
    //原点在左下
    CGContextRef context = CGBitmapContextCreate(nil, frame.size.width * F2NativeScale, frame.size.height * F2NativeScale, 8, 0, spaceRef, kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedFirst);
       
    NSCAssert(context, @"F2CoreGraphicsCanvasContext#error:CGContextRef is nil");
    if (!context) {
        CGColorSpaceRelease(spaceRef);
        NSLog(@"F2CoreGraphicsCanvasContext#error:CGContextRef is nil");
        return;
    }
    
    //设置rgb空间 否则图像为灰色
    CGContextSetFillColorSpace(context,  spaceRef);
    CGContextSetStrokeColorSpace(context,  spaceRef);
    CGColorSpaceRelease(spaceRef);
    
    //CGContext原点在左下角 翻转到左上角
    CGContextTranslateCTM(context, 0, frame.size.height * F2NativeScale);
    CGContextScaleCTM(context, 1, -1);

    self.cgContext = context;
    NSLog(@"F2CoreGraphicsCanvasContext#Create GCanvas Context cost %lld ms", (xg::CurrentTimestampAtMM() - start));
    
    if(callback) callback(self);
}

-(BOOL)draw {
    if (!self.cgContext) {
        return NO;
    }
    
    CGImageRef imageRef = CGBitmapContextCreateImage(self.cgContext);
    NSCAssert(imageRef, @"F2CoreGraphicsCanvasContext#draw:imageRef is nil");
    if (!imageRef) {
        return NO;
    }
    
    self.contextView.layer.contents = (__bridge id)imageRef;
    
    CGImageRelease(imageRef);
    return YES;
}

-(void)destroy {
    if (!self.cgContext) {
        return;
    }
    CGContextRelease(self.cgContext);
    self.cgContext = nil;
}

-(void *)context2d {
    return (void *)self.cgContext;
}

- (UIView *)contextView {
    return _contextView;
}

- (UIImage *)snapshot {
    if (!self.cgContext) {
        return nil;
    }
    CGImageRef imageRef = CGBitmapContextCreateImage(self.cgContext);
    NSCAssert(imageRef, @"F2CoreGraphicsCanvasContext#snapshot:imageRef is nil");
    UIImage *snapshot = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    return snapshot;
}
@end
