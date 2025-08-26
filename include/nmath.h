#ifndef NMATH_H
#define NMATH_H

#define DOT_PRODUCT(v0, v1) ( (v0)[X] * (v1)[X] + (v0)[Y] * (v1)[Y] + (v0)[Z] * (v1)[Z] )

#ifdef __cplusplus
extern "C" {
#endif

double Q_rsqrt(double number);

#ifdef __cplusplus
}
#endif

enum
{
	X,
	Y,
	Z
};

#endif