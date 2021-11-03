//
//  F2CanvasThread.h
//  F2
//
//  Created by XiaLuo on 2021/2/7.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//


#import <Foundation/Foundation.h>

#if defined(PRODUCT_WALLET)
#import <AntCanvas/AntCanvasThread.h>
#endif

NS_ASSUME_NONNULL_BEGIN

#if defined(PRODUCT_WALLET)

@interface F2CanvasThread : AntCanvasThread
@end
#else
@interface F2CanvasThread : NSObject

@property (nonatomic, strong) NSThread *myThread;

- (instancetype)initWithName:(NSString *)name;

- (void)performBlockASyncOnThread:(void (^)(void))block;

/// 需要调用 destroy 来停止 runloop
- (void)destroy;

@end
#endif

NS_ASSUME_NONNULL_END


