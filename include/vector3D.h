#ifndef VECTOR3D_H
#define VECTOR3D_H

#define DOT_PRODUCT(v0, v1) ( (v0).x * (v1).x + (v0).y * (v1).y + (v0).z * (v1).z )

typedef struct
{
	double x;
	double y;
	double z;
} vector3D_t;

#endif