#include "filter.h"


void filter(hls::stream<ap_axiu<32,1,1,1> > &src_axi,
		hls::stream<ap_axiu<32,1,1,1> > &dst_axi)
{
#pragma HLS INTERFACE axis port=dst_axi
#pragma HLS INTERFACE axis port=src_axi
#pragma HLS DATAFLOW
#pragma HLS INTERFACE ap_ctrl_none port=return

	hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_10UC3> img1(MAX_HEIGHT, MAX_WIDTH);
	hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_10UC3> img2(MAX_HEIGHT, MAX_WIDTH);
	hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_10UC3> img1_1(MAX_HEIGHT, MAX_WIDTH);
	hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_10UC1> img1_1_Y(MAX_HEIGHT, MAX_WIDTH);
	hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_10UC1> img1_1_U(MAX_HEIGHT, MAX_WIDTH);
	hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_10UC1> img1_1_V(MAX_HEIGHT, MAX_WIDTH);
	hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_10UC1> img1_2(MAX_HEIGHT, MAX_WIDTH);

	const int k_dummy[3][3] =
	{ { -1, -1, -1 },
	  { -1,  8, -1 },
	  { -1, -1, -1 }
	};

	hls::Window<3,3,int> kernel;
	hls::Point_<int> anchor;

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			kernel.val[i][j] = k_dummy[i][j];

	anchor.x = -1;
	anchor.y = -1;

	hls::AXIvideo2Mat(src_axi, img1);

	hls::CvtColor<HLS_BGR2YCrCb>(img1, img1_1);
	hls::Split(img1_1, img1_1_Y, img1_1_U, img1_1_V);
	hls::Consume(img1_1_U);
	hls::Consume(img1_1_V);
	hls::Filter2D(img1_1_Y, img1_2, kernel, anchor);
	hls::CvtColor<HLS_GRAY2RGB>(img1_2, img2);

	hls::Mat2AXIvideo(img2, dst_axi);
}
