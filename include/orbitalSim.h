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
	double dt;
	EphemeridesBody_t* EphemeridesBody;
	int bodyNum;
} OrbitalSim_t;

OrbitalSim_t* constructOrbitalSim(double timeStep);
void destroyOrbitalSim(OrbitalSim_t* sim);
void updateOrbitalSim(OrbitalSim_t* sim);

#endif