package com.antgroup.antv.f2;

import android.content.Context;

import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.*;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class F2InstrumentedTest {
    @Test
    public void runAllTest() {
        if(BuildConfig.DEBUG) {
            System.loadLibrary("f2tests");
            F2TestProxy test = new F2TestProxy();

            //所有c++的单测都在JNI的runTest中
            assertEquals(test.runTest(), 1);
        }
    }
}