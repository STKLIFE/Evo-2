#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (854)

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))
#define REGFLAG_DELAY             							0XFFF
#define REGFLAG_END_OF_TABLE      							0xEEE   // END OF REGISTERS MARKER


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
//#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

       
struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[128];
};


static struct LCM_setting_table lcm_initialization_setting[] = {	

/*{0xF0, 5,{0x55, 0xAA, 0x52, 0x08, 0x00}},  ////BOE5.0 + FWM

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

{0x71, 1,{0x48}},

{0x72, 1,{0x48}},

{0x73, 2,{0x00, 0x44}},

{0x97, 1,{0xEE}},

{0x83, 1,{0x93}},

{0xA3, 1,{0x22}},

{0xFD, 3,{0x28, 0x3C, 0x00}},

{0xFA, 3,{0x8F, 0x00, 0x00}},

{0x9A, 1,{0xc2}},///ba

{0x9B, 1,{0x72}},//6a

{0x82, 2,{0x72, 0x72}},///6a

{0xB1, 1,{0x10}},

{0x7A, 2,{0x13, 0x1A}},

{0x7B, 2,{0x13, 0x1A}},

//
{0x64, 16,{0x18, 0x04, 0x03, 0x5B, 0x03, 0x03, 0x18, 0x03, 0x03, 0x5C, 0x03, 0x03, 0x1A, 0x7A, 0x1A, 0x7A}},

{0x67, 16,{0x18, 0x06, 0x03, 0x59, 0x03, 0x03, 0x18, 0x05, 0x03, 0x5A, 0x03, 0x03, 0x1A, 0x7A, 0x1A, 0x7A}},

{0x60, 8,{0x18, 0x07, 0x1A, 0x7A, 0x18, 0x02, 0x1A, 0x7A}},

{0x63, 8,{0x18, 0x02, 0x1A, 0x7A, 0x18, 0x06, 0x1A, 0x7A}},

{0x68, 13,{0x00, 0x08, 0x0A, 0x08, 0x09, 0x00, 0x00, 0x08, 0x0A, 0x08, 0x09, 0x00, 0x00}},

{0x69, 7,{0x14, 0x22, 0x14, 0x22, 0x44, 0x22, 0x08}},

//


{0xD1, 52,{0x00, 0x00, 0x00, 0x10, 0x00, 0x3B, 0x00, 0x81, 0x00, 0xC3, 0x00, 0xF8, 0x01, 0x30, 0x01, 0x8D, 0x01, 0xD8, 0x02, 0x2C, 0x02, 0x65, 0x02, 0xAA, 0x02, 0xDF, 0x02, 0xE0, 0x03, 0x0E, 0x03, 0x3D, 0x03, 0x56, 0x03, 0x75, 0x03, 0x88, 0x03, 0x9D, 0x03, 0xA6, 0x03, 0xAD, 0x03, 0xB4, 0x03, 0xBB, 0x03, 0xCF, 0x03, 0xFF}},

{0xD2, 52,{0x00, 0x00, 0x00, 0x10, 0x00, 0x3B, 0x00, 0x81, 0x00, 0xC3, 0x00, 0xF8, 0x01, 0x30, 0x01, 0x8D, 0x01, 0xD8, 0x02, 0x2C, 0x02, 0x65, 0x02, 0xAA, 0x02, 0xDF, 0x02, 0xE0, 0x03, 0x0E, 0x03, 0x3D, 0x03, 0x56, 0x03, 0x75, 0x03, 0x88, 0x03, 0x9D, 0x03, 0xA6, 0x03, 0xAD, 0x03, 0xB4, 0x03, 0xBB, 0x03, 0xCF, 0x03, 0xFF}},

{0xD3, 52,{0x00, 0x00, 0x00, 0x10, 0x00, 0x3B, 0x00, 0x81, 0x00, 0xC3, 0x00, 0xF8, 0x01, 0x30, 0x01, 0x8D, 0x01, 0xD8, 0x02, 0x2C, 0x02, 0x65, 0x02, 0xAA, 0x02, 0xDF, 0x02, 0xE0, 0x03, 0x0E, 0x03, 0x3D, 0x03, 0x56, 0x03, 0x75, 0x03, 0x88, 0x03, 0x9D, 0x03, 0xA6, 0x03, 0xAD, 0x03, 0xB4, 0x03, 0xBB, 0x03, 0xCF, 0x03, 0xFF}},

{0xD4, 52,{0x00, 0x00, 0x00, 0x10, 0x00, 0x3B, 0x00, 0x81, 0x00, 0xC3, 0x00, 0xF8, 0x01, 0x30, 0x01, 0x8D, 0x01, 0xD8, 0x02, 0x2C, 0x02, 0x65, 0x02, 0xAA, 0x02, 0xDF, 0x02, 0xE0, 0x03, 0x0E, 0x03, 0x3D, 0x03, 0x56, 0x03, 0x75, 0x03, 0x88, 0x03, 0x9D, 0x03, 0xA6, 0x03, 0xAD, 0x03, 0xB4, 0x03, 0xBB, 0x03, 0xCF, 0x03, 0xFF}},

{0xD5, 52,{0x00, 0x00, 0x00, 0x10, 0x00, 0x3B, 0x00, 0x81, 0x00, 0xC3, 0x00, 0xF8, 0x01, 0x30, 0x01, 0x8D, 0x01, 0xD8, 0x02, 0x2C, 0x02, 0x65, 0x02, 0xAA, 0x02, 0xDF, 0x02, 0xE0, 0x03, 0x0E, 0x03, 0x3D, 0x03, 0x56, 0x03, 0x75, 0x03, 0x88, 0x03, 0x9D, 0x03, 0xA6, 0x03, 0xAD, 0x03, 0xB4, 0x03, 0xBB, 0x03, 0xCF, 0x03, 0xFF}},

{0xD6, 52,{0x00, 0x00, 0x00, 0x10, 0x00, 0x3B, 0x00, 0x81, 0x00, 0xC3, 0x00, 0xF8, 0x01, 0x30, 0x01, 0x8D, 0x01, 0xD8, 0x02, 0x2C, 0x02, 0x65, 0x02, 0xAA, 0x02, 0xDF, 0x02, 0xE0, 0x03, 0x0E, 0x03, 0x3D, 0x03, 0x56, 0x03, 0x75, 0x03, 0x88, 0x03, 0x9D, 0x03, 0xA6, 0x03, 0xAD, 0x03, 0xB4, 0x03, 0xBB, 0x03, 0xCF, 0x03, 0xFF}},

{0x11, 0,{0x00}},
{REGFLAG_DELAY, 220, {}},
{0x29, 0,{0x00}},
{REGFLAG_DELAY, 120, {}},
 {REGFLAG_END_OF_TABLE, 0x00, {}}*/
   {0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
	{0xF6,2,{0x5A, 0x87}}, 
	{0xC1,1,{0x3F}},
	{0xC6,1,{0xF8}}, 
	{0xC9,1,{0x10}}, 
	{0xCD,1,{0x25}}, 
	{0xFA,3,{0x08, 0x08, 0x08}},
	{0xA7,1,{0x47}}, 
	{0xA0,1,{0xCC}},
	{0xAC,1,{0x65}},
	{0xF8,1,{0x8A}},
	{0xA3,1,{0x22}},
	{0xFD,3,{0x28, 0x3C, 0x00}}, 
	{0x9A,1,{0x8c}}, 
	{0x9B,1,{0x87}}, 
	{0x82,2,{0x7f, 0x7f}},
	{0xB1,1,{0x04}}, 
	{0x7A,2,{0x13, 0x1A}},
	{0x7B,2,{0x13, 0x1A}},

    {0x60,8,{0x18,0x07,0x10,0x7A,0x18,0x02,0x10,0x7A}}, 
	{0x63,8,{0x18,0x02,0x10,0x7A,0x18,0x06,0x10,0x7A}},
	{0x64,16,{0x18,0x04,0x03,0x5B,0x03,0x03,0x18,0x03,0x03,0x5C,0x03,0x03,0x10,0x7A,0x10,0x7A}},
	{0x67,16,{0x18,0x06,0x03,0x59,0x03,0x03,0x18,0x05,0x03,0x5A,0x03,0x03,0x10,0x7A,0x10,0x7A}},

	{0x69,7,{0x14,0x22,0x14,0x22,0x14,0x22,0x08}},
	
	{0xD1, 52,{0x00, 0x00, 0x00, 0x02, 0x00, 0x1B, 0x00, 0x28, 0x00, 0x53, 0x00, 0x88, 0x00, 0xc0, 0x01, 0x10, 0x01, 0x52, 0x01, 0xB0, 0x01, 0xE0, 0x02, 0x30, 0x02, 0x72, 0x02, 0x73, 0x02, 0xAC, 0x02, 0xE4, 0x03, 0x04, 0x03, 0x28, 0x03, 0x40, 0x03, 0x5B, 0x03, 0x72, 0x03, 0x90, 0x03, 0x9b, 0x03, 0xAB, 0x03, 0xBC, 0x03, 0xFF}},
	{0xD2, 52,{0x00, 0x00, 0x00, 0x02, 0x00, 0x1B, 0x00, 0x28, 0x00, 0x53, 0x00, 0x88, 0x00, 0xc0, 0x01, 0x10, 0x01, 0x52, 0x01, 0xB0, 0x01, 0xE0, 0x02, 0x30, 0x02, 0x72, 0x02, 0x73, 0x02, 0xAC, 0x02, 0xE4, 0x03, 0x04, 0x03, 0x28, 0x03, 0x40, 0x03, 0x5B, 0x03, 0x72, 0x03, 0x90, 0x03, 0x9b, 0x03, 0xAB, 0x03, 0xBC, 0x03, 0xFF}},
	{0xD3, 52,{0x00, 0x00, 0x00, 0x02, 0x00, 0x1B, 0x00, 0x28, 0x00, 0x53, 0x00, 0x88, 0x00, 0xc0, 0x01, 0x10, 0x01, 0x52, 0x01, 0xB0, 0x01, 0xE0, 0x02, 0x30, 0x02, 0x72, 0x02, 0x73, 0x02, 0xAC, 0x02, 0xE4, 0x03, 0x04, 0x03, 0x28, 0x03, 0x40, 0x03, 0x5B, 0x03, 0x72, 0x03, 0x90, 0x03, 0x9b, 0x03, 0xAB, 0x03, 0xBC, 0x03, 0xFF}},
	{0xD4, 52,{0x00, 0x00, 0x00, 0x02, 0x00, 0x1B, 0x00, 0x28, 0x00, 0x53, 0x00, 0x88, 0x00, 0xc0, 0x01, 0x10, 0x01, 0x52, 0x01, 0xB0, 0x01, 0xE0, 0x02, 0x30, 0x02, 0x72, 0x02, 0x73, 0x02, 0xAC, 0x02, 0xE4, 0x03, 0x04, 0x03, 0x28, 0x03, 0x40, 0x03, 0x5B, 0x03, 0x72, 0x03, 0x90, 0x03, 0x9b, 0x03, 0xAB, 0x03, 0xBC, 0x03, 0xFF}},
	{0xD5, 52,{0x00, 0x00, 0x00, 0x02, 0x00, 0x1B, 0x00, 0x28, 0x00, 0x53, 0x00, 0x88, 0x00, 0xc0, 0x01, 0x10, 0x01, 0x52, 0x01, 0xB0, 0x01, 0xE0, 0x02, 0x30, 0x02, 0x72, 0x02, 0x73, 0x02, 0xAC, 0x02, 0xE4, 0x03, 0x04, 0x03, 0x28, 0x03, 0x40, 0x03, 0x5B, 0x03, 0x72, 0x03, 0x90, 0x03, 0x9b, 0x03, 0xAB, 0x03, 0xBC, 0x03, 0xFF}},
	{0xD6, 52,{0x00, 0x00, 0x00, 0x02, 0x00, 0x1B, 0x00, 0x28, 0x00, 0x53, 0x00, 0x88, 0x00, 0xc0, 0x01, 0x10, 0x01, 0x52, 0x01, 0xB0, 0x01, 0xE0, 0x02, 0x30, 0x02, 0x72, 0x02, 0x73, 0x02, 0xAC, 0x02, 0xE4, 0x03, 0x04, 0x03, 0x28, 0x03, 0x40, 0x03, 0x5B, 0x03, 0x72, 0x03, 0x90, 0x03, 0x9b, 0x03, 0xAB, 0x03, 0xBC, 0x03, 0xFF}},
    {0x11, 1,{0x00}},
    {REGFLAG_DELAY, 120, {}},
    {0x29, 1, {0X00}},
    {REGFLAG_DELAY, 20, {}},

	// Setting ending by predefined flag
   {REGFLAG_END_OF_TABLE, 0x00, {}}

};

static struct LCM_setting_table lcm_sleep_In_setting[] = {
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
    
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;
    
    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_TWO_LANE;
    
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding 	= LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format	= LCM_DSI_FORMAT_RGB888;
    
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


static void lcm_init(void)
{
    SET_RESET_PIN(1);
	MDELAY(10);//Must > 5ms
    SET_RESET_PIN(0);
    MDELAY(50);//Must > 5ms
    SET_RESET_PIN(1);
    MDELAY(120);//Must > 50ms

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
	#ifdef BUILD_LK
    printf("gc9503_fwvga_dsi_vdo_boe_tengjun_o1 lcm_init\n");
	#else
	printk("gc9503_fwvga_dsi_vdo_boe_tengjun_o1 lcm_init\n");
	#endif
}


static void lcm_suspend(void)
{
	
	
    push_table(lcm_sleep_In_setting, sizeof(lcm_sleep_In_setting) / sizeof(struct LCM_setting_table), 1);
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
	printf("uboot gc9503_fwvga_dsi_vdo_boe_tengjun_o1 %s\n", __func__);
#else
	printk("kernel gc9503_fwvga_dsi_vdo_boe_tengjun_o1 %s\n", __func__);
#endif
	lcm_init();
}



static unsigned int lcm_compare_id(void)
{
   int array[4];
   char buffer[5];
	int   id=0;
	
    SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
    MDELAY(5);
    SET_RESET_PIN(0);
	MDELAY(25);
	SET_RESET_PIN(1);
	MDELAY(120);  

   array[0] = 0x00033700;
    dsi_set_cmdq(array, 1, 1);
    
   read_reg_v2(0x04, buffer, 3);

   id = buffer[0];
   id = (id << 8) | buffer[1];
    
    #ifdef BUILD_LK
		printf("gc9503_fwvga_dsi_vdo_boe_tengjun_o1 uboot id = 0x%x  %x %x %x\n", id, buffer[0], buffer[1], buffer[2]);
	#else
		printk("gc9503_fwvga_dsi_vdo_boe_tengjun_o1 kernel id = 0x%x  %x %x %x\n", id, buffer[0], buffer[1], buffer[2]);
	#endif
	
	if((0x95 == buffer[1]) && ((0x03 == buffer[2]) || (0x04 == buffer[2]))){
			return 1;
		}
	return 0;
	
}


LCM_DRIVER gc9503_fwvga_dsi_vdo_boe_tengjun_o1_lcm_drv =
{
    .name			= "gc9503_fwvga_dsi_vdo_boe_tengjun_o1",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
    .compare_id     = lcm_compare_id,        
};

