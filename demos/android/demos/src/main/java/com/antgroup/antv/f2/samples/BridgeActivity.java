package com.antgroup.antv.f2.samples;

import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.os.Bundle;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Log;

/**
 * author by luna
 * date on 2022/5/29
 */

public class BridgeActivity extends AppCompatActivity {

    private static final String TAG = "BridgeActivity";
    private F2CanvasCubeControlV2 mF2CanvasCubeControlV2;
    private LinearLayout mContainer;
    private final HashMap<String, Object> mBridgeMap = new HashMap<>();
    private TextView mTextContent1;
    private TextView mTextContent2;
    private View mLine;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        setContentView(R.layout.activity_bridge);
        init();
    }

    private void init() {
        mContainer = findViewById(R.id.container);
        mF2CanvasCubeControlV2 = new F2CanvasCubeControlV2(this);
        HashMap<String, Object> attrsMap = new HashMap<>();
        attrsMap.put("needToolTip", "true");
        attrsMap.put("longPress", "true");
        attrsMap.put("click", "true");
        attrsMap.put("pan", "true");
        attrsMap.put("pinch", "true");

        mBridgeMap.put("attrs", attrsMap);
        initView();
    }

    private void initView() {
        WindowManager wm = (WindowManager) this.getSystemService(Context.WINDOW_SERVICE);
        int width = wm.getDefaultDisplay().getWidth();
        int height = wm.getDefaultDisplay().getHeight();

        F2CanvasView widgetView = mF2CanvasCubeControlV2.createWidgetView(this, mBridgeMap, null, width * 9 / 10, height / 4);
        mContainer.addView(widgetView);

        findViewById(R.id.destroy).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startDestroy();
            }
        });
        findViewById(R.id.render1).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startRender();
            }
        });
        mTextContent1 = findViewById(R.id.text_content1);
        mTextContent2 = findViewById(R.id.text_content2);

        mLine = findViewById(R.id.line_view);
        mLine.setVisibility(View.GONE);
        startRender();
    }

    private String getChartData() {
        String assetPath = "mock_cube_in_out.json";
        return Utils.loadAssetFile(BridgeActivity.this, assetPath);
    }

    private void startRender() {
        F2Log.get().e(TAG, "startRender ");
        JSONObject object = new JSONObject();
        object.put("chartConfig", getChartData());

        mF2CanvasCubeControlV2.invokeMethod("render", object, new F2CanvasCubeControlV2.CSCallback() {

            @Override
            public void callbackKeepAlive(Map<String, Object> var1, boolean var2) {
                if (var1 != null) {
                    if (var1.containsKey("renderResult") && var1.get("renderResult") != null) {
                        JSONObject renderResult = (JSONObject) var1.get("renderResult");
                        if (renderResult.containsKey("state")) {
                            Boolean state = renderResult.getBoolean("state");
                            if (state != null && state) {
//                            getChartPositions();
                            }
                        }
                    } else if (var1.containsKey("tooltips")) {
                        String tooltips = (String) var1.get("tooltips");
                        F2Log.get().e(TAG, "tooltips " + tooltips);
                    } else {
                        F2Log.get().e(TAG, "render " + JSON.toJSONString(var1));
                    }
                }
            }
        });
    }

//    private void getChartPosition() {
//        JSONObject info = new JSONObject();
//
//        JSONObject item = new JSONObject();
////        if (mCount % 2 == 1) {
////            item.put("date", "20230606");
////            item.put("value", -8.03);
////        } else {
//        item.put("date", "2018年");
//        item.put("value", 0.229);
////        }
//        info.put("itemData", item);
//        mF2CanvasCubeControlV2.invokeMethod("getPosition", info, new F2CanvasCubeControlV2.CSCallback() {
//
//            @Override
//            public void callbackKeepAlive(Map<String, Object> var1, boolean var2) {
//                if (var1 != null) {
//                    JSONArray position = (JSONArray) var1.get("position");
//                    if (position != null && position.size() == 2) {
//                        BigDecimal xposition = (BigDecimal) position.get(0);
//                        BigDecimal yposition = (BigDecimal) position.get(1);
//                        mTextContent.setText("坐标 x:" + xposition + " ,yposition:" + yposition);
//                        FrameLayout.LayoutParams layoutParams = (FrameLayout.LayoutParams) mTextContentChart.getLayoutParams();
//                        layoutParams.leftMargin = xposition.intValue();
//                        layoutParams.topMargin = yposition.intValue();
//                        mTextContentChart.setLayoutParams(layoutParams);
//                    }
//                }
//            }
//        });
//    }

//    private JSONObject getItem() {
//        JSONObject item = new JSONObject();
//        item.put("date", "2");
//        item.put("value", 0.0052);
//        return item;
//    }
//
//    private JSONObject getItem1() {
//        JSONObject item = new JSONObject();
//        item.put("date", "5");
//        item.put("value", 0.0158);
//        return item;
//    }

//    private void getChartPositions() {
//        JSONObject info = new JSONObject();
//
//        JSONArray items = new JSONArray();
//        items.add(getItem());
//        items.add(getItem1());
//
//
//        info.put("itemDatas", items);
//        mF2CanvasCubeControlV2.invokeMethod("getPositions", info, new F2CanvasCubeControlV2.CSCallback() {
//
//            @Override
//            public void callbackKeepAlive(Map<String, Object> var1, boolean var2) {
//                if (var1 != null) {
//                    JSONArray positions = (JSONArray) var1.get("positions");
//                    if (positions != null && positions.size() > 0) {
//                        for (int i = 0; i < positions.size(); i++) {
//                            JSONArray position = positions.getJSONArray(i);
//                            if (position != null && position.size() == 2) {
//                                BigDecimal xposition = (BigDecimal) position.get(0);
//                                BigDecimal yposition = (BigDecimal) position.get(1);
//                                if (i == 0) {
//                                    mTextContent1.setText("坐标1 x:" + xposition + " ,yposition:" + yposition);
//                                } else {
//                                    mTextContent2.setText("坐标2 x:" + xposition + " ,yposition:" + yposition);
//                                    FrameLayout.LayoutParams layoutParams = (FrameLayout.LayoutParams) mLine.getLayoutParams();
//                                    layoutParams.leftMargin = xposition.intValue();
//                                    layoutParams.topMargin = yposition.intValue();
//                                    mLine.setLayoutParams(layoutParams);
//                                }
//                            }
//                        }
//                    } else {
//                        F2Log.get().e(TAG, "getPositions ");
//                    }
//                }
//            }
//        });
//    }

    private void startDestroy() {
        if (mF2CanvasCubeControlV2 != null) {
            mF2CanvasCubeControlV2.invokeMethod("destroy", null, null);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        startDestroy();
        if (mF2CanvasCubeControlV2 != null) {
            mF2CanvasCubeControlV2.onDestroy();
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
        }
        return super.onOptionsItemSelected(item);
    }
}
