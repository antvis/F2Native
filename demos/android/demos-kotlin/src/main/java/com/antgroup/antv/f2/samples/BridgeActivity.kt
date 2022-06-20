package com.antgroup.antv.f2.samples

import android.os.Bundle
import android.view.MenuItem
import android.view.View
import android.widget.Button
import android.widget.FrameLayout
import android.widget.LinearLayout
import androidx.appcompat.app.AppCompatActivity
import com.alibaba.fastjson.JSONObject
import com.antgroup.antv.f2.samples.Utils.loadAssetFile
import java.util.*

/**
 * author by luna
 * date on 2022/5/29
 */
class BridgeActivity : AppCompatActivity() {
    private var mControl1: BridgeControl? = null
    private var mControl2: BridgeControl? = null
    private var mContainer: LinearLayout? = null
    private val mContainer2: FrameLayout? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar!!.setDisplayHomeAsUpEnabled(true)
        setContentView(R.layout.activity_bridge)
        init()
    }

    private var mBirdgeMap: HashMap<String, Any>? = null
    private fun init() {
        mContainer = findViewById(R.id.container)
        mBirdgeMap = HashMap()
        val attrsMap = HashMap<String, Any>()
        attrsMap["needToolTip"] = "true"
        attrsMap["longPress"] = "true"
        mBirdgeMap!!["attrs"] = attrsMap
        init1()
        init2()
        init3()
        findViewById<View>(R.id.destroy).setOnClickListener { startDestroy() }
    }

    private fun init3() {
//        mContainer2 = findViewById(R.id.container2);
//        Button button = findViewById(R.id.render3);
//
//        mContainer2.setVisibility(View.GONE);
//        button.setVisibility(View.GONE);
//        button.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                startRender3();
//            }
//        });
    }

    private fun init1() {
        mControl1 = BridgeControl()
        val widgetView = mControl1!!.createWidgetView(this, mBirdgeMap, 400, 300)
        mContainer!!.addView(widgetView)
        startRender()
        val button = findViewById<Button>(R.id.render1)
        button.visibility = View.GONE
        button.setOnClickListener { startRender() }
    }

    private fun startRender() {
        val `object` = JSONObject()
        val data = loadAssetFile(this, "mock_cube_data.json")
        `object`["chartConfig"] = data
        val item = JSONObject()
        item["date"] = 1653634800000L
        item["price"] = 3130.2394
        `object`["getPosition"] = item
        mControl1!!.invokeMethod("render", `object`)
    }

    private fun init2() {
        mControl2 = BridgeControl()
        val widgetView = mControl2!!.createWidgetView(this, mBirdgeMap, 400, 300)
        mContainer!!.addView(widgetView)
        startRender2()
        val button = findViewById<Button>(R.id.render2)
        button.visibility = View.GONE
        button.setOnClickListener { startRender2() }
    }

    private fun startRender2() {
        val `object` = JSONObject()
        val data = loadAssetFile(this, "mock_cube_data2.json")
        `object`["chartConfig"] = data
        mControl2!!.invokeMethod("render", `object`)
    }

    private fun startRender3() {
//        JSONObject object = new JSONObject();
//        String data = Utils.loadAssetFile(this, "mock_cube_data.json");
//        object.put("chartConfig", data);
//        mControlV2.invokeMethod("render", object);
    }

    private fun startDestroy() {
        mControl1!!.invokeMethod("destroy", null)
        mControl2!!.invokeMethod("destroy", null)
    }

    override fun onDestroy() {
        super.onDestroy()
        startDestroy()
        mControl1!!.onDestroy()
        mControl2!!.onDestroy()
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        if (item.itemId == android.R.id.home) {
            finish()
        }
        return super.onOptionsItemSelected(item)
    }
}