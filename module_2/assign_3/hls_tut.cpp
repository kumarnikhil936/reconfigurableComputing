#include "hls_tut.h"

void assign_2(stream_in_t& inStream, stream_out_t& outStream) {
#pragma HLS INTERFACE axis register both port=outStream
#pragma HLS INTERFACE axis register both port=inStream
#pragma HLS INTERFACE ap_ctrl_none port=return

    const ap_int<3> h_x[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    const ap_int<3> h_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    ap_int<11> g_x = 0;
    ap_int<11> g_y = 0;
	const ap_int<11> zero = 0;
    const ap_int<8> zero_out = 0;
	ap_uint<8> dataIn[DATA_WIDTH*DATA_HEIGHT];
	ap_uint<8> out;
	bool inside = false;
	bool fLast = false;
	int index = 0;
	int cur_index = 0;
	stream_t tmp_in;
	
	// write out zeros for top row+1
	write_start : for (int i=0; i<=DATA_WIDTH; i++){
		stream_t dataOut;
		dataOut.strb = -1;
		dataOut.keep = 15;
		dataOut.user = 0;
		dataOut.id = 0;
		dataOut.dest = 0;
		dataOut.data = zero_out;
		dataOut.last = 0;
		outStream.write(dataOut);
	}
	
	// PreRead from stream
	read : for(index=0; index < N_PIXELS; index++) {
		inStream.read(tmp_in);
		dataIn[index] = tmp_in.data;
		fLast = tmp_in.last;
	}
	index -= 1;

	// process DATA_HEIGHT-2 rows
	proc : do {
		// compute filter for current pixel
		process : {
		    g_x = 0;
		    g_y = 0;
			out = 0;
			cur_index = index - 2*DATA_WIDTH - 2 ;
    		for_2 : for (int i = 0; i < N_DIM; i++, cur_index+=DATA_WIDTH) {
    			for_3 : for (int j = 0; j < N_DIM; j++) {
    				//cout << "data in " << dataIn[cur_index+j].data << endl;
                    g_x += dataIn[cur_index+j]*h_x[i][j];
                    g_y += dataIn[cur_index+j]*h_y[i][j];
    			}
    		}
			
			// check if on the border (%120, %119)
			inside = !(((index % DATA_WIDTH) == 0) || ((index % DATA_WIDTH) == 1));
		    if (inside){
				if (g_x < zero) g_x = -g_x;
				if (g_y < zero) g_y = -g_y;
				out = (g_x+g_y)>>3;  // normalize to 0-255
		    }else{
		    	out = 0;
		    }

		    //cout << "out " << out << " index " << index << endl;
    	}
	
		// write current result on the stream
		write : {
			stream_t dataOut;
			dataOut.strb = -1;
			dataOut.keep = 15;
			dataOut.user = 0;
			dataOut.id = 0;
			dataOut.dest = 0;
			dataOut.data = out;
			dataOut.last = 0;
			outStream.write(dataOut);
		}
		// Read next pixel from stream
		fLast = tmp_in.last;
		index += 1;
		if (!fLast) {
			inStream.read(tmp_in);
			dataIn[index] = tmp_in.data;
		}
	} while(!fLast);
	
	// write out zeros for last row+1
	write_end : for (int i=0; i<=DATA_WIDTH; i++){
		stream_t dataOut;
		dataOut.strb = -1;
		dataOut.keep = 15;
		dataOut.user = 0;
		dataOut.id = 0;
		dataOut.dest = 0;
		dataOut.data = zero_out;
		dataOut.last = (i==DATA_WIDTH) ? 1 : 0;
		outStream.write(dataOut);
	}
}

