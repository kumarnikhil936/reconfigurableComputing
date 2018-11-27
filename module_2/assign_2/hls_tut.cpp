#include "hls_tut.h"
void assign_2(ap_uint<8> in[DATA_HEIGHT][DATA_WIDTH], ap_uint<8> out[DATA_HEIGHT][DATA_WIDTH]) {
#pragma HLS RESOURCE variable=out core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=out
#pragma HLS RESOURCE variable=in core=RAM_1P_BRAM
#pragma HLS INTERFACE bram port=in


	#pragma HLS INTERFACE ap_ctrl_hs port=return

    int g_x[DATA_HEIGHT][DATA_WIDTH] = {0};
    int g_y[DATA_HEIGHT][DATA_WIDTH] = {0};

    int h_x[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    int h_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    process : for (int x=1; x < DATA_WIDTH-1; x++){
    	for (int y=1; y < DATA_HEIGHT-1; y++){
    		for (int i = -N; i < N+1; i++) {
    			for (int j = -N; j < N+1; j++) {
                    g_x[y][x] += (int(in[y+i][x+j])*h_x[N+i][N+j]);
				    g_y[y][x] += (int(in[y+i][x+j])*h_y[N+i][N+j]);
    			}
    		}
    	}
    }

    write : for (int a = 0; a < DATA_WIDTH; a++){
    	for (int b = 0; b < DATA_HEIGHT; b++){
    		//out[b][a] = sqrt((pow(g_x[b][a], 2)+pow(g_y[b][a], 2)))/4.5;  // normalize to range 0-255
    		out[b][a] = (fabs(g_x[b][a])+fabs(g_y[b][a]))/4.5;
    	}
    }

}
