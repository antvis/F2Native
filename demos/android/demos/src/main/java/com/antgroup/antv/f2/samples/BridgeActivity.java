package com.antgroup.antv.f2.samples;

import java.util.HashMap;

import android.os.Bundle;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

import androidx.appcompat.app.AppCompatActivity;

import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2CanvasView;

/**
 * author by luna
 * date on 2022/5/29
 */

public class BridgeActivity extends AppCompatActivity {

    private BridgeControl mControl1;
    private BridgeControl mControl2;
    private LinearLayout mContainer;
    private FrameLayout mContainer2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        setContentView(R.layout.activity_bridge);
        init();
    }

    private HashMap<String, Object> mBirdgeMap;

    private void init() {
        mContainer = findViewById(R.id.container);
        mBirdgeMap = new HashMap<>();
        HashMap<String, Object> attrsMap = new HashMap<>();
        attrsMap.put("needToolTip", "true");
        attrsMap.put("longPress", "true");

        mBirdgeMap.put("attrs", attrsMap);
        init1();
        init2();
        init3();

        findViewById(R.id.destroy).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startDestroy();
            }
        });
    }

    private void init3() {
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

    private void init1() {
        mControl1 = new BridgeControl();
        F2CanvasView widgetView = mControl1.createWidgetView(this, mBirdgeMap, 400, 300);
        mContainer.addView(widgetView);
        startRender();

        Button button = findViewById(R.id.render1);
        button.setVisibility(View.GONE);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startRender();
            }
        });
    }

    private void startRender() {
        JSONObject object = new JSONObject();
        String data = Utils.loadAssetFile(this, "mock_cube_data.json");
        object.put("chartConfig", data);
        JSONObject item = new JSONObject();
        item.put("date", 1653634800000L);
        item.put("price", 3130.2394);

        object.put("getPosition", item);
        mControl1.invokeMethod("render", object);
    }

    private void init2() {
        mControl2 = new BridgeControl();
        F2CanvasView widgetView = mControl2.createWidgetView(this, mBirdgeMap, 400, 300);
        mContainer.addView(widgetView);
        startRender2();

        Button button = findViewById(R.id.render2);
        button.setVisibility(View.GONE);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startRender2();
            }

        });
    }

    private void startRender2() {
        JSONObject object = new JSONObject();
        String data = Utils.loadAssetFile(this, "mock_cube_data2.json");
        object.put("chartConfig", data);
        mControl2.invokeMethod("render", object);
    }

    private void startRender3() {
//        JSONObject object = new JSONObject();
//        String data = Utils.loadAssetFile(this, "mock_cube_data.json");
//        object.put("chartConfig", data);
//        mControlV2.invokeMethod("render", object);
    }


    private void startDestroy() {
        mControl1.invokeMethod("destroy", null);
        mControl2.invokeMethod("destroy", null);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        startDestroy();
        mControl1.onDestroy();
        mControl2.onDestroy();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
        }
        return super.onOptionsItemSelected(item);
    }


}
