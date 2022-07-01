//
//  F2TestUtil.m
//  F2Tests
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#import "F2TestUtil.h"

@implementation F2TestUtil

+ (BOOL)saveImage:(UIImage *)image name:(NSString *)name {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES);
    NSString *filePath = [[paths objectAtIndex:0]stringByAppendingPathComponent:name];  // 保存文件的名称
    NSLog(@"saveImage: %@", filePath);
    BOOL result =[UIImageJPEGRepresentation(image, 1) writeToFile:filePath  atomically:YES]; // 保存成功会返回YES
    NSCAssert(result, @"saveImage failure");
    return result;
}

+ (UIImage *)readImage:(NSString *)imageName {
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *jsonPath = [bundle pathForResource:imageName ofType:nil];
    return [[UIImage alloc] initWithContentsOfFile:jsonPath];
}

@end
