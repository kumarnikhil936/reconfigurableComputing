#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
using namespace std;

#define N 1
#define DATA_WIDTH   120
#define DATA_HEIGHT  90
#define N_DIM 3
#define N_PIXELS 2*DATA_WIDTH+N_DIM
#define MAX_PIXEL DATA_HEIGHT*(DATA_WIDTH-2)-2

typedef ap_axiu<8,4,5,5> stream_t; // Defines the signals of AXIS
typedef hls::stream<stream_t> stream_in_t;
typedef hls::stream<stream_t> stream_out_t;

void assign_2(stream_in_t& inStream, stream_out_t& outStream); // Top-level function
