# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
-dontshrink
-dontoptimize
-dontpreverify
-keep public class com.antgroup.antv.f2.** {
    public <methods>;
    public <fields>;
    public <init>();
    public <init>(***);
    native <methods>;
}

-keep public class com.antgroup.antv.f2.F2Function {
    protected final java.lang.String nExecute(java.lang.String);
}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile
