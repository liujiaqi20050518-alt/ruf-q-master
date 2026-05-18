#include "output.h"
#include "data.h"
#include "main.h"
#include "tim.h"

void flowout_config(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    timer_break_parameter_struct timer_breakpara;
    timer_deinit(TIMER0);

    timer_initpara.prescaler = 2 - 1;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 12000 - 1;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);

    switch (g_parameters.output_mode)
    {
    case ANALOG:
        // 模拟量输出
        // 4-20mA: PA0 low, PA7/PA8 return to TIMER0 CH0N/CH0.
        gpio_pin_remap_config(GPIO_TIMER0_PARTIAL_REMAP, ENABLE);
        gpio_bit_reset(FLOW_OUT_SEL_GPIO_Port, FLOW_OUT_SEL_Pin);
        gpio_init(DIG_P_GPIO_Port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, DIG_P_Pin);
        gpio_init(DIG_N_GPIO_Port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, DIG_N_Pin);

        timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
        timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
        timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
        timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_LOW;
        timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
        timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_HIGH;

        timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);
        timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 0);
        timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
        break;

    case PLUSE_PNP:
        // PNP型脉冲输出
        // PNP: PA7 low, PA8/TIMER0_CH0 outputs the pulse.
        gpio_pin_remap_config(GPIO_TIMER0_PARTIAL_REMAP, ENABLE);
        gpio_bit_set(FLOW_OUT_SEL_GPIO_Port, FLOW_OUT_SEL_Pin);
        gpio_bit_reset(DIG_P_GPIO_Port, DIG_P_Pin);
        gpio_bit_reset(DIG_N_GPIO_Port, DIG_N_Pin);
        gpio_init(DIG_P_GPIO_Port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, DIG_P_Pin);
        gpio_init(DIG_N_GPIO_Port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, DIG_N_Pin);

        timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
        timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
        timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
        timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
        timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
        timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

        timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);
        timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 6000);
        timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
        timer_primary_output_config(TIMER0, ENABLE);
        break;

    case PLUSE_NPN:
        // NPN型脉冲输出
        // NPN: PA8 low, PA7/TIMER0_CH0N outputs the pulse.
        // CH0 must be enabled for OCxREF generation; CH0N polarity LOW
        // compensates for complementary inversion (CH0N = ~OCxREF xor POL).
        gpio_pin_remap_config(GPIO_TIMER0_PARTIAL_REMAP, ENABLE);
        gpio_bit_set(FLOW_OUT_SEL_GPIO_Port, FLOW_OUT_SEL_Pin);
        gpio_bit_reset(DIG_P_GPIO_Port, DIG_P_Pin);
        gpio_bit_reset(DIG_N_GPIO_Port, DIG_N_Pin);
        gpio_init(DIG_P_GPIO_Port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, DIG_P_Pin);
        gpio_init(DIG_N_GPIO_Port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, DIG_N_Pin);

        timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
        timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
        timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
        timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_LOW;
        timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
        timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

        timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);
        timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 6000);
        timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
        timer_primary_output_config(TIMER0, ENABLE);
        break;

    case ALARM_PNP:
        // PNP型报警输出
        gpio_bit_set(FLOW_OUT_SEL_GPIO_Port, FLOW_OUT_SEL_Pin);

        timer_disable(TIMER0);
        timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_LOW);

        gpio_init(ALARM_PNP_OUTPUT_GPIO_Port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, ALARM_PNP_OUTPUT_Pin);
        break;
    case ALARM_NPN:
        // NPN型报警输出
        gpio_bit_set(FLOW_OUT_SEL_GPIO_Port, FLOW_OUT_SEL_Pin);

        timer_disable(TIMER0);
        timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_LOW);

        gpio_init(ALARM_NPN_OUTPUT_GPIO_Port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, ALARM_NPN_OUTPUT_Pin);
        // 报警输出
        break;
    }
    
    timer_breakpara.runoffstate = TIMER_ROS_STATE_DISABLE;
    timer_breakpara.ideloffstate = TIMER_IOS_STATE_DISABLE;
    timer_breakpara.deadtime = 255;
    timer_breakpara.breakpolarity = TIMER_BREAK_POLARITY_LOW;
    timer_breakpara.outputautostate = TIMER_OUTAUTO_ENABLE;
    timer_breakpara.protectmode = TIMER_CCHP_PROT_0;
    timer_breakpara.breakstate = TIMER_BREAK_DISABLE;
    timer_break_config(TIMER0, &timer_breakpara);

    timer_primary_output_config(TIMER0, ENABLE);
    timer_auto_reload_shadow_enable(TIMER0);
    timer_enable(TIMER0);
}
