---
title: F2Guide
order: 7
---

图形辅助对象，用于在特定点位进行标注

## 创建 F2Guide 实例

```
  chart.guide();
```
<br/>

## 方法
### - flag
绘制旗子辅助图形
#### iOS
```obj-c
  chart.guide().flag(
                     @{
                        @"color":@"#108EE9",
                        @"position": @[@(1), @(99.9)],
                        @"content": @"flagDemo",
                        @"padding": @[@(5), @(3), @(5), @(3)] ,
                        @"backgroundColor":@"#C7E7FF",
                        @"textColor":@"#108EE9",
                        @"textSize": @(10.f)
                      });
```
#### Android
```java
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `color`| String | 旗子辅助标志的颜色（Hex rgb）
| `position`| Array&lt;Number&gt; | 辅助标志绘制的位置 <br/>padding[0] 表示X轴所在的位置  <br/> padding[1] 表示Y轴所在的位置
| `padding`| Array&lt;Number&gt; |  辅助标志绘制的位置 <br/>padding[0] 左  <br/> padding[1] 上  <br/>padding[2] 右 <br/> padding[3] 下
| `backgroundColor`| String | 旗子辅助标志的背景色  （Hex rgb）
| `textColor`| String | 文字颜色
| `textSize` | Number | 文字大小
<br/>

### - text
绘制文字辅助图形
#### iOS
```obj-c
chart.guide().text(
               @{
                 @"position": @[@(1), @(99.9)],
                 @"content": @"flagDemo",
                 @"padding": @[@(5), @(3), @(5), @(3)] ,
                 @"textColor":@"#108EE9",
                 @"textSize": @(10.f)
                 });
```

#### Android
```java
```

- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `position`| Array&lt;Number&gt; | 辅助标志绘制的位置 <br/>padding[0] 表示X轴所在的位置  <br/> padding[1] 表示Y轴所在的位置
| `padding`| Array&lt;Number&gt; |  辅助标志绘制的位置 <br/>padding[0] 左  <br/> padding[1] 上  <br/>padding[2] 右 <br/> padding[3] 下
| `textColor`| String | 文字颜色
| `textSize` | Number | 文字大小
<br/>







