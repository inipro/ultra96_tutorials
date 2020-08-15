/******************************************************************************/
/*                                                                            */
/* PmodTMP2.h -- PmodTMP2 Library                                             */
/*                                                                            */
/******************************************************************************/
/* Author: Arthur Brown                                                       */
/* Copyright 2016, Digilent Inc.                                              */
/******************************************************************************/
/* Module Description:                                                        */
/*                                                                            */
/* This file contains code for running a demonstration of the PmodTMP2 when   */
/* PmodTMP2 when used with the PmodTMP2 IP core.                              */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    06/09/2016(ABrown):   Created                                           */
/*    05/08/2017(jPeyron):  Updated                                           */
/*    11/10/2017(atangzwj): Validated for Vivado 2016.4                       */
/*    01/13/2018(atangzwj): Validated for Vivado 2017.4                       */
/*                                                                            */
/******************************************************************************/

#ifndef PMODTMP2_H
#define PMODTMP2_H

/************ Include Files ************/

#include "xiic.h"
#include "xiic_l.h"
#include "xil_types.h"
#include "xstatus.h"


/************ Macro Definitions ************/

// Register addresses
#define TMP2_REG_TEMP     0x0
#define TMP2_REG_CONFIG   0x1
#define TMP2_REG_HYST     0x2
#define TMP2_REG_LIMITSET 0x3

// Configuration settings
#define TMP2_ONESHOT   0x80 //One Shot mode
#define TMP2_RES9      0x00 //9-bit resolution
#define TMP2_RES10     0x20 //10-bit resolution
#define TMP2_RES11     0x40 //11-bit resolution
#define TMP2_RES12     0x60 //12-bit resolution
#define TMP2_FAULT1    0x00 //1 fault queue bits
#define TMP2_FAULT2    0x08 //2 fault queue bits
#define TMP2_FAULT4    0x10 //4 fault queue bits
#define TMP2_FAULT6    0x18 //6 fault queue bits
#define TMP2_ALERTLOW  0x00 //Alert bit active-low
#define TMP2_ALERTHIGH 0x04 //Alert bit active-high
#define TMP2_CMPMODE   0x00 //comparator mode
#define TMP2_INTMODE   0x02 //interrupt mode
#define TMP2_SHUTDOWN  0x01 //Shutdown enabled
#define	TMP2_STARTUP   0x00 //Shutdown Disabled

//Device Constants
#define TMP2_ADDR 0x4B // Based on jumpers JP1, JP2, and JP3
                       // A table explaining the various address configurations
                       // is available in the PmodTMP2 reference manual

#define TMP2_CONF_DEFAULT \
      (TMP2_RES9 | TMP2_FAULT1 | TMP2_ALERTLOW | TMP2_CMPMODE)
      // Default Startup Configuration Used, this is just so the device can be
      // reset to startup configurations at a later time, it doesn't need to be
      // called anywhere.


/************ Type Definitions ************/

typedef struct PmodTMP2 {
   XIic TMP2Iic;
   u8 chipAddr;
   u8 currentRegister;
   u8 recvbytes;
   u8 *recv;
} PmodTMP2;


/************ Function Prototypes ************/

void TMP2_begin(PmodTMP2 *InstancePtr, u32 IIC_Address, u8 Chip_Address);
void TMP2_end(PmodTMP2 *InstancePtr);
int  TMP2_IICInit(XIic *IicInstancePtr);
void TMP2_ReadIIC(PmodTMP2 *InstancePtr, u8 reg, u8 *Data, int nData);
void TMP2_WriteIIC(PmodTMP2 *InstancePtr, u8 reg, u8 *Data, int nData);
void TMP2_config(PmodTMP2 *InstancePtr, u8 configuration);
double TMP2_getTemp(PmodTMP2 *InstancePtr);
double TMP2_FtoC(double tempF);
double TMP2_CtoF(double tempC);

#endif // PMODTMP2_H
