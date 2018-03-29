#include "processing.h"
#include "iir.h"
#define one_scale 32768

void calculateShelvingCoeff(float c_alpha, Int16* output)
{
	/* Your code here */

	output [0] = c_alpha * one_scale;
	output [1] = -one_scale;
	output [2] = one_scale - 1;
	output [3] = -c_alpha * one_scale;
}

void calculatePeekCoeff(float c_alpha, float c_beta, Int16* output)
{
	/* Your code here */

	output [0] = c_alpha * one_scale;
	output [1] = ((Int32)(-c_beta * (1 + c_alpha) * one_scale)) >> 1;
	output [2] = one_scale - 1;
	output [3] = one_scale - 1;
	output [4] = ((Int32)(-c_beta * (1 + c_alpha) * one_scale)) >> 1;
	output [5] = c_alpha * one_scale;
}

Int16 shelvingHP(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
	/* Your code here */

	Int16 temp = first_order_IIR(input, coeff, x_history, y_history) >> 1;
	input = input >> 1;

	return input - temp + _smpy(k, (input + temp));

}

Int16 shelvingLP(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
	/* Your code here */

	Int16 filter_output = first_order_IIR(input, coeff, x_history, y_history);
	Int32 out = ((Int32)input - filter_output) >> 1;
	Int16 temp_out;
	if (out >= 32767)
		temp_out =  32767;
	else if (out <= - 32768)
		temp_out = - 32768;
	else
		temp_out = (Int16)out;

	out = _smpy(temp_out, k) + (((Int32)input + filter_output) >> 1);
	if (out >= 32767)
		return 32767;
	else if (out <= - 32768)
		return - 32768;
	else
		return (Int16)out;

	return 0;

}

Int16 shelvingPeek(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
	/* Your code here */

	Int16 temp = second_order_IIR(input, coeff, x_history, y_history) >> 1;
	input = input >> 1;

	return input + temp + _smpy(k, (input - temp));

}
