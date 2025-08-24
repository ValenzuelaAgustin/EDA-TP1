#include "nmath.h"

/** 
 * @author Greg Walsh
 * @brief Inverse square root algorithm from Quake III engine
 * @cite 
 */
float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = * (long*) &y;
	i = 0x5f3759df - ( i >> 1 );
	y = * (float*) &i;
	y = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}