package com.antgroup.antv.f2.samples;

import android.os.Bundle;
import android.view.MenuItem;

import com.antgroup.antv.f2.F2CanvasView;

import androidx.appcompat.app.AppCompatActivity;

public class SampleActivityHorizontal extends AppCompatActivity {

    private F2CanvasView mCanvasView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        setContentView(R.layout.activity_sample_horizontal);

        mCanvasView = findViewById(R.id.canvasView);
        mCanvasView.initCanvasContext(new F2CanvasView.ConfigBuilder()
                .canvasId("F2AntCanvas")
//                .backgroundColor("#000000")
                .asyncRender(false)
                .setOption("canvasBizId", "F2NativeDemo")
                .setOption("appId", "1000")
                .build());

        ChartModel chartModel = (ChartModel) getIntent().getSerializableExtra("ChartModel");
        setTitle(chartModel.title);
        try {
            F2CanvasView.Adapter adapter = chartModel.adapterClass.newInstance();
            mCanvasView.setAdapter(adapter);
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mCanvasView.destroy();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
        }
        return super.onOptionsItemSelected(item);
    }
}
