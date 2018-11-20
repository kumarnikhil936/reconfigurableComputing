#include "hls_tut.h"
void assign_2(data_in_t& in, data_out_t& out) {
#pragma HLS INTERFACE ap_ctrl_hs port=return


  //data_in_t dataIn[DATA_HEIGHT][DATA_WIDTH];
  //data_out_t dataOut[DATA_HEIGHT][DATA_WIDTH];

  // Read from memory 120*90 values
  /*read : for(int i=0; i < DATA_HEIGHT; i++) {
		   for(int j=0; j < DATA_WIDTH; j++) {
			   in[i][j] = in[i+j];
		   }
		}*/

    int h_x[3][3], h_y[3][3];
    int g_x[DATA_HEIGHT][DATA_WIDTH], g_y[DATA_HEIGHT][DATA_WIDTH];

    g_x = {0};
    g_y = {0};

    h_x = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    h_y = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    process : for (int x=1; x < DATA_WIDTH-1; x++){
    	for (int y=1; y < DATA_HEIGHT-1; y++){
    		for (int i = -N; i < N+1; i++) {
    			for (int j = -N; j < N+1; j++) {
                    g_x[y][x] += in[y+i, x+j]*h_x[N+i, N+j];
				    g_y[y][x] += in[y+i, x+j]*h_y[N+i, N+j];
    			}
    		}
    	}
    }

    write : for (int a = 0; a < DATA_WIDTH; a++){
    	for (int b = 0; b < DATA_HEIGHT; b++){
    		out[a][b] = sqrt((pow(g_x[a][b], 2)+pow(g_y[a][b], 2)));
    	}
    }

}
