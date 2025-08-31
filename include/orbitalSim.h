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
	double simulationSpeed;		// In simulated seconds per real second
	double dt;			// In seconds ( has to be updated with the frametime to match the simulation speed )
	EphemeridesBody_t* EphemeridesBody;
	EphemeridesBody_t* Asteroids;
	EphemeridesBody_t spaceship;
	unsigned int bodyNum;
	unsigned int asteroidsNum;
} OrbitalSim_t;

/**
 * @brief Constructs an orbital simulation
 *
 * @param float The time step
 * @return The orbital simulation
 */
OrbitalSim_t* constructOrbitalSim(double simulationSpeed, unsigned int asteroidsNum);

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim_t* sim);

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim_t* sim);

#endif