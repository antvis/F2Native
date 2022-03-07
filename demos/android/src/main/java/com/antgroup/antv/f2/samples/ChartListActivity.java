package com.antgroup.antv.f2.samples;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2Log;
import com.antgroup.antv.f2.F2RenderThread;
import com.antgroup.antv.f2.F2RenderThreadFactory;
//import com.antgroup.antv.f2.F2StandaloneRenderThread;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

public class ChartListActivity extends AppCompatActivity {

    private RecyclerView listView;

//    private F2RenderThreadFactory mThreadFactory = new F2RenderThreadFactory() {
//        private F2StandaloneRenderThread.Worker mSharedThread = null;
//        @Override
//        public F2RenderThread createRenderThread(F2Config canvasOptions) {
//            // 所有图表共享同一个线程
//            if (mSharedThread == null) {
//                String sessionId = canvasOptions.getStringField("session_id");
//                mSharedThread = new F2StandaloneRenderThread.Worker(sessionId == null ? "t-ChartListActivityThread" : sessionId);
//            }
//            return mSharedThread.assign();
//        }
//    };

    private List<WeakReference<F2CanvasView>> canvasViews = new ArrayList<>();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        setContentView(R.layout.activity_main);

        listView = findViewById(R.id.listView);

        listView.setLayoutManager(new LinearLayoutManager(this));
        listView.setAdapter(new CanvasViewAdapter(MainActivity.getChartModels(), null, canvasViews));
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        F2Log.i("ChartListActivity", "canvasViews Size: " + canvasViews.size());
        for (WeakReference<F2CanvasView> canvasView : canvasViews) {
            if (canvasView.get() != null) {
                canvasView.get().destroy();
            }
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
        }
        return super.onOptionsItemSelected(item);
    }

    static class CanvasViewHolder extends RecyclerView.ViewHolder {

        TextView chartName;
        F2CanvasView canvasView;
        public CanvasViewHolder(@NonNull View itemView, F2RenderThreadFactory threadFactory, List<WeakReference<F2CanvasView>> canvasViews) {
            super(itemView);
            chartName = itemView.findViewById(R.id.chart_name);
            canvasView = itemView.findViewById(R.id.canvasView);
            canvasView.initCanvasContext(new F2CanvasView.ConfigBuilder()
                    .canvasId("F2AntCanvas")
                    .asyncRender(true)
                    .setOption("canvasBizId", "ChartListActivity")
                    .setOption("appId", "1000")
                    .build(), threadFactory);
            canvasViews.add(new WeakReference<>(canvasView));
        }
    }


    static class CanvasViewAdapter extends RecyclerView.Adapter<CanvasViewHolder> {
        private List<ChartModel> mChartModels;
        F2RenderThreadFactory mThreadFactory;
        private List<WeakReference<F2CanvasView>> mCanvasViews;

        CanvasViewAdapter(List<ChartModel> chartModels, F2RenderThreadFactory threadFactory, List<WeakReference<F2CanvasView>> canvasViews) {
            mChartModels = chartModels;
            this.mThreadFactory = threadFactory;
            mCanvasViews = canvasViews;
        }

        @NonNull
        @Override
        public CanvasViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
            View itemView = LayoutInflater.from(parent.getContext()).inflate(R.layout.layout_chart_canvasview, parent, false);
            return new CanvasViewHolder(itemView, mThreadFactory, mCanvasViews);
        }

        @Override
        public void onBindViewHolder(@NonNull CanvasViewHolder holder, int position) {
            final ChartModel chartModel = mChartModels.get(position);
            holder.chartName.setText(chartModel.title);

            try {
                F2CanvasView.Adapter adapter = chartModel.adapterClass.newInstance();
                holder.canvasView.setAdapter(adapter);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        @Override
        public int getItemCount() {
            return mChartModels.size();
        }
    }
}