//
//  F2CanvasThread.m
//  F2
//
//  Created by XiaLuo on 2021/2/7.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "F2CanvasThread.h"

@implementation F2CanvasThread {
    NSThread *_execThread;
}

- (void)dealloc {
    CFRunLoopStop([[NSRunLoop currentRunLoop] getCFRunLoop]);
    _execThread = nil;
}

- (instancetype)initWithAsync:(BOOL)async {
    self = [super init];
    if(self) {
        if(async) {
            _execThread = [[NSThread alloc] initWithTarget:self selector:@selector(runRunLoopThread) object:nil];
            [_execThread start];
        } else {
            _execThread = [NSThread mainThread];
        }
    }
    return self;
}

- (void)setName:(NSString *)name {
    _name = name;
    if(![_execThread isMainThread]) {
        _execThread.name = _name;
    }
}

- (void)runRunLoopThread {
    CFRunLoopSourceContext ctx = {0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    CFRunLoopSourceRef source = CFRunLoopSourceCreate(NULL, 0, &ctx);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), source, kCFRunLoopDefaultMode);
    CFRelease(source);

    while(kCFRunLoopRunStopped !=
          CFRunLoopRunInMode(kCFRunLoopDefaultMode, ((NSDate *)[NSDate distantFuture]).timeIntervalSinceReferenceDate, NO)) {
    }
}

- (void)runBlockSyncOnExecuteThread:(void (^)())block {
    [self runBlockSyncOnExecuteThread:block forcePost:NO];
}

- (void)runBlockSyncOnExecuteThread:(void (^)(void))block forcePost:(BOOL)force {
    if(!force && [NSThread currentThread] == _execThread) {
        block();
    } else {
        [self performSelector:@selector(runBlockSyncOnExecuteThread:)
                     onThread:_execThread
                   withObject:[block copy]
                waitUntilDone:YES];
    }
}

- (void)runBlockASyncOnExecuteThread:(void (^)())block {
    [self runBlockASyncOnExecuteThread:block forcePost:NO];
}

- (void)runBlockASyncOnExecuteThread:(void (^)(void))block forcePost:(BOOL)force {
    if(!force && [NSThread currentThread] == _execThread) {
        block();
    } else {
        [self performSelector:@selector(runBlockSyncOnExecuteThread:)
                     onThread:_execThread
                   withObject:[block copy]
                waitUntilDone:NO];
    }
}

- (void)runBlockASyncOnExecuteThread:(void (^)(void))block delay:(NSTimeInterval)timeInterval {

    [self performSelector:@selector(runBlockSyncOnExecuteThread:) withObject:block afterDelay:timeInterval / 1000];
}

- (BOOL)isMainThread {
    return [_execThread isMainThread];
}

@end
