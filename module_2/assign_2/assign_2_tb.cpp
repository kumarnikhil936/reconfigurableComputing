#include <iostream>

#include "hls_tut.h"

#define N_DATA_IN     10 * N_DATA_ADD
#define N_DATA_OUT    N_DATA_IN/N_DATA_ADD

using namespace std;

int main() {
  //ap_uint<8> inData[DATA_HEIGHT][DATA_WIDTH];
  ap_uint<8> outData[DATA_HEIGHT][DATA_WIDTH];

  ap_uint<8> data[DATA_HEIGHT][DATA_WIDTH];
  //ap_uint<8> goldenRes[DATA_HEIGHT][DATA_WIDTH];

  // Initialize input data
  for(int i=0; i < DATA_HEIGHT; i++) {
    //cout << i*N_DATA_OUT << "..." << i*N_DATA_OUT+N_DATA_OUT-1 << ": ";
    for(int j=0; j < DATA_WIDTH; j++) {
	if ((i%2==0) | (j%4==0)){
	      data[i][j] = 255;
	      //cout << data[i][j] << ", ";
	} else{
	      data[i][j] = 0;
	      //cout << data[i][j] << ", ";
	}
    }
    cout << endl;
  }

  // Compute golden result
  //for(int i=0; i < N_DATA_OUT; i++) {
   // ap_uint<16> sum = 0;
    //for(int j=0; j < N_DATA_ADD; j++) {
     // sum += data[i*N_DATA_ADD + j];
    //}
    //goldenRes[i] = sum;
  //}

  //for(int i=0; i < N_DATA_IN; i++) {
   // data_in_t tmp;
    //tmp.data = data[i]; // Copy data to stream object
    //tmp.last = (i==N_DATA_IN-1) ? 1 : 0;
    //inStream.write( tmp ); // Write data on stream
  //}

  assign_2(data, outData); // Call IP core function

  for(int i=0; i < 5; i++) {
  	for(int j=0; j < 10; j++) {
	    cout << i << "\t" << j << "\t" << outData[i][j] << endl;
	}
  }

  return 0;
}
