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
	double mass;		// [kg]
	float radius;		// [m]
	Color color;		// Raylib color
	double position[3];	// [m]
	double velocity[3];	// [m/s]
	double acceleration[3];	// [m/s^2]
} EphemeridesBody_t;

extern EphemeridesBody_t solarSystem[];
extern const int SOLARSYSTEM_BODYNUM;
extern EphemeridesBody_t alphaCentauriSystem[];
extern const int ALPHACENTAURISYSTEM_BODYNUM;

#endif