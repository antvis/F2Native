# F2Native 16KB页面大小支持完整指南

## 当前状态 ✅

经过检查，F2Native项目已经具备完整的16KB页面大小支持：

### 已完成的配置
- ✅ **NDK 27.2.12479018** - 支持16KB页面大小
- ✅ **AGP 8.4.2** - 支持16KB对齐和打包
- ✅ **android.zipAlignPageSize=16384** - Gradle属性已设置
- ✅ **链接器标志** - CMakeLists.txt中已添加16KB对齐标志

### 具体修改内容

#### 1. gradle.properties
```properties
android.useAndroidX=true
android.injected.testOnly=false
alipay.useJDK17=true
org.gradle.jvmargs=-Xms8G -Xmx12G
android.zipAlignPageSize=16384  # 已添加
```

#### 2. build.gradle
```gradle
rootProject.extensions.sdks = [
    'ndkVersion': "27.2.12479018"  # 已升级到NDK 27
]
```

#### 3. android/f2native/CMakeLists.txt
```cmake
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections -flto=thin -Wl,-z,max-page-size=16384 -Wl,-z,common-page-size=16384")
```

## 验证步骤

### 1. 构建验证
```bash
# 清理并重新构建
cd android
./gradlew clean
./gradlew assembleRelease

# 验证构建产物
find f2native/build -name "*.so" -type f | xargs -I {} readelf -l {} | grep "LOAD.*0x4000"
```

### 2. 运行时验证
```bash
# 检查设备是否支持16KB页面大小
adb shell getconf PAGE_SIZE

# 在16KB设备上测试应用启动
adb logcat | grep -i "f2native"
```

### 3. 兼容性测试
- 在Android 15+ 16KB设备上测试
- 在现有4KB设备上验证向后兼容性
- 检查所有支持的ABI：armeabi-v7a, arm64-v8a

## 注意事项

### 1. 向后兼容性
16KB页面大小支持是向后兼容的，在4KB设备上仍然可以正常运行。

### 2. 性能影响
- 16KB对齐可能会略微增加.so文件大小
- 启动性能基本不受影响
- 内存使用效率可能略有提升

### 3. 构建时间
- 由于对齐要求，构建时间可能略微增加
- 建议使用增量构建来优化开发体验

## 故障排除

### 常见问题
1. **构建失败**：确保NDK 27+和AGP 8.4.2+已正确安装
2. **运行时崩溃**：检查设备是否支持16KB页面大小
3. **兼容性问题**：验证所有ABI都正确配置了16KB标志

### 调试工具
```bash
# 检查ELF文件头
readelf -l libf2.so

# 检查页面大小
adb shell getconf PAGE_SIZE

# 检查内存映射
adb shell cat /proc/self/maps
```

## 总结

F2Native项目现在已经完全支持16KB页面大小，所有必要的配置都已到位。建议进行完整的构建和测试验证，确保在16KB设备上的兼容性。