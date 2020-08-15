#include "xparameters.h"
#include "xgpio.h"
#include "sleep.h"

int main (void)
{
	XGpio btns, leds;
	int btns_check;

	print("-- Start of the Program --\r\n");

	XGpio_Initialize(&btns, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&btns, 1, 0xffffffff);

	XGpio_Initialize(&leds, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&leds, 2, 0x00000000);

	while (1)
	{
		btns_check = XGpio_DiscreteRead(&btns, 1);
		XGpio_DiscreteWrite(&leds, 2, btns_check);
		xil_printf("Button Status : %x\r\n", btns_check);
		sleep(1);
	}
}