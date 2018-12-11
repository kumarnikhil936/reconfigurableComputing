#include <iostream>

#include "hls_tut.h"

#define N_DATA_IN     10 * N_DATA_ADD
#define N_DATA_OUT    N_DATA_IN/N_DATA_ADD

using namespace std;

int main() {  
  stream_in_t inStream;
  stream_out_t outStream;

  ap_uint<8> data[DATA_HEIGHT][DATA_WIDTH];
  ap_uint<8> data_out[DATA_HEIGHT][DATA_WIDTH];

  // Initialize input data
  for(int i=0; i < DATA_HEIGHT; i++) {
    for(int j=0; j < DATA_WIDTH; j++) {
	if ((i%2==0) || (j%4==0)){
	      data[i][j] = 255;
	} else{
	      data[i][j] = 0;
	}
    }
  }

  // save to stream
  for(int i=0; i < DATA_HEIGHT; i++) {
	  for (int j=0; j < DATA_WIDTH; j++) {
		stream_t tmp;
		tmp.data = data[i][j]; // Copy data to stream object
		tmp.last = ((i==(DATA_HEIGHT-1)) && (j==(DATA_WIDTH-1))) ? 1 : 0;
		inStream.write( tmp ); // Write data on stream
	  }
  }

  assign_2(inStream, outStream); // Call IP core function
  
  // save from stream
  for(int i=0; i < DATA_HEIGHT; i++) {
	  for (int j=0; j < DATA_WIDTH; j++) {
		stream_t tmp;
		outStream.read(tmp);
		data_out[i][j] = tmp.data; // Copy data from stream object
	  }
  }

  // Visualize
  for(int i=0; i < 5; i++) {
  	for(int j=0; j < 10; j++) {
	    cout << i << "\t" << j << "\t" << data_out[i][j] << endl;
	}
  }

  return 0;
}
