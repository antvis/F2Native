package com.antgroup.antv.f2.kotlinsamples;

import android.content.Context;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * @author qingyuan.yl
 * @date 2020-09-24
 */
public class Utils {

    public static String loadAssetFile(Context context, String assetFile) {
        try {
            InputStream is = context.getAssets().open(assetFile);

            byte[] buf = new byte[1024 * 500];

            ByteArrayOutputStream output = new ByteArrayOutputStream();
            int size = 0;
            while ((size = is.read(buf, 0, buf.length)) >= 0) {
                output.write(buf, 0, size);
            }

            return new String(output.toByteArray());
        } catch (IOException e) {
        }
        return null;
    }
}
