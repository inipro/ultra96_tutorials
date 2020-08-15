#include <hls_video.h>

#define MAX_HEIGHT 1080
#define MAX_WIDTH 1920


void filter(hls::stream<ap_axiu<32,1,1,1> > &src_axi,
		hls::stream<ap_axiu<32,1,1,1> > &dst_axi,
		int rows, int cols);
