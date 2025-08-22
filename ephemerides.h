/**
 * @brief Ephemerides for orbital simulation
 * @author Marc S. Ressl
 * 
 * @copyright Copyright (c) 2022-2023
 */

#ifndef EPHEMERIDES_H
#define EPHEMERIDES_H

#include <raylib.h>

typedef struct
{
	const char* name;	// Name
	float mass;		// [kg]
	float radius;		// [m]
	Color color;		// Raylib color
	Vector3 position;	// [m]
	Vector3 velocity;	// [m/s]
	Vector3 acceleration;	// [m/s^2]
} EphemeridesBody;

extern EphemeridesBody solarSystem[];
extern const int SOLARSYSTEM_BODYNUM;
extern EphemeridesBody alphaCentauriSystem[];
extern const int ALPHACENTAURISYSTEM_BODYNUM;

#endif