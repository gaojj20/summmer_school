#include"dft.h"
#include"ap_int.h"
void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE],DTYPE real_op[SIZE], DTYPE imag_op[SIZE])
{
#pragma HLS INTERFACE m_axi port=imag_op offset=slave
#pragma HLS INTERFACE m_axi port=real_op offset=slave
#pragma HLS INTERFACE m_axi port=imag_sample offset=slave
#pragma HLS INTERFACE m_axi port=real_sample offset=slave
#pragma HLS INTERFACE s_axilite port=return bundle=ctrl
	int i, j;
    DTYPE c_0, s_0;
    DTYPE c_1, s_1;

		dft_jthCalculate:
   for (j = 0; j < SIZE; j += 2) {
			// Calculate each frequency domain sample iteratively
			dft_each_Calculate:
	  for (i = 0; i < SIZE; i += 1) {
#pragma HLS PIPELINE II=1;
				// Utilize HLS tool to calculate sine and cosine values
				c_0 = cos_coefficients_table[(ap_uint<10>)(i * j)];
				s_0 = sin_coefficients_table[(ap_uint<10>)(i * j)];
				c_1 = cos_coefficients_table[(ap_uint<10>)((i) * (j+1))];
				s_1 = sin_coefficients_table[(ap_uint<10>)((i) * (j+1))];
				// Multiply the current phasor with the appropriate input sample and keep
				// running sum
				real_op[i] += (real_sample[j] * c_0 - imag_sample[j] * s_0) +
								(real_sample[j + 1] * c_1 - imag_sample[j + 1] * s_1);
				imag_op[i] += (real_sample[j] * s_0 + imag_sample[j] * c_0) +
								(real_sample[j + 1] * s_1 + imag_sample[j + 1] * c_1);
			}
		}
}
