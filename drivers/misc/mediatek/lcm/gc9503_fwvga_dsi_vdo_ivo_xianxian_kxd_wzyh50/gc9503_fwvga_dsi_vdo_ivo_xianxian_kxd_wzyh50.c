#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
	#include <string.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
#endif

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										480
#define FRAME_HEIGHT 										854

#define REGFLAG_DELAY             							0xFFF
#define REGFLAG_END_OF_TABLE      							0xEEE   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE	0
#define GC9503_LCM_ID       0x9504

//#define GC9504_TEST
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    

 struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};
 
static struct LCM_setting_table lcm_initialization_setting[] = {

{0xF0, 5,{0x55, 0xAA, 0x52, 0x08, 0x00}},

{0xF6, 2,{0x5A, 0x87}},

{0xC1, 1,{0x3F}},

{0xC2, 1,{0x0E}},

{0xC6, 1,{0xF8}},

{0xC9, 1,{0x10}},

{0xCD, 1,{0x25}},

{0xF8, 1,{0x8A}},

{0xAC, 1,{0x65}},

{0xA7, 1,{0x47}},

{0xA0, 1,{0xDD}},

{0xFA, 3,{0x8F, 0x00, 0x00}},

{0x71, 1,{0x48}},

{0x72, 1,{0x48}},

{0x73, 2,{0x00, 0x44}},

{0xA3, 1,{0x22}},

{0xFD, 3,{0x3C, 0x3C, 0x00}},

{0x97, 1,{0xEE}},

{0x83, 1,{0x93}},

{0x9A, 1,{0xBD}},

{0x9B, 1,{0x53}},

{0x82, 2,{0x76, 0x76}},

{0xB1, 1,{0x10}},

{0x7A, 2,{0x13, 0x1A}},

{0x7B, 2,{0x13, 0x1A}},
{0x6D, 32,{0x1E, 0x1E, 0x02, 0x0A, 0x0C, 0x0E, 0x10, 0x1E, 0x19, 0x1A, 0x1E, 0x08, 0x1E, 0x1E, 0x04, 0x1E, 0x1E, 0x03, 0x1E, 0x1E, 0x07, 0x1E, 0x1A, 0x19, 0x1E, 0x0F, 0x0D, 0x0B, 0x09, 0x01, 0x1E, 0x1E}},
//{0x6D, 32,{0x1E, 0x1E, 0x07, 0x09, 0x0F, 0x0D, 0x0B, 0x1E, 0x19, 0x1A, 0x1E, 0x01, 0x1E, 0x1E, 0x03, 0x1E, 0x1E, 0x04, 0x1E, 0x1E, 0x02, 0x1E, 0x1A, 0x19, 0x1E, 0x0C, 0x0E, 0x10, 0x0A, 0x08, 0x1E, 0x1E}},


{0x64, 16,{0x38, 0x0B, 0x03, 0x65, 0x03, 0x03, 0x38, 0x0A, 0x03, 0x66, 0x03, 0x03, 0x76, 0x76, 0x76, 0x76}},

{0x65, 16,{0x38, 0x09, 0x03, 0x5F, 0x03, 0x03, 0x38, 0x08, 0x03, 0x60, 0x03, 0x03, 0x76, 0x76, 0x76, 0x76}},

{0x66, 16,{0x38, 0x07, 0x03, 0x61, 0x03, 0x03, 0x38, 0x06, 0x03, 0x62, 0x03, 0x03, 0x76, 0x76, 0x76, 0x76}},

{0x67, 16,{0x38, 0x05, 0x03, 0x63, 0x03, 0x03, 0x38, 0x04, 0x03, 0x64, 0x03, 0x03, 0x76, 0x76, 0x76, 0x76}},

{0x60, 8,{0x38, 0x0D, 0x76, 0x76, 0x38, 0x0C, 0x76, 0x76}},

{0x61, 8,{0x33, 0x5A, 0x76, 0x76, 0x33, 0x5B, 0x76, 0x76}},

{0x63, 8,{0x23, 0x57, 0x76, 0x76, 0x23, 0x58, 0x76, 0x76}},



{0x68, 13,{0x3B, 0x08, 0x10, 0x08, 0x0F, 0x76, 0x76, 0x08, 0x10, 0x08, 0x0F, 0x76, 0x76}},

{0x69, 7,{0x14, 0x22, 0x14, 0x22, 0x44, 0x22, 0x08}},

{0x6B, 1,{0x07}},

{0xD1, 52,{0x00, 0x00, 0x00, 0x20, 0x00, 0x66, 0x00, 0x79, 0x00, 0xA2, 0x00, 0xBC, 0x00, 0xE8, 0x01, 0x5D, 0x01, 0xC0, 0x02, 0x10, 0x02, 0x38, 0x02, 0x88, 0x02, 0xC0, 0x02, 0xC2, 0x02, 0xF2, 0x03, 0x22, 0x03, 0x3D, 0x03, 0x5B, 0x03, 0x70, 0x03, 0x88, 0x03, 0x98, 0x03, 0xA6, 0x03, 0xAE, 0x03, 0xB8, 0x03, 0xD0, 0x03, 0xFF}},

{0xD2, 52,{0x00, 0x00, 0x00, 0x20, 0x00, 0x66, 0x00, 0x79, 0x00, 0xA2, 0x00, 0xBC, 0x00, 0xE8, 0x01, 0x5D, 0x01, 0xC0, 0x02, 0x10, 0x02, 0x38, 0x02, 0x88, 0x02, 0xC0, 0x02, 0xC2, 0x02, 0xF2, 0x03, 0x22, 0x03, 0x3D, 0x03, 0x5B, 0x03, 0x70, 0x03, 0x88, 0x03, 0x98, 0x03, 0xA6, 0x03, 0xAE, 0x03, 0xB8, 0x03, 0xD0, 0x03, 0xFF}},

{0xD3, 52,{0x00, 0x00, 0x00, 0x20, 0x00, 0x66, 0x00, 0x79, 0x00, 0xA2, 0x00, 0xBC, 0x00, 0xE8, 0x01, 0x5D, 0x01, 0xC0, 0x02, 0x10, 0x02, 0x38, 0x02, 0x88, 0x02, 0xC0, 0x02, 0xC2, 0x02, 0xF2, 0x03, 0x22, 0x03, 0x3D, 0x03, 0x5B, 0x03, 0x70, 0x03, 0x88, 0x03, 0x98, 0x03, 0xA6, 0x03, 0xAE, 0x03, 0xB8, 0x03, 0xD0, 0x03, 0xFF}},

{0xD4, 52,{0x00, 0x00, 0x00, 0x20, 0x00, 0x66, 0x00, 0x79, 0x00, 0xA2, 0x00, 0xBC, 0x00, 0xE8, 0x01, 0x5D, 0x01, 0xC0, 0x02, 0x10, 0x02, 0x38, 0x02, 0x88, 0x02, 0xC0, 0x02, 0xC2, 0x02, 0xF2, 0x03, 0x22, 0x03, 0x3D, 0x03, 0x5B, 0x03, 0x70, 0x03, 0x88, 0x03, 0x98, 0x03, 0xA6, 0x03, 0xAE, 0x03, 0xB8, 0x03, 0xD0, 0x03, 0xFF}},

{0xD5, 52,{0x00, 0x00, 0x00, 0x20, 0x00, 0x66, 0x00, 0x79, 0x00, 0xA2, 0x00, 0xBC, 0x00, 0xE8, 0x01, 0x5D, 0x01, 0xC0, 0x02, 0x10, 0x02, 0x38, 0x02, 0x88, 0x02, 0xC0, 0x02, 0xC2, 0x02, 0xF2, 0x03, 0x22, 0x03, 0x3D, 0x03, 0x5B, 0x03, 0x70, 0x03, 0x88, 0x03, 0x98, 0x03, 0xA6, 0x03, 0xAE, 0x03, 0xB8, 0x03, 0xD0, 0x03, 0xFF}},

{0xD6, 52,{0x00, 0x00, 0x00, 0x20, 0x00, 0x66, 0x00, 0x79, 0x00, 0xA2, 0x00, 0xBC, 0x00, 0xE8, 0x01, 0x5D, 0x01, 0xC0, 0x02, 0x10, 0x02, 0x38, 0x02, 0x88, 0x02, 0xC0, 0x02, 0xC2, 0x02, 0xF2, 0x03, 0x22, 0x03, 0x3D, 0x03, 0x5B, 0x03, 0x70, 0x03, 0x88, 0x03, 0x98, 0x03, 0xA6, 0x03, 0xAE, 0x03, 0xB8, 0x03, 0xD0, 0x03, 0xFF}},

{0x11, 0,{0x00}},
{REGFLAG_DELAY, 220, {}},
{0x29, 0,{0x00}},
{REGFLAG_DELAY, 120, {}},

	// Setting ending by predefined flag
   {REGFLAG_END_OF_TABLE, 0x00, {}}

};

#if 0
static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 0, {0x00}},
    {REGFLAG_DELAY, 100, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
#endif

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
     
	 
	 
{0x6C, 1,{0x60}},
{REGFLAG_DELAY, 20, {}},
{0xB1, 1,{0x00}},

{0xFA, 4,{0x7F, 0x00, 0x00, 0x00}},
	{REGFLAG_DELAY, 20, {}},
{0x6C, 1,{0x50}},
	{REGFLAG_DELAY, 10, {}},
{0x28, 0,{0x00}},
	{REGFLAG_DELAY, 50, {}},
{0x10, 0,{0x00}},
	{REGFLAG_DELAY, 20, {}},
{0xF0, 5,{0x55, 0xAA, 0x52, 0x08, 0x00}},

{0xC2, 1,{0xCE}},

{0xC3, 1,{0xCD}},

{0xC6, 1,{0xFC}},

{0xCD, 1,{0x64}},

{0xC9, 1,{0xCD}},

{0xF6, 2,{0x5A, 0x87}},

{0xFD, 3,{0xAA, 0xAA, 0x0A}},

{0xFE, 2,{0x6A, 0x0A}},

{0x78, 2,{0x2A, 0xAA}},

{0x92, 2,{0x17, 0x08}},

{0x77, 2,{0xAA, 0x2A}},

{0x76, 2,{0xAA, 0xAA}},

{0xAF, 1,{0x02}},
	   
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
//				MDELAY(10);
       	}
    }
	
}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));
    
    params->type   = LCM_TYPE_DSI;
    
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;
    
    // enable tearing-free
    params->dbi.te_mode				= LCM_DBI_TE_MODE_VSYNC_ONLY;
    params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
    
#if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif
	 // 
  
    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_TWO_LANE;
    
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding 	= LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format	  = LCM_DSI_FORMAT_RGB888;
    
    // Video mode setting		
    params->dsi.intermediat_buffer_num = 2;
    
    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
   // params->dsi.cont_clock = 1;  //added by cheguosheng  // Continuous mode   or  not Continuous mode

	params->dsi.vertical_sync_active	= 8;// 3    2  4
	params->dsi.vertical_backporch		= 16;// 20   11  6
	params->dsi.vertical_frontporch		= 20; // 8 1  12 11
	params->dsi.vertical_active_line	= FRAME_HEIGHT; 

	params->dsi.horizontal_sync_active	= 8;// 10 50  20
	params->dsi.horizontal_backporch	= 56;//30  150
	params->dsi.horizontal_frontporch	= 80;//30 150
  
    params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
    params->dsi.PLL_CLOCK				= 217;
}


static unsigned int lcm_compare_id(void);

static void lcm_init(void)
{
    SET_RESET_PIN(1);
	MDELAY(10);//Must > 5ms
    SET_RESET_PIN(0);
    MDELAY(100);//Must > 5ms
    SET_RESET_PIN(1);
    MDELAY(120);//Must > 50ms

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);

}


static void lcm_suspend(void)
{

	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	SET_RESET_PIN(1);
     MDELAY(10);//Must > 5ms
    SET_RESET_PIN(0);
    MDELAY(10);//Must > 5ms
    SET_RESET_PIN(1);
    MDELAY(120);//Must > 50ms
}


static void lcm_resume(void)
{
    #ifdef BUILD_LK
		printf("%s, LK gc9503_fwvga_dsi_vdo_boe_tengjun_o debug \n", __func__);
    #else
		printk("%s, kernel gc9503_fwvga_dsi_vdo_boe_tengjun_o debug \n", __func__);
    #endif
        
	//lcm_compare_id();
lcm_init();
		//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}

static unsigned int lcm_compare_id(void)
{ 
   int array[4];
   char buffer[5];
   int id=0;

   SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
   MDELAY(5);
   SET_RESET_PIN(0);
   MDELAY(25);
   SET_RESET_PIN(1);
   MDELAY(250);

   array[0] = 0x00033700;
   dsi_set_cmdq(array, 1, 1);
   read_reg_v2(0x04, buffer, 3);

   id = buffer[0];
   id = (id << 8) | buffer[1];

	#if defined(BUILD_LK)
		printf("GC9503_LCM_ID uboot id = 0x%x  %x %x %x\n", id, buffer[0], buffer[1], buffer[2]);
	#else
		printk("GC9503_LCM_ID kernel id = 0x%x  %x %x %x\n", id, buffer[0], buffer[1], buffer[2]);
	#endif
	if((0x95 == buffer[1]) && ((0x03 == buffer[2]) || (0x04 == buffer[2]))){
			//LCD_PRINT("lcd_gc9503_mipi read id success!\n");
			return 1;
		}
	return 0;
	
}


LCM_DRIVER gc9503_fwvga_dsi_vdo_ivo_xianxian_kxd_wzyh50_lcm_drv =
{
                       
    .name			= "gc9503_fwvga_dsi_vdo_ivo_xianxian_kxd_wzyh50",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
};

