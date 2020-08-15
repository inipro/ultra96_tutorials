#include <stdio.h>
#include <string.h>
#include <ap_int.h>

void simple_assign(
		ap_uint<8> pmod_c,
		ap_uint<8> *pmod_a){
#pragma HLS INTERFACE ap_none port=pmod_a
#pragma HLS INTERFACE ap_none port=pmod_c
#pragma HLS INTERFACE ap_ctrl_none port=return

	*pmod_a = pmod_c;
}
