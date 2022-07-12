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
@property (nonatomic, assign) CGImageRef cgImage;

@end

@implementation F2CanvasContext

void F2ProviderReleaseData (void *info, const void *data, size_t size) {
    free((void*)data);
}

-(instancetype)initWithSize:(CGSize)size {
    if (self = [super init]) {
        int width = size.width * F2NativeScale;
        int height = size.height * F2NativeScale;
        [self create:CGSizeMake(width, height)];
    }
    return self;
}

- (void)changeSize:(CGSize)size {
    //oriSize被F2NativeScale扩大过了, newSize也要扩大
    CGSize oriSize = CGSizeMake(CGBitmapContextGetWidth(self.cgContext), CGBitmapContextGetHeight(self.cgContext));
    CGSize newSize = CGSizeMake(size.width * F2NativeScale, size.height * F2NativeScale);
    if (!CGSizeEqualToSize(newSize, oriSize)) {
        [self destroy];
        [self create:newSize];
    }
}

- (void)create:(CGSize)size {
    //基础信息
    CGSize newSize = CGSizeMake(MAX(size.width , 1), MAX(size.height, 1));
    uint32_t* rgbImageBuf = (uint32_t*)malloc(newSize.width * newSize.height * 4);
    self.cgImage = [self imageWithSize:newSize data:rgbImageBuf];
    self.cgContext = [self contextWithSize:newSize data:rgbImageBuf];
}

- (CGContextRef) contextWithSize:(CGSize)size data:(uint32_t *)rgbImageBuf {
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast;
    CGColorSpaceRef spaceRef = CGColorSpaceCreateDeviceRGB();
    //画布
    CGContextRef context = CGBitmapContextCreate(rgbImageBuf, size.width, size.height, 8, size.width * 4, spaceRef, bitmapInfo);
    //设置rgb空间 否则图像为灰色
    CGContextSetFillColorSpace(context,  spaceRef);
    CGContextSetStrokeColorSpace(context,  spaceRef);
    CGColorSpaceRelease(spaceRef);
    return context;
}

- (CGImageRef)imageWithSize:(CGSize)size data:(uint32_t *)rgbImageBuf {
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast;
    size_t bytesPerRow = size.width * 4;
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, rgbImageBuf, bytesPerRow * size.height, F2ProviderReleaseData);
    CGColorSpaceRef spaceRef = CGColorSpaceCreateDeviceRGB();
    //bitmap
    CGImageRef image = CGImageCreate(size.width, size.height, 8, 32, bytesPerRow, spaceRef, bitmapInfo, provider, NULL, true, kCGRenderingIntentDefault);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(spaceRef);
    return image;
}

-(void)dealloc {
    [self destroy];
}

-(void)destroy {
    if (self.cgContext) {
        CGContextRelease(self.cgContext);
        self.cgContext = nil;
    }
    
    if (self.cgImage) {
        CGImageRelease(self.cgImage);
        self.cgImage = nil;
    }
}

-(CGImageRef)bitmap {
    return self.cgImage;
}

-(CGContextRef)context2d {
    return self.cgContext;
}

- (CGFloat)nativeScale {
    return F2NativeScale;
}
@end
