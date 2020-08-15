/*
FILENAME: ov5640.c
AUTHOR: Greg Taylor     CREATION DATE: 12 Aug 2019

DESCRIPTION:

CHANGE HISTORY:
12 Aug 2019		Greg Taylor
	Initial version

MIT License

Copyright (c) 2019 Greg Taylor <gtaylor@sonic.net>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include "sleep.h"
#include "platform.h"
#include "xgpiops.h"
#include "xiicps.h"
#include "ov5640.h"

static XGpioPs gpio;
static XIicPs iic;

int ov5640_init() {
	XGpioPs_Config *gpio_config;
	XIicPs_Config *iic_config;
	u8 bit_mask;
	u8 addr[2];
	u8 camera_model_id[2];

	if ( (gpio_config = XGpioPs_LookupConfig(XPAR_PSU_GPIO_0_DEVICE_ID)) == NULL) {
		xil_printf("XGpioPs_LookupConfig() failed\r\n");
		return XST_FAILURE;
	}
	if (XGpioPs_CfgInitialize(&gpio, gpio_config, gpio_config->BaseAddr)) {
		xil_printf("XGpioPs_CfgInitialize() failed\r\n");
		return XST_FAILURE;
	}

	// Reset and enable ov5640 power supplies
	XGpioPs_SetDirectionPin(&gpio, ov5640_ENABLE_GPIO_PIN, 1);
	XGpioPs_SetOutputEnablePin(&gpio, ov5640_ENABLE_GPIO_PIN, 1);
	XGpioPs_WritePin(&gpio, ov5640_ENABLE_GPIO_PIN, 0);
	usleep(100000);
	XGpioPs_WritePin(&gpio, ov5640_ENABLE_GPIO_PIN, 1);
	usleep(100000);

	xil_printf("Reset and enabled ov5640 power supplies\r\n");

	// Reset i2c expander
	XGpioPs_SetDirectionPin(&gpio, ULTRA_96_I2C_EXPANDER_RESET_N_GPIO_PIN, 1);
	XGpioPs_SetOutputEnablePin(&gpio, ULTRA_96_I2C_EXPANDER_RESET_N_GPIO_PIN, 1);
	XGpioPs_WritePin(&gpio, ULTRA_96_I2C_EXPANDER_RESET_N_GPIO_PIN, 0);
	XGpioPs_WritePin(&gpio, ULTRA_96_I2C_EXPANDER_RESET_N_GPIO_PIN, 1);

    if ( (iic_config = XIicPs_LookupConfig(XPAR_PSU_I2C_1_DEVICE_ID)) == NULL) {
		xil_printf("XIicPs_LookupConfig() failed\r\n");
		return XST_FAILURE;
	}
    if (XIicPs_CfgInitialize(&iic, iic_config, iic_config->BaseAddress) != XST_SUCCESS) {
		xil_printf("XIicPs_CfgInitialize() failed\r\n");
		return XST_FAILURE;
	}

	if (XIicPs_SelfTest(&iic) != XST_SUCCESS) {
		xil_printf("XIicPs_SelfTest() failed\r\n");
		return XST_FAILURE;
	}

	if (XIicPs_SetSClk(&iic, I2C_BUS_FREQ) != XST_SUCCESS) {
		xil_printf("XIicPs_SetSClk failed\r\n");
		return XST_FAILURE;
	}

	// Read i2c expander chip control reg
	if (XIicPs_MasterRecvPolled(&iic, &bit_mask, 1, ULTRA_96_I2C_EXPANDER_SLAVE_ADDR) != XST_SUCCESS) {
		xil_printf("i2c expander receive failed\r\n");
		return XST_FAILURE;
	}
	usleep(1000); // chip needs some delay for some reason
    bit_mask |= ULTRA_96_I2C_EXPANDER_HSEXP_I2C2_BIT_MASK;
	if (XIicPs_MasterSendPolled(&iic, &bit_mask, 1, ULTRA_96_I2C_EXPANDER_SLAVE_ADDR) != XST_SUCCESS) {
		xil_printf("i2c expander send failed\r\n");
		return XST_FAILURE;
	}

	memset(addr, 0, sizeof(addr));
	if (XIicPs_MasterSendPolled(&iic, addr, 2, ov5640_I2C_SLAVE_ADDR) != XST_SUCCESS) {
		xil_printf("ov5640 send failed\r\n");
		return XST_FAILURE;
	}
	if (XIicPs_MasterRecvPolled(&iic, camera_model_id, 2, ov5640_I2C_SLAVE_ADDR) != XST_SUCCESS) {
		xil_printf("ov5640 receive failed\r\n");
		return XST_FAILURE;
	}

	if (camera_model_id[0] != 0x2 && camera_model_id[1] == 0x19) {
		xil_printf("could not read camera id\r\n");
		return XST_FAILURE;
	}
	else {
		xil_printf("I2C communication established with ov5640\r\n");
	}

	/**************************************************************************************************/

	ov5640_write(0x3103, 0x11);
	ov5640_write(0x3008, 0x82);

	usleep(1000 * 1000);


/**********ov5640_init******************************************************************************/

	//[7]=0 Software reset; [6]=1 Software power down; Default=0x02
	ov5640_write(0x3008, 0x42);
	//[1]=1 System input clock from PLL; Default read = 0x11
	ov5640_write(0x3103, 0x03);
	//[3:0]=0000 MD2P,MD2N,MCP,MCN input; Default=0x00
	ov5640_write(0x3017, 0x00);
	//[7:2]=000000 MD1P,MD1N, D3:0 input; Default=0x00
	ov5640_write(0x3018, 0x00);
	//[6:4]=001 PLL charge pump, [3:0]=1000 MIPI 8-bit mode
	ov5640_write(0x3034, 0x18);

	//PLL1 configuration
	//[7:4]=0001 System clock divider /1, [3:0]=0001 Scale divider for MIPI /1
	ov5640_write(0x3035, 0x11);
	//[7:0]=56 PLL multiplier
	ov5640_write(0x3036, 0x38);
	//[4]=1 PLL root divider /2, [3:0]=1 PLL pre-divider /1
	ov5640_write(0x3037, 0x11);
	//[5:4]=00 PCLK root divider /1, [3:2]=00 SCLK2x root divider /1, [1:0]=01 SCLK root divider /2
	ov5640_write(0x3108, 0x01);
	//PLL2 configuration
	//[5:4]=01 PRE_DIV_SP /1.5, [2]=1 R_DIV_SP /1, [1:0]=00 DIV12_SP /1
	ov5640_write(0x303D, 0x10);
	//[4:0]=11001 PLL2 multiplier DIV_CNT5B = 25
	ov5640_write(0x303B, 0x19);

	ov5640_write(0x3630, 0x2e);
	ov5640_write(0x3631, 0x0e);
	ov5640_write(0x3632, 0xe2);
	ov5640_write(0x3633, 0x23);
	ov5640_write(0x3621, 0xe0);
	ov5640_write(0x3704, 0xa0);
	ov5640_write(0x3703, 0x5a);
	ov5640_write(0x3715, 0x78);
	ov5640_write(0x3717, 0x01);
	ov5640_write(0x370b, 0x60);
	ov5640_write(0x3705, 0x1a);
	ov5640_write(0x3905, 0x02);
	ov5640_write(0x3906, 0x10);
	ov5640_write(0x3901, 0x0a);
	ov5640_write(0x3731, 0x02);
	//VCM debug mode
	ov5640_write(0x3600, 0x37);
	ov5640_write(0x3601, 0x33);
	//System control register changing not recommended
	ov5640_write(0x302d, 0x60);
	//??
	ov5640_write(0x3620, 0x52);
	ov5640_write(0x371b, 0x20);
	//?? DVP
	ov5640_write(0x471c, 0x50);

	ov5640_write(0x3a13, 0x43);
	ov5640_write(0x3a18, 0x00);
	ov5640_write(0x3a19, 0xf8);
	ov5640_write(0x3635, 0x13);
	ov5640_write(0x3636, 0x06);
	ov5640_write(0x3634, 0x44);
	ov5640_write(0x3622, 0x01);
	ov5640_write(0x3c01, 0x34);
	ov5640_write(0x3c04, 0x28);
	ov5640_write(0x3c05, 0x98);
	ov5640_write(0x3c06, 0x00);
	ov5640_write(0x3c07, 0x08);
	ov5640_write(0x3c08, 0x00);
	ov5640_write(0x3c09, 0x1c);
	ov5640_write(0x3c0a, 0x9c);
	ov5640_write(0x3c0b, 0x40);

	//[7]=1 color bar enable, [3:2]=00 eight color bar
	ov5640_write(0x503d, 0x00);
	//[2]=1 ISP vflip, [1]=1 sensor vflip
	ov5640_write(0x3820, 0x46);

	//[7:5]=010 Two lane mode, [4]=0 MIPI HS TX no power down, [3]=0 MIPI LP RX no power down, [2]=1 MIPI enable, [1:0]=10 Debug mode; Default=0x58
	ov5640_write(0x300e, 0x45);
	//[7:5]=010 One lane mode, [4]=0 MIPI HS TX no power down, [3]=0 MIPI LP RX no power down, [2]=1 MIPI enable, [1:0]=10 Debug mode; Default=0x58
	//ov5640_write(0x300e, 0x25);
	//[5]=0 Clock free running, [4]=1 Send line short packet, [3]=0 Use lane1 as default, [2]=1 MIPI bus LP11 when no packet; Default=0x04
	ov5640_write(0x4800, 0x14);
	ov5640_write(0x302e, 0x08);
	//[7:4]=0x3 YUV422, [3:0]=0x0 YUYV
	//ov5640_write(0x4300, 0x30);
	//[7:4]=0x6 RGB565, [3:0]=0x0 ov5640_write(b[4:0],g[5:3],g[2:0],r[4:0]}
	ov5640_write(0x4300, 0x6f);
	ov5640_write(0x501f, 0x01);

	ov5640_write(0x4713, 0x03);
	ov5640_write(0x4407, 0x04);
	ov5640_write(0x440e, 0x00);
	ov5640_write(0x460b, 0x35);
	//[1]=0 DVP PCLK divider manual control by 0x3824[4:0]
	ov5640_write(0x460c, 0x20);
	//[4:0]=1 SCALE_DIV=INT(3824[4:0]/2)
	ov5640_write(0x3824, 0x01);

	//MIPI timing
	//		ov5640_write(0x4805, 0x10); //LPX global timing select=auto
	//		ov5640_write(0x4818, 0x00); //hs_prepare + hs_zero_min ns
	//		ov5640_write(0x4819, 0x96);
	//		ov5640_write(0x482A, 0x00); //hs_prepare + hs_zero_min UI
	//
	//		ov5640_write(0x4824, 0x00); //lpx_p_min ns
	//		ov5640_write(0x4825, 0x32);
	//		ov5640_write(0x4830, 0x00); //lpx_p_min UI
	//
	//		ov5640_write(0x4826, 0x00); //hs_prepare_min ns
	//		ov5640_write(0x4827, 0x32);
	//		ov5640_write(0x4831, 0x00); //hs_prepare_min UI

	//[7]=1 LENC correction enabled, [5]=1 RAW gamma enabled, [2]=1 Black pixel cancellation enabled, [1]=1 White pixel cancellation enabled, [0]=1 Color interpolation enabled
	ov5640_write(0x5000, 0x07);
	//[7]=0 Special digital effects, [5]=0 scaling, [2]=0 UV average disabled, [1]=1 Color matrix enabled, [0]=1 Auto white balance enabled
	ov5640_write(0x5001, 0x03);

/**********************************************************************************************/

	ov5640_write(0x3008, 0x42);

/*************ov5640_1920_1080_30*****************************************************************/

	//PLL1 configuration
	//[7:4]=0010 System clock divider /2, [3:0]=0001 Scale divider for MIPI /1
	ov5640_write(0x3035, 0x21); // 30fps setting
	//[7:0]=105 PLL multiplier
	ov5640_write(0x3036, 0x69);
	//[4]=0 PLL root divider /1, [3:0]=5 PLL pre-divider /1.5
	ov5640_write(0x3037, 0x05);
	//[5:4]=01 PCLK root divider /2, [3:2]=00 SCLK2x root divider /1, [1:0]=01 SCLK root divider /2
	ov5640_write(0x3108, 0x11);

	//[6:4]=001 PLL charge pump, [3:0]=1010 MIPI 10-bit mode
	ov5640_write(0x3034, 0x1A);

	//[3:0]=0 X address start high byte
	ov5640_write(0x3800, (336 >> 8) & 0x0F);
	//[7:0]=0 X address start low byte
	ov5640_write(0x3801, 336 & 0xFF);
	//[2:0]=0 Y address start high byte
	ov5640_write(0x3802, (426 >> 8) & 0x07);
	//[7:0]=0 Y address start low byte
	ov5640_write(0x3803, 426 & 0xFF);

	//[3:0] X address end high byte
	ov5640_write(0x3804, (2287 >> 8) & 0x0F);
	//[7:0] X address end low byte
	ov5640_write(0x3805, 2287 & 0xFF);
	//[2:0] Y address end high byte
	ov5640_write(0x3806, (1529 >> 8) & 0x07);
	//[7:0] Y address end low byte
	ov5640_write(0x3807, 1529 & 0xFF);

	//[3:0]=0 timing hoffset high byte
	ov5640_write(0x3810, (16 >> 8) & 0x0F);
	//[7:0]=0 timing hoffset low byte
	ov5640_write(0x3811, 16 & 0xFF);
	//[2:0]=0 timing voffset high byte
	ov5640_write(0x3812, (12 >> 8) & 0x07);
	//[7:0]=0 timing voffset low byte
	ov5640_write(0x3813, 12 & 0xFF);

	//[3:0] Output horizontal width high byte
	ov5640_write(0x3808, (1920 >> 8) & 0x0F);
	//[7:0] Output horizontal width low byte
	ov5640_write(0x3809, 1920 & 0xFF);
	//[2:0] Output vertical height high byte
	ov5640_write(0x380a, (1080 >> 8) & 0x7F);
	//[7:0] Output vertical height low byte
	ov5640_write(0x380b, 1080 & 0xFF);

	//HTS line exposure time in # of pixels Tline=HTS/sclk
	ov5640_write(0x380c, (2500 >> 8) & 0x1F);
	ov5640_write(0x380d, 2500 & 0xFF);
	//VTS frame exposure time in # lines
	ov5640_write(0x380e, (1120 >> 8) & 0xFF);
	ov5640_write(0x380f, 1120 & 0xFF);

	//[7:4]=0x1 horizontal odd subsample increment, [3:0]=0x1 horizontal even subsample increment
	ov5640_write(0x3814, 0x11);
	//[7:4]=0x1 vertical odd subsample increment, [3:0]=0x1 vertical even subsample increment
	ov5640_write(0x3815, 0x11);

	//[2]=0 ISP mirror, [1]=0 sensor mirror, [0]=0 no horizontal binning
	ov5640_write(0x3821, 0x00);

	//little MIPI shit: global timing unit, period of PCLK in ns * 2(depends on # of lanes)
	ov5640_write(0x4837, 24); // 1/84M*2

	//Undocumented anti-green settings
	ov5640_write(0x3618, 0x00); // Removes vertical lines appearing under bright light
	ov5640_write(0x3612, 0x59);
	ov5640_write(0x3708, 0x64);
	ov5640_write(0x3709, 0x52);
	ov5640_write(0x370c, 0x03);

	//[7:4]=0x0 Formatter RAW, [3:0]=0x0 BGBG/GRGR
	ov5640_write(0x4300, 0x00);
	//[2:0]=0x3 Format select ISP RAW (DPC)
	ov5640_write(0x501f, 0x03);

/************ov5640_advanced_awb****************************************************************/

		// Enable Advanced AWB
	ov5640_write(0x3406 ,0x00);
	ov5640_write(0x5192 ,0x04);
	ov5640_write(0x5191 ,0xf8);
	ov5640_write(0x518d ,0x26);
	ov5640_write(0x518f ,0x42);
	ov5640_write(0x518e ,0x2b);
	ov5640_write(0x5190 ,0x42);
	ov5640_write(0x518b ,0xd0);
	ov5640_write(0x518c ,0xbd);
	ov5640_write(0x5187 ,0x18);
	ov5640_write(0x5188 ,0x18);
	ov5640_write(0x5189 ,0x56);
	ov5640_write(0x518a ,0x5c);
	ov5640_write(0x5186 ,0x1c);
	ov5640_write(0x5181 ,0x50);
	ov5640_write(0x5184 ,0x20);
	ov5640_write(0x5182 ,0x11);
	ov5640_write(0x5183 ,0x00);
	ov5640_write(0x5001 ,0x03);

/**********************************************************************************************/

	ov5640_write(0x3008, 0x02);

	return XST_SUCCESS;
}

int ov5640_write(u16 addr, u8 data) {
	u8 buf[3];

	buf[0] = addr >> 8;
	buf[1] = addr & 0xff;
	buf[2] = data;

	while (TransmitFifoFill(&iic) || XIicPs_BusIsBusy(&iic)) { //while (XIicPs_BusIsBusy(&iic)) {
		usleep(1);
		xil_printf("waiting for transmit...\r\n");
	}

	if (XIicPs_MasterSendPolled(&iic, buf, 3, ov5640_I2C_SLAVE_ADDR) != XST_SUCCESS) {
		xil_printf("ov5640 write failed, addr: %x\r\n", addr);
		return XST_FAILURE;
	}
	usleep(1000);

	return XST_SUCCESS;
}

int ov5640_read(u16 addr, u8 *data) {
	u8 buf[2];

	buf[0] = addr >> 8;
	buf[1] = addr & 0xff;

	if (XIicPs_MasterSendPolled(&iic, buf, 2, ov5640_I2C_SLAVE_ADDR) != XST_SUCCESS) {
		xil_printf("ov5640 write failed\r\n");
		return XST_FAILURE;
	}
	if (XIicPs_MasterRecvPolled(&iic, data, 1, ov5640_I2C_SLAVE_ADDR) != XST_SUCCESS) {
		xil_printf("ov5640 receive failed\r\n");
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}
