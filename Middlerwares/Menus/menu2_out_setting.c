// main_menu.c
#include "menu.h"
#include "data.h"
#include "parameter_editor.h"
#include "elog.h"
int pulse_handler(void* parameter)
{
    int pip_index = *(int*)parameter;
    g_parameters.k_factor = pip_index;
    g_parameters.is_saved = 1;
    SaveParameters(&g_parameters);
    return 0;
}
int menu4_pulse_handler(void * parameter) {
    //这边需要一个判断函数，用来判断序列号对应哪个
    //这里先用第一个来做逻辑
    const char* pulse_muenu[8] = {"0.1ml/P", "1ml/P", "10ml/P", "100ml/P", 
                                    "1L/P", "10L/P", "100L/P", "1000L/P"};
    const char* text[5] = {0};
    int index =0;
    for(int i = 0;i<8;i++)
    {
        if(g_product_configs[0].pulse_modes[i] != OUTPUT_NONE)
        {
            text[index] = pulse_muenu[i];
            index++;
        }
    }
     // 材质设置
    MenuItem fliuid_items[] = {
        {text[0], pulse_handler, text[0]},
        {text[1], pulse_handler, text[1]},
        {text[2], pulse_handler, text[2]},
        {text[3], pulse_handler, text[3]},
        {text[4], pulse_handler, text[4]}
    };
    
    // 主菜单结构
    Menu Materialsetting_menu = {
        .title[0] = "PLS K-factor",
        .title[1] = " 脉冲当量设置 ",
        .items_count = sizeof(fliuid_items)/sizeof(fliuid_items[0]),
        .items = fliuid_items,
        .items_per_page = DEFAULT_ITEMS_PER_PAGE,
        .start =g_parameters.pipe_type,
        .type  = 1
    };
    
    return display_menu(&Materialsetting_menu);
}
int menu2_outsetting_handler(void * parameter) {
    // 主菜单项
    static const MenuItem main_items[] = 
    {
        {"Modbus ADDR", menu4_modbus_addr_handler , " Modbus地址 "},
        {"Output mode"   , menu3_output_mode_handler, " 输出模式 "    },
        {"PLS K-factor", menu4_pulse_handler, " 脉冲当量 " },
        // {"Rs485 setup"   , menu3_rs485_setting_handler," 485设置 "  },
        {"Analog Output"  , menu3_4_20_mh_handler, " 模拟输出 "  },
        {"ALM Output"  , menu3_alarm_handler, " 报警输出 "  },
    };
    
    // 主菜单结构
    Menu outsetting_menu = {
        .title[0] = "Output Setting",
        .title[1] = " 输出设置 ",
        .items_count = sizeof(main_items)/sizeof(main_items[0]),
        .items = main_items,
        .items_per_page = DEFAULT_ITEMS_PER_PAGE,
        .start = 0
    };
    
    return display_menu(&outsetting_menu);
}
