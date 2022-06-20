package com.antgroup.antv.f2.samples

import com.antgroup.antv.f2.F2CanvasView
import java.io.Serializable

/**
 * @author weiqing.tang
 * @date 2022-06-20
 */
class ChartModel : Serializable {
    @JvmField
    var title: String
    @JvmField
    var adapterClass: Class<out F2CanvasView.Adapter?>
    var horizontal = false

    constructor(title: String, adapterClass: Class<out F2CanvasView.Adapter?>) {
        this.title = title
        this.adapterClass = adapterClass
    }

    constructor(
        title: String,
        adapterClass: Class<out F2CanvasView.Adapter?>,
        horizontal: Boolean
    ) {
        this.title = title
        this.adapterClass = adapterClass
        this.horizontal = horizontal
    }
}