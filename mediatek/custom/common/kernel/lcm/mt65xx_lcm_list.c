#include <lcm_drv.h>
#ifdef BUILD_LK
#include <platform/disp_drv_platform.h>
#else
#include <linux/delay.h>
#include <mach/mt_gpio.h>
#endif
#include <cust_gpio_usage.h>
extern LCM_DRIVER cmi_otm1282a_hd720_dsi_cmd_lcm_drv;/*name changed*/
extern LCM_DRIVER auo_otm1282a_hd720_dsi_cmd_lcm_drv;

LCM_DRIVER* lcm_driver_list[] = 
{
#if defined(OTM1282A_HD720_DSI_CMD)
    &cmi_otm1282a_hd720_dsi_cmd_lcm_drv,/*change lcm driver name*/
    &auo_otm1282a_hd720_dsi_cmd_lcm_drv,
#endif
};

#define LCM_COMPILE_ASSERT(condition) LCM_COMPILE_ASSERT_X(condition, __LINE__)
#define LCM_COMPILE_ASSERT_X(condition, line) LCM_COMPILE_ASSERT_XX(condition, line)
#define LCM_COMPILE_ASSERT_XX(condition, line) char assertion_failed_at_line_##line[(condition)?1:-1]

unsigned int lcm_count = sizeof(lcm_driver_list)/sizeof(LCM_DRIVER*);
//LCM_COMPILE_ASSERT(0 != sizeof(lcm_driver_list)/sizeof(LCM_DRIVER*));

static unsigned char lcd_id_pins_value = 0xFF;

/******************************************************************************
Function:       which_lcd_module
Description:    get lcd module value
Input:          none
Output:         none
Return:         lcd module value
Others:          none
******************************************************************************/
unsigned char which_lcd_module()
{
    unsigned char lcd_id0, lcd_id1;
    unsigned char lcd_id;

    //only recognise once
    if(0xFF != lcd_id_pins_value)
    {
        return lcd_id_pins_value;
    }
    /*because of gpio no initialization,do it by hand*/
    mt_set_gpio_mode(GPIO_DISP_ID0_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_DISP_ID0_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_DISP_ID0_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_mode(GPIO_DISP_ID1_PIN, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_DISP_ID1_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_DISP_ID1_PIN, GPIO_PULL_ENABLE);

    lcd_id0 = mt_get_gpio_in(GPIO_DISP_ID0_PIN);
    lcd_id1 = mt_get_gpio_in(GPIO_DISP_ID1_PIN);

#ifdef BUILD_LK
    printf("which_lcd_module,lcd_id0:%d\n",lcd_id0);
    printf("which_lcd_module,lcd_id1:%d\n",lcd_id1);
#else
    printk("which_lcd_module,lcd_id0:%d\n",lcd_id0);
    printk("which_lcd_module,lcd_id1:%d\n",lcd_id1);
#endif
    lcd_id =  lcd_id0 | (lcd_id1 << 1);

#ifdef BUILD_LK
    printf("which_lcd_module,lcd_id:%d\n",lcd_id);
#else
    printk("which_lcd_module,lcd_id:%d\n",lcd_id);
#endif
    lcd_id_pins_value = lcd_id;
    return lcd_id;
}
