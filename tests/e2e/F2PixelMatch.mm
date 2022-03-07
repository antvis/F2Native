//
//  PixelMatch.m
//  F2Tests
//
//  Created by weiqing.twq on 2022/1/13.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#import "F2PixelMatch.h"
#import "pixelmatch.hpp"

@implementation F2PixelMatch

+(bool)match:(UIImage *)image1 image2:(UIImage *)image2 outImage:(UIImage **)outImage {
    if (!image1) {
        NSLog(@"F2PixelMatch#match image1 is nil");
        return NO;
    }
    
    if (!image2) {
        NSLog(@"F2PixelMatch#match image2 is nil");
        return NO;
    }
    
    if (!CGSizeEqualToSize(image1.size, image2.size)) {
        NSLog(@"image1 size(%@) should be equal to image2 size(%@", NSStringFromCGSize(image1.size), NSStringFromCGSize(image2.size));
        return NO;
    }
    
    //转换成jpeg
    NSData *data1 =  UIImageJPEGRepresentation(image1, 1);
    NSData *data2 =  UIImageJPEGRepresentation(image2, 1);

    UIImage *jpeg1 = [UIImage imageWithData:data1];
    UIImage *jpeg2 = [UIImage imageWithData:data2];
    
    //not release provider
    CFDataRef rawData1 = CGDataProviderCopyData(CGImageGetDataProvider(jpeg1.CGImage));
    CFDataRef rawData2 = CGDataProviderCopyData(CGImageGetDataProvider(jpeg2.CGImage));
    
    std::size_t size = image1.size.width * image1.size.height * 4;
    uint8_t *actualDiff = (uint8_t *)calloc(1, size);
    
    uint64_t match = mapbox::pixelmatch((const uint8_t *)CFDataGetBytePtr(rawData1),
                                        (const uint8_t *)CFDataGetBytePtr(rawData2),
                                        image1.size.width,
                                        image1.size.height,
                                        actualDiff);
    //看实际使用情况 可以设置threshold = size * 0.05 5%容错率
    int threshold = 0;
    
    if (match > threshold) {
        CGColorSpaceRef colorSpace=CGColorSpaceCreateDeviceRGB();
          CGContextRef bitmapContext=CGBitmapContextCreate((void *)actualDiff, image1.size.width, image1.size.height, 8, 4*image1.size.width, colorSpace,  kCGImageAlphaPremultipliedLast | kCGBitmapByteOrderDefault);
          CFRelease(colorSpace);
          free(actualDiff);
          CGImageRef cgImage=CGBitmapContextCreateImage(bitmapContext);
          CGContextRelease(bitmapContext);
          
          *outImage = [UIImage imageWithCGImage:cgImage];
          CGImageRelease(cgImage);
    }
        
    return match == threshold;
}

@end
