#include "hls_tut.h"


void assign_2(ap_uint<8> in[DATA_HEIGHT][DATA_WIDTH], ap_uint<8> out[DATA_HEIGHT][DATA_WIDTH]) {
#pragma HLS RESOURCE variable=out core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=out
#pragma HLS RESOURCE variable=in core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=in


	#pragma HLS INTERFACE ap_ctrl_hs port=return

    const ap_int<3> h_x[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    const ap_int<3> h_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    ap_int<11> g_x = 0;
    ap_int<11> g_y = 0;
	const ap_int<11> zero = 0;

    process : for (int x=0; x < DATA_WIDTH; x++){
    	for_1 : for (int y=0; y < DATA_HEIGHT; y++){
		    g_x = 0;
		    g_y = 0;
    		for_2 : for (int i = -N; (i < N+1); i++) {
    			for_3 : for (int j = -N; j < N+1; j++) {
                    g_x += in[y+i][x+j]*h_x[N+i][N+j];
				    g_y += in[y+i][x+j]*h_y[N+i][N+j];
    			}
    		}

		    bool inside =  (x>0) & (x<DATA_WIDTH-1) & (y>0) & (y<DATA_HEIGHT-1);
		    if (inside){
				if (g_x < zero) g_x = -g_x;
				if (g_y < zero) g_y = -g_y;
				out[y][x] = (g_x+g_y)>>3;  // normalize to 0-255
		    }else{
		    	out[y][x] = 0;
		    }
    	}
    }
}

