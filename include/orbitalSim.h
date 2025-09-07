/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H
#include "ephemerides.h"

/**
 * @brief Orbital simulation definition
 */
typedef struct
{
	double dt;			// In seconds ( has to be updated with the frametime to match the simulation speed )
	double time_elapsed;		// In seconds
	EphemeridesBody_t* PlanetarySystem;
	Body_t* Asteroids;
	EphemeridesBody_t Spaceship;
	Body_t BlackHole;// IMPLEMENTAR y borrar este comentario
	unsigned int bodyNum;
	unsigned int asteroidsNum;
} OrbitalSim_t;

/**
 * @brief Constructs an orbital simulation
 *
 * @param asteroidsNum 
 * @param easter_egg 
 * @param System 
 *
 * @return The orbital simulation
 */
OrbitalSim_t* constructOrbitalSim(unsigned int asteroidsNum, int easter_egg, int System);

/**
 * @brief Destroys an orbital simulation
 *
 * @param sim 
 */
void destroyOrbitalSim(OrbitalSim_t* sim);

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim_t* sim);

#endif