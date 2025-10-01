package com.antgroup.antv.f2;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;

/**
 * author by luna
 * date on 2023/8/31
 */

public class F2ImageLoadUtil {

    private static final String TAG = F2ImageLoadUtil.class.getSimpleName();

    private static final Handler HANDLER = new Handler(Looper.getMainLooper());

    public static void removeHandlerCallbacks() {
        F2Log.get().e(TAG, " removeHandlerCallbacks ");
        HandlerThreadFactory.getLoadBitmapThreadHandler().removeCallbacks(null);
        HANDLER.removeCallbacks(null);
    }

    // c++调用在线图片下载
    public static void loadImage(final String url, final long canvasImageHandler) {
        F2Log.get().e(TAG, " loadImage url: " + url);
        if (TextUtils.isEmpty(url) || canvasImageHandler == 0) {
            F2Log.get().e(TAG, " loadImage empty");
            return;
        }
        ImageLoadRunnable loadRunnable = new ImageLoadRunnable(url, canvasImageHandler);
        HandlerThreadFactory.getLoadBitmapThreadHandler().post(loadRunnable);
    }

    private static class ImageLoadRunnable implements Runnable {

        private String mPath;
        private long mCanvasImageHandler;

        public ImageLoadRunnable(String path, final long canvasImageHandler) {
            mPath = path;
            mCanvasImageHandler = canvasImageHandler;
        }

        @Override
        public void run() {
            if (mPath != null) {
                try {
                    URL url = new URL(mPath);
                    HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                    InputStream is = conn.getInputStream();
                    Bitmap loadedBitmap = BitmapFactory.decodeStream(is);
                    loadImageFinish(loadedBitmap);
                } catch (Exception e) {
                    F2Log.get().e(TAG, " ImageLoadRunnable exception: " + e.getMessage());
                }
            }
        }

        private void loadImageFinish(final Bitmap loadedBitmap) {
            F2Log.get().e(TAG, " loadImageFinish start ");
            HANDLER.post(new Runnable() {
                @Override
                public void run() {
                    if (mCanvasImageHandler != 0 && loadedBitmap != null) {
                        NativeChartProxy.nLoadImageResult(mCanvasImageHandler, loadedBitmap, true);
                    }
                }
            });
        }
    }

}
