//
//  F2CanvasThread.m
//  F2
//
//  Created by XiaLuo on 2021/2/7.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "F2CanvasThread.h"

@implementation F2CanvasThread

#if defined(PRODUCT_WALLET)

#else
- (instancetype)initWithName:(NSString *)name {
    self = [super init];
    if(self) {
        self.myThread = [[NSThread alloc] initWithTarget:self selector:@selector(runRunLoopThread) object:nil];
        [self.myThread start];

    }
    return self;
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

- (void)performBlockASyncOnThread:(void (^)())block {
    [self runBlockSyncOnExecuteThread:block forcePost:NO];
}

- (void)runBlockSyncOnExecuteThread:(void (^)(void))block forcePost:(BOOL)force {
    if(!force && [NSThread currentThread] == self.myThread) {
        block();
    } else {
        [self performSelector:@selector(performBlockASyncOnThread:)
                     onThread:self.myThread
                   withObject:[block copy]
                waitUntilDone:YES];
    }
}

- (void)destroy; {
    CFRunLoopStop([[NSRunLoop currentRunLoop] getCFRunLoop]);
    self.myThread = nil;
}

#endif

@end
