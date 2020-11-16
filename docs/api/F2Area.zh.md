---
title: F2Area
order: 5
---

图形区域对象，用于绘制区域范围

## 创建 F2Area 实例

```
  chart.area();
```
<br/>

## 方法
### - position 
```
  chart.area().position("date*value");
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `attr`| String | X*Y：绘制的区域在source中对应的名称

<br/>

### - fixedColor 
设置统一的区域填充的颜色
```
  chart.area().fixedColor("#108EE9");
```
- 参数

| **属性名** |  **类型** | **解释** |
| --- | --- | --- |
| `attr`| String | 设置区域填充的颜色 （Hex rgba）
