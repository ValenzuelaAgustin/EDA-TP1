#include "nmath.h"
#include <stdint.h>

/** 
 * @author Greg Walsh
 * @brief Inverse square root algorithm from Quake III engine
 * @cite 
 */

double Q_rsqrt(double number)
{
	int64_t i;
	double x2, y;
	const double threehalfs = 1.5;

	x2 = number * 0.5;
	y = number;
	i = * (int64_t*) &y;
	i = 0x5FE69FF2E48E8808 - ( i >> 1 );
	y = * (double*) &i;
	y = y * ( threehalfs - ( x2 * y * y ) );
	y = y * ( threehalfs - ( x2 * y * y ) );
//	y = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}