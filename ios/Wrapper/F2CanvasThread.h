//
//  F2CanvasThread.h
//  F2
//
//  Created by XiaLuo on 2021/2/7.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface F2CanvasThread : NSObject

@property (nonatomic, copy) NSString *name;

- (instancetype)initWithAsync:(BOOL)async;

- (void)runBlockSyncOnExecuteThread:(void (^)(void))block;

- (void)runBlockSyncOnExecuteThread:(void (^)(void))block forcePost:(BOOL)force;

- (void)runBlockASyncOnExecuteThread:(void (^)(void))block;

- (void)runBlockASyncOnExecuteThread:(void (^)(void))block forcePost:(BOOL)force;

- (void)runBlockASyncOnExecuteThread:(void (^)(void))block delay:(NSTimeInterval)timeInterval;

- (BOOL)isMainThread;

@end

NS_ASSUME_NONNULL_END
