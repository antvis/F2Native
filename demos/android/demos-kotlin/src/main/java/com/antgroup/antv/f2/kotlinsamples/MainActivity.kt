package com.antgroup.antv.f2.kotlinsamples

import android.content.Intent
import android.os.Bundle
import android.view.*
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.recyclerview.widget.RecyclerView.ViewHolder
import com.antgroup.antv.f2.kotlinsamples.charts.*
import java.util.*

class MainActivity : AppCompatActivity() {
    private var listView: RecyclerView? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        closeGuard()
        setContentView(R.layout.activity_main)
        listView = findViewById(R.id.listView) as? RecyclerView
        listView?.setLayoutManager(LinearLayoutManager(this))
        listView?.setAdapter(ChartAdapter(chartModels))
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menuInflater.inflate(R.menu.menu, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        when (item.itemId) {
            R.id.action_multiCharts -> {
                val intent = Intent(this, ChartListActivity::class.java)
                startActivity(intent)
            }
            R.id.action_bridge -> {
                startActivity(Intent(this, BridgeActivity::class.java))
            }
        }
        return super.onOptionsItemSelected(item)
    }

    internal class ChartViewHolder(itemView: View) : ViewHolder(itemView) {
        var chartName: TextView

        init {
            chartName = itemView.findViewById(R.id.chart_name)
        }
    }

    internal class ChartAdapter(private val mChartModels: List<ChartModel>) :
        RecyclerView.Adapter<ChartViewHolder>() {
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ChartViewHolder {
            val itemView = LayoutInflater.from(parent.context)
                .inflate(R.layout.layout_chart_item, parent, false)
            return ChartViewHolder(itemView)
        }

        override fun onBindViewHolder(holder: ChartViewHolder, position: Int) {
            val chartModel = mChartModels[position]
            holder.chartName.text = chartModel.title
            holder.itemView.setOnClickListener { v ->
                val intent: Intent
                intent = if (chartModel.horizontal) {
                    Intent(v.context, SampleActivityHorizontal::class.java)
                } else {
                    Intent(v.context, SampleActivity::class.java)
                }
                intent.putExtra("ChartModel", chartModel)
                v.context.startActivity(intent)
            }
        }

        override fun getItemCount(): Int {
            return mChartModels.size
        }
    }

    companion object {
        @JvmStatic
        val chartModels: List<ChartModel>
            get() = Arrays.asList(
                ChartModel("1688", SingleLineChart_1688::class.java),
                ChartModel("基础折线图", SingleLineChart_1::class.java),
                ChartModel("基础折线图(双Y轴)", SingleLineChart_2::class.java),
                ChartModel("对比折线图", MultiLinesChart_1::class.java),
                ChartModel("基础面积图", SingleAreaChart_1::class.java),
                ChartModel("基础面积图_2(带负值)", SingleAreaChart_2::class.java),
                ChartModel("层叠面积图", MultiAreasChart::class.java),
                ChartModel("雷达面积图", RadarAreaChart::class.java),
                ChartModel("基础柱状图", SingleIntervalChart_1::class.java),
                ChartModel("基础柱状图2", SingleIntervalChart_2::class.java),
                ChartModel("分组柱状图", MultiIntervalChart::class.java),
                ChartModel("分组柱状图(带负值)", MultiIntervalChart_under_zero::class.java),
                ChartModel("区间柱状图", SectionIntervalChart::class.java),
                ChartModel("散点图", SinglePointChart_1::class.java),
                ChartModel("基础条状图", SingleIntervalChart_3::class.java),
                ChartModel("饼图", PieChart::class.java),
                ChartModel("环形图", CirclePieChart::class.java),
                ChartModel("折线图(分时度量&手势)", SingleLineChart_TimeSharing_ToolTip::class.java),
                ChartModel("上证指数异动分时图", MKTrendChart::class.java),
                ChartModel("股票日K趋势图", SingleLineChart_KLine::class.java),
                ChartModel("蜡烛图(日K)", CandleChart_klineDay::class.java, true)
            )

        private fun closeGuard() {
            try {
                Class.forName("dalvik.system.CloseGuard")
                    .getMethod("setEnabled", Boolean::class.javaPrimitiveType)
                    .invoke(null, true)
            } catch (e: Exception) {
            }
        }
    }
}