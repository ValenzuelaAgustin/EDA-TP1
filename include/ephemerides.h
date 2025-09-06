/**
 * @brief Ephemerides for orbital simulation
 * @author Marc S. Ressl
 * 
 * @copyright Copyright (c) 2022-2023
 */

#ifndef EPHEMERIDES_H
#define EPHEMERIDES_H

#include "nmath.h"
#include <raylib.h>

#define GRAVITATIONAL_CONSTANT 6.6743E-11	// [N * m^2 / Kg^2]

typedef struct
{
	double mass_GC;			// [m^3 / s^2]
	vector3D_t position;		// [m]
	vector3D_t velocity;		// [m/s]
	vector3D_t acceleration;	// [m/s^2]
} Body_t;

typedef struct
{
//	const char* name;		// Name
	Color color;			// Raylib color
	float radius;			// [m]
	Body_t body;
} EphemeridesBody_t;

/**
 * @brief Solay system ephermerides for 2022-01-01T00:00:00Z
 * 
 * @cite https://ssd.jpl.nasa.gov/horizons/app.html#/
 * @cite https://ssd.jpl.nasa.gov/planets/phys_par.html
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