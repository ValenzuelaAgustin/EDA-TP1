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
	const char* name;		// Name
	double mass;			// [kg]
	float radius;			// [m]
	Color color;			// Raylib color
	double position[3];		// [m]
	double velocity[3];		// [m/s]
	double acceleration[3];		// [m/s^2]
} EphemeridesBody_t;

/**
 * @brief Solay system ephermerides for 2022-01-01T00:00:00Z
 * 
 * @cite https://ssd.jpl.nasa.gov/horizons/app.html#/
*/
extern EphemeridesBody_t solarSystem[];
extern const unsigned int SOLARSYSTEM_BODYNUM;

enum
{
	SOL,
	MERCURIO,
	VENUS,
	TIERRA,
	MARTE,
	JUPITER,
	SATURNO,
	URANO,
	NEPTUNO
};

/**
 * Alpha Centauri system ephermerides for 2022-01-01T00:00:00Z
 * 
 * @cite https://ssd.jpl.nasa.gov/horizons/app.html#/
*/
extern EphemeridesBody_t alphaCentauriSystem[];
extern const unsigned int ALPHACENTAURISYSTEM_BODYNUM;

enum
{
	ALFA_CENTAURI_A,
	ALFA_CENTAURI_B
};

#endif