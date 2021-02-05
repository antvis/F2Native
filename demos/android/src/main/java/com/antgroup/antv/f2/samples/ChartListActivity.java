package com.antgroup.antv.f2.samples;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.antgroup.antv.f2.F2CanvasView;

import java.util.List;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

public class ChartListActivity extends AppCompatActivity {

    private RecyclerView listView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        setContentView(R.layout.activity_main);

        listView = findViewById(R.id.listView);

        listView.setLayoutManager(new LinearLayoutManager(this));
        listView.setAdapter(new CanvasViewAdapter(MainActivity.getChartModels()));
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
        public CanvasViewHolder(@NonNull View itemView) {
            super(itemView);
            chartName = itemView.findViewById(R.id.chart_name);
            canvasView = itemView.findViewById(R.id.canvasView);
            canvasView.initCanvasContext(new F2CanvasView.ConfigBuilder()
                    .canvasId("F2AntCanvas")
                    .asyncRender(true)
                    .setOption("canvasBizId", "ChartListActivity")
                    .setOption("appId", "1000")
                    .build());
        }
    }


    static class CanvasViewAdapter extends RecyclerView.Adapter<CanvasViewHolder> {
        private List<ChartModel> mChartModels;
        CanvasViewAdapter(List<ChartModel> chartModels) {
            mChartModels = chartModels;
        }

        @NonNull
        @Override
        public CanvasViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
            View itemView = LayoutInflater.from(parent.getContext()).inflate(R.layout.layout_chart_canvasview, parent, false);
            return new CanvasViewHolder(itemView);
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