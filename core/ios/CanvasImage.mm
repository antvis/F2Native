//
//  CanvasImage.cpp
//  F2
//
//  Created by weiqing.twq on 2022/1/17.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#if defined(__APPLE__)

#include "CanvasImage.h"
#include <UIKit/UIKit.h>
#include <CoreGraphics/CoreGraphics.h>

typedef void (^F2OnLoadCallback)(UIImage *_Nonnull image);

@interface F2CanvasImageProxy : NSObject

@end

@implementation F2CanvasImageProxy

-(void)onLoad:(NSString *)urlStr callback:(F2OnLoadCallback) callback {
    //todo 在支付宝域内可借助域内的图片下载能力 提供更好的体验
    NSURL *url = [NSURL URLWithString:urlStr];
    
    dispatch_queue_t asynchronousQueue = dispatch_queue_create("imageDownloadQueue", NULL);
    //创建异步线程
    dispatch_async(asynchronousQueue, ^{
        //网络下载图片  NSData格式
        NSError *error;
        NSData *imageData = [NSData dataWithContentsOfURL:url options:NSDataReadingMappedIfSafe error:&error];
        UIImage *image = [UIImage imageWithData:imageData];
        
        //回到主线程更新UI 已经释放了锁
        //TODO 判断this对象是否被释放了
        if(!error && image && callback) {
            dispatch_async(dispatch_get_main_queue(), ^{
                callback(image);
            });
        }
    });
}

@end

using namespace xg;
using namespace xg::canvas;

CanvasImage::~CanvasImage() {
    if (image_) {
        CGImageRelease(CGImageRef(image_));
        image_ = nil;
    }
    
    F2CanvasImageProxy *proxy = (__bridge_transfer F2CanvasImageProxy *)downloader_;
    proxy = nullptr;
}

void CanvasImage::OnLoad(std::function<void()> finished) {
    if (!downloader_) {
        downloader_ = (__bridge_retained void *)[[F2CanvasImageProxy alloc] init];
    }
    
    F2CanvasImageProxy *proxy = (__bridge F2CanvasImageProxy *)downloader_;
    
    [proxy onLoad:[NSString stringWithUTF8String:url_.c_str()] callback:^(UIImage * _Nonnull image) {
        image_ = image.CGImage;
        CGImageRetain(CGImageRef(image_));
        if(finished) finished();
    }];
}

float CanvasImage::GetWidth() {
    return CGImageGetWidth((CGImageRef)image_);
}

float CanvasImage::GetHeight() {
    return CGImageGetHeight((CGImageRef)image_);
}

#endif //__APPLE__
