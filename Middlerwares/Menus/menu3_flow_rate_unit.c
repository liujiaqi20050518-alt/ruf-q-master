// main_menu.c
#include "menu.h"
#include "data.h"
static const float flow_unit_coeffs[] = {
    1.0f,                // 0: m³/h (基准单位)
    1000.0f / 60.0f,     // 1: L/min (1m³/h = 1000L/60min ≈16.6667 L/min)
    1000000.0f / 60.0f,  // 2: mL/min (1m³/h = 1000000mL/60min ≈16666.6667 mL/min)
    1000.0f              // 3: L/h (1m³/h = 1000 L/h)
};
int menu4_flow_rate_unit_handler(void* parameter)
{
    int pip_index = *(int*)parameter;
    if (g_parameters.flow_rate_unit != pip_index)
    {
        // 计算换算比例：新数值 = 旧数值 * (旧单位系数 / 新单位系数)
        float convert_ratio = flow_unit_coeffs[pip_index] / flow_unit_coeffs[g_parameters.flow_rate_unit];
        
        // --------------------------
        // 1. 同步换算模拟上下限
        // --------------------------
        g_parameters.upper_range *= convert_ratio;   // 模拟上限
        g_parameters.lower_range *= convert_ratio;   // 模拟下限
        
        // --------------------------
        // 2. 同步换算报警上下限
        // --------------------------
        g_parameters.alarm_upper_range *= convert_ratio; // 报警上限
        g_parameters.alarm_lower_range *= convert_ratio; // 报警下限
        
        // --------------------------
        // 3. 同步换算零切下限
        // --------------------------
        g_parameters.zero_cut *= convert_ratio;    // 零切下限

        // 更新单位并标记为已修改，保存参数
        g_parameters.flow_rate_unit = pip_index;
        g_parameters.is_saved = 1;
        SaveParameters(&g_parameters);
    }
    return 0;
}
int menu3_flow_rate_unit_handler(void * parameter) {
     // 材质设置
    static const MenuItem flow_rate_unit_items[] = {
        {"m3/h", menu4_flow_rate_unit_handler, " 立方米每小时 "},
        {"L/min", menu4_flow_rate_unit_handler, " 升每分钟 "},
        {"mL/min", menu4_flow_rate_unit_handler," 毫升每分钟 "},
        {"L/h", menu4_flow_rate_unit_handler, " 升每小时 "}
    };
    
    // 主菜单结构
    Menu FlowrateSetting_menu = {
        .title[0] = "Flow rate unit",
        .title[1] = " 流量单位 ",
        .items_count = sizeof(flow_rate_unit_items)/sizeof(flow_rate_unit_items[0]),
        .items = flow_rate_unit_items,
        .items_per_page = DEFAULT_ITEMS_PER_PAGE,
        .start =g_parameters.flow_rate_unit,
        .type  = 1
    };
    
    return display_menu(&FlowrateSetting_menu);
}