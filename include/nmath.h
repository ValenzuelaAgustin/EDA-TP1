#ifndef NMATH_H
#define NMATH_H

#define DOT_PRODUCT(v0, v1) ( (v0).x * (v1).x + (v0).y * (v1).y + (v0).z * (v1).z )

#ifdef __cplusplus
extern "C" {
#endif

double Q_rsqrt(double number);

#ifdef __cplusplus
}
#endif

typedef struct
{
	double x;
	double y;
	double z;
} vector3D_t;

enum
{
	X,
	Y,
	Z
};

#endif