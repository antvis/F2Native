package com.antgroup.antv.f2.samples;

import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.antgroup.antv.f2.samples.charts.CandleChart_klineDay;
import com.antgroup.antv.f2.samples.charts.CirclePieChart;
import com.antgroup.antv.f2.samples.charts.MKTrendChart;
import com.antgroup.antv.f2.samples.charts.MultiAreasChart;
import com.antgroup.antv.f2.samples.charts.MultiIntervalChart;
import com.antgroup.antv.f2.samples.charts.MultiLinesChart_1;
import com.antgroup.antv.f2.samples.charts.PieChart;
import com.antgroup.antv.f2.samples.charts.SectionIntervalChart;
import com.antgroup.antv.f2.samples.charts.SingleAreaChart_1;
import com.antgroup.antv.f2.samples.charts.SingleAreaChart_2;
import com.antgroup.antv.f2.samples.charts.SingleIntervalChart_1;
import com.antgroup.antv.f2.samples.charts.SingleIntervalChart_2;
import com.antgroup.antv.f2.samples.charts.SingleIntervalChart_3;
import com.antgroup.antv.f2.samples.charts.MultiIntervalChart_under_zero;
import com.antgroup.antv.f2.samples.charts.SingleLineChart_1;
import com.antgroup.antv.f2.samples.charts.SingleLineChart_2;
import com.antgroup.antv.f2.samples.charts.SinglePointChart_1;

import java.util.Arrays;
import java.util.List;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

public class MainActivity extends AppCompatActivity {

    private RecyclerView listView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        closeGuard();
        setContentView(R.layout.activity_main);
        listView = findViewById(R.id.listView);

        listView.setLayoutManager(new LinearLayoutManager(this));
        listView.setAdapter(new ChartAdapter(getChartModels()));

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_multiCharts: {
                Intent intent = new Intent(this, ChartListActivity.class);
                startActivity(intent);
                break;
            }
        }
        return super.onOptionsItemSelected(item);
    }

    static class ChartViewHolder extends RecyclerView.ViewHolder {

        TextView chartName;
        public ChartViewHolder(@NonNull View itemView) {
            super(itemView);
            chartName = itemView.findViewById(R.id.chart_name);
        }
    }

    static class ChartAdapter extends RecyclerView.Adapter<ChartViewHolder> {
        private List<ChartModel> mChartModels;
        ChartAdapter(List<ChartModel> chartModels) {
            mChartModels = chartModels;
        }

        @NonNull
        @Override
        public ChartViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
            View itemView = LayoutInflater.from(parent.getContext()).inflate(R.layout.layout_chart_item, parent, false);
            return new ChartViewHolder(itemView);
        }

        @Override
        public void onBindViewHolder(@NonNull ChartViewHolder holder, int position) {
            final ChartModel chartModel = mChartModels.get(position);
            holder.chartName.setText(chartModel.title);
            holder.itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent intent;
                    if (chartModel.horizontal) {
                        intent = new Intent(v.getContext(), SampleActivityHorizontal.class);
                    } else {
                        intent = new Intent(v.getContext(), SampleActivity.class);
                    }
                    intent.putExtra("ChartModel", chartModel);
                    v.getContext().startActivity(intent);
                }
            });
        }

        @Override
        public int getItemCount() {
            return mChartModels.size();
        }
    }

    public static List<ChartModel> getChartModels() {
        return Arrays.asList(
                new ChartModel("基础折线图", SingleLineChart_1.class),
                new ChartModel("基础折线图(双Y轴)", SingleLineChart_2.class),
                new ChartModel("对比折线图", MultiLinesChart_1.class),

                new ChartModel("基础面积图", SingleAreaChart_1.class),
                new ChartModel("基础面积图_2(带负值)", SingleAreaChart_2.class),
                new ChartModel("层叠面积图", MultiAreasChart.class),

                new ChartModel("基础柱状图", SingleIntervalChart_1.class),
                new ChartModel("基础柱状图2", SingleIntervalChart_2.class),
                new ChartModel("分组柱状图", MultiIntervalChart.class),
                new ChartModel("分组柱状图(带负值)", MultiIntervalChart_under_zero.class),
                new ChartModel("区间柱状图", SectionIntervalChart.class),
                new ChartModel("散点图", SinglePointChart_1.class),

                new ChartModel("基础条状图", SingleIntervalChart_3.class),

                new ChartModel("饼图", PieChart.class),
                new ChartModel("环形图", CirclePieChart.class),
                new ChartModel("上证指数异动分时图", MKTrendChart.class),
                new ChartModel("蜡烛图(日K)", CandleChart_klineDay.class, true)
        );
    }

    private static void closeGuard() {
        try {
            Class.forName("dalvik.system.CloseGuard")
                    .getMethod("setEnabled", boolean.class)
                    .invoke(null, true);
        } catch (Exception e) {
        }
    }
}
