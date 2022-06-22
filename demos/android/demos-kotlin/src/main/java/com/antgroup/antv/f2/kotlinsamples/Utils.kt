package com.antgroup.antv.f2.kotlinsamples

import android.content.Context
import java.io.ByteArrayOutputStream
import java.io.IOException

/**
 * @author weiqing.tang
 * @date 2022-06-20
 */
object Utils {
    @JvmStatic
    fun loadAssetFile(context: Context, assetFile: String?): String? {
        try {
            val `is` = context.assets.open(assetFile)
            val buf = ByteArray(1024 * 500)
            val output = ByteArrayOutputStream()
            var size = 0
            while (`is`.read(buf, 0, buf.size).also { size = it } >= 0) {
                output.write(buf, 0, size)
            }
            return String(output.toByteArray())
        } catch (e: IOException) {
        }
        return null
    }
}