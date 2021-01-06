#include "filter.h"

void filter(hls::stream<ap_axiu<24,1,1,1>> &src_axi,
			hls::stream<ap_axiu<24,1,1,1>> &dst_axi)
{
#pragma HLS INTERFACE axis port=dst_axi
#pragma HLS INTERFACE axis port=src_axi
#pragma HLS DATAFLOW
#pragma HLS INTERFACE ap_ctrl_none port=return

	xf::cv::Mat<XF_8UC3,MAX_HEIGHT, MAX_WIDTH, NPC> img1(MAX_HEIGHT, MAX_WIDTH);
	xf::cv::Mat<XF_8UC1,MAX_HEIGHT, MAX_WIDTH, NPC> img2(MAX_HEIGHT, MAX_WIDTH);
	xf::cv::Mat<XF_8UC1,MAX_HEIGHT, MAX_WIDTH, NPC> img3(MAX_HEIGHT, MAX_WIDTH);
	xf::cv::Mat<XF_8UC3,MAX_HEIGHT, MAX_WIDTH, NPC> img4(MAX_HEIGHT, MAX_WIDTH);

	short int kernel[9] = {
			-1, -1, -1,
			-1,  8, -1,
			-1, -1, -1
	};

	xf::cv::AXIvideo2xfMat(src_axi, img1);

	xf::cv::rgb2gray<XF_8UC3, XF_8UC1, MAX_HEIGHT, MAX_WIDTH, NPC>(img1, img2);

	xf::cv::filter2D<XF_BORDER_CONSTANT, 3, 3, XF_8UC1, XF_8UC1, MAX_HEIGHT, MAX_WIDTH, NPC>(img2, img3, kernel, 0);

	xf::cv::gray2rgb<XF_8UC1, XF_8UC3, MAX_HEIGHT, MAX_WIDTH, NPC>(img3, img4);

	xf::cv::xfMat2AXIvideo(img4, dst_axi);
}

