package com.antgroup.antv.f2.samples;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Log;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

public class ChartListActivity extends AppCompatActivity {

    private RecyclerView listView;

    private List<WeakReference<F2CanvasView>> canvasViews = new ArrayList<>();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        setContentView(R.layout.activity_main);

        listView = findViewById(R.id.listView);

        listView.setLayoutManager(new LinearLayoutManager(this));
        listView.setAdapter(new CanvasViewAdapter(MainActivity.getChartModels(), canvasViews));
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

        public CanvasViewHolder(@NonNull View itemView, List<WeakReference<F2CanvasView>> canvasViews) {
            super(itemView);
            chartName = itemView.findViewById(R.id.chart_name);
            canvasView = itemView.findViewById(R.id.canvasView);
            canvasView.initCanvasContext(new F2CanvasView.ConfigBuilder()
                    .setOption("canvasBizId", "ChartListActivity")
                    .setOption("appId", "1000")
                    .build());
            canvasViews.add(new WeakReference<>(canvasView));
        }
    }


    static class CanvasViewAdapter extends RecyclerView.Adapter<CanvasViewHolder> {
        private List<ChartModel> mChartModels;
        private List<WeakReference<F2CanvasView>> mCanvasViews;

        CanvasViewAdapter(List<ChartModel> chartModels, List<WeakReference<F2CanvasView>> canvasViews) {
            mChartModels = chartModels;
            mCanvasViews = canvasViews;
        }

        @NonNull
        @Override
        public CanvasViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
            View itemView = LayoutInflater.from(parent.getContext()).inflate(R.layout.layout_chart_canvasview, parent, false);
            return new CanvasViewHolder(itemView, mCanvasViews);
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