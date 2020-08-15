/*
FILENAME: ov5640.h
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
#ifndef SRC_ov5640_H_
#define SRC_ov5640_H_

#define I2C_BUS_FREQ 400000 // in Hz

//#define ov5640_ENABLE_GPIO_PIN 37
#define ov5640_ENABLE_GPIO_PIN 36
//#define IMX219_I2C_SLAVE_ADDR  0x10
#define ov5640_I2C_SLAVE_ADDR  0x3c

#define ULTRA_96_I2C_EXPANDER_RESET_N_GPIO_PIN    12
#define ULTRA_96_I2C_EXPANDER_SLAVE_ADDR          0x75
//#define ULTRA_96_I2C_EXPANDER_HSEXP_I2C2_BIT_MASK 0b00000100
#define ULTRA_96_I2C_EXPANDER_HSEXP_I2C2_BIT_MASK 0b00000010

/* Lens */
/* infinity, 0 current */
#define ov5640_LENS_MIN					0
/* macro, max current */
#define ov5640_LENS_MAX					255
#define ov5640_LENS_STEP				1
/* AEC */
#define ov5640_DEFAULT_EXP		                10000
#define ov5640_DEFAULT_GAIN		                UINT8P8(1.0)
#define ov5640_GAIN_MIN					UINT8P8(1.0)
#define ov5640_GAIN_MAX					UINT8P8(8.0)
#define ov5640_EXP_MIN					1
#define ov5640_ANA_GAIN_GLOBAL                          0x0157
#define ov5640_COARSE_INT_TIME_HI                       0x015A
#define ov5640_COARSE_INT_TIME_LO                       0x015B
#define ov5640_FRM_LENGTH_HI                            0x0160
#define ov5640_FRM_LENGTH_LO                            0x0161

int ov5640_init();
int ov5640_write(u16 addr, u8 data);
int ov5640_read(u16 addr, u8 *data);

#endif /* SRC_ov5640_H_ */
