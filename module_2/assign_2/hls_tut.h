#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
using namespace std;

#define N 1
#define DATA_WIDTH   120
#define DATA_HEIGHT  90

// typedef ap_axiu<DATA_WIDTH_IN,1,1,1> data_in_t; // Defines the signals of AXIS
// typedef hls::stream<data_in_t> stream_in_t;

// typedef ap_axiu<DATA_WIDTH_OUT,1,1,1> data_out_t; // Defines the signals of AXIS
// typedef hls::stream<data_out_t> stream_out_t;

//void assign_2(data_in_t& in, data_out_t& out, bool start, bool idle, bool done); // Top-level function
// void hls_tut(stream_in_t& inStream, stream_out_t& outStream); // Top-level function

void assign_2(ap_uint<8> in[DATA_HEIGHT][DATA_WIDTH], ap_uint<8> out[DATA_HEIGHT][DATA_WIDTH]); // Top-level function
//void assign_2(ap_uint<8> in, ap_int<8> out); // Top-level function
