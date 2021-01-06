#ifndef _FILTER_H
#define _FILTER_H

#include "hls_stream.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "common/xf_infra.hpp"
#include "imgproc/xf_cvt_color.hpp"
#include "imgproc/xf_custom_convolution.hpp"

#define MAX_HEIGHT 1080
#define MAX_WIDTH 1920
#define NPC XF_NPPC1

void filter(hls::stream<ap_axiu<24,1,1,1>> &src_axi,
			hls::stream<ap_axiu<24,1,1,1>> &dst_axi);

#endif

