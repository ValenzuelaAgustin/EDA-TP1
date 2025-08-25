/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows
#define _USE_MATH_DEFINES

#include "OrbitalSim.h"
#include "nmath.h"
#include <stdlib.h>
#include <math.h>

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F
#define ASTEROIDS_AMMOUNT 500

/**
 * @brief Gets a uniform random value in a range
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return The random value
 */
float getRandomFloat(float min, float max)
{
	return min + (max - min) * rand() / (float)RAND_MAX;
}

/**
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
void configureAsteroid(EphemeridesBody_t* body, float centerMass)
{
	// Logit distribution
	float x = getRandomFloat(0, 1);
	float l = logf(x) - logf(1 - x) + 1;

	// https://mathworld.wolfram.com/DiskPointPicking.html
	float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabsf(l));
	float phi = getRandomFloat(0, 2.0F * (float)M_PI);

	// Surprise!
	// phi = 0;

	// https://en.wikipedia.org/wiki/Circular_orbit#Velocity
	float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
	float vy = getRandomFloat(-1E2F, 1E2F);

	// Fill in with your own fields:
	body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
	body->radius = 2E3F; // Typical asteroid radius: 2km
	body->color = GRAY;
	body->position[X] = r * cosf(phi);
	body->position[Y] = 0;
	body->position[Z] = r * sinf(phi);

	body->velocity[X] = -v * sinf(phi);
	body->velocity[Y] = vy;
	body->velocity[Z] = v * cosf(phi);
}

/**
 * @brief Constructs an orbital simulation
 *
 * @param float The time step
 * @return The orbital simulation
 */
OrbitalSim_t* constructOrbitalSim(double timeStep)
{
	OrbitalSim_t* ptr = new OrbitalSim_t;
	if (!ptr)
		return NULL;

	ptr->bodyNum = SOLARSYSTEM_BODYNUM + ASTEROIDS_AMMOUNT;
	ptr->EphemeridesBody = (EphemeridesBody_t*)malloc(sizeof(EphemeridesBody_t) * ptr->bodyNum);
	if (!ptr->EphemeridesBody)
	{
		delete ptr;
		return NULL;
	}

	ptr->dt = timeStep;
	int i;
	for (i = 0; i < SOLARSYSTEM_BODYNUM; i++)
	{
		ptr->EphemeridesBody[i] = solarSystem[i];

	}
	for (i = SOLARSYSTEM_BODYNUM; i < ptr->bodyNum; i++)
	{
		configureAsteroid(ptr->EphemeridesBody + i, solarSystem[0].mass);
	}

	return ptr; // This should return your orbital sim
}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim_t* sim)
{
	delete sim;
}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim_t* sim)
{
	//if (!sim || !sim->EphemeridesBody || sim->bodyNum < 1 || sim->dt <= 0)
	//	return;

	int i, j;
	double acceleration[3];
	double inverse_distance_cubed;

	for (i = 0; i < sim->bodyNum; i++)
	{
		sim->EphemeridesBody[i].acceleration[X] = 0;
		sim->EphemeridesBody[i].acceleration[Y] = 0;
		sim->EphemeridesBody[i].acceleration[Z] = 0;
	}

	for (i = 0; i < sim->bodyNum - 1; i++)
	{
		for (j = i + 1; j < sim->bodyNum; j++)
		{
			acceleration[X] = sim->EphemeridesBody[j].position[X] - sim->EphemeridesBody[i].position[X];
			acceleration[Y] = sim->EphemeridesBody[j].position[Y] - sim->EphemeridesBody[i].position[Y];
			acceleration[Z] = sim->EphemeridesBody[j].position[Z] - sim->EphemeridesBody[i].position[Z];

			inverse_distance_cubed = Q_rsqrt(DOT_PRODUCT(acceleration, acceleration));
			inverse_distance_cubed = inverse_distance_cubed * inverse_distance_cubed * inverse_distance_cubed;
			inverse_distance_cubed *= GRAVITATIONAL_CONSTANT;

			acceleration[X] *= inverse_distance_cubed;
			acceleration[Y] *= inverse_distance_cubed;
			acceleration[Z] *= inverse_distance_cubed;

			sim->EphemeridesBody[i].acceleration[X] += sim->EphemeridesBody[j].mass * acceleration[X];
			sim->EphemeridesBody[i].acceleration[Y] += sim->EphemeridesBody[j].mass * acceleration[Y];
			sim->EphemeridesBody[i].acceleration[Z] += sim->EphemeridesBody[j].mass * acceleration[Z];

			sim->EphemeridesBody[j].acceleration[X] -= sim->EphemeridesBody[i].mass * acceleration[X];
			sim->EphemeridesBody[j].acceleration[Y] -= sim->EphemeridesBody[i].mass * acceleration[Y];
			sim->EphemeridesBody[j].acceleration[Z] -= sim->EphemeridesBody[i].mass * acceleration[Z];
		}
	}

	for (i = 0; i < sim->bodyNum; i++)
	{
		sim->EphemeridesBody[i].velocity[X] += sim->EphemeridesBody[i].acceleration[X] * sim->dt;
		sim->EphemeridesBody[i].velocity[Y] += sim->EphemeridesBody[i].acceleration[Y] * sim->dt;
		sim->EphemeridesBody[i].velocity[Z] += sim->EphemeridesBody[i].acceleration[Z] * sim->dt;

		sim->EphemeridesBody[i].position[X] += sim->EphemeridesBody[i].velocity[X] * sim->dt;
		sim->EphemeridesBody[i].position[Y] += sim->EphemeridesBody[i].velocity[Y] * sim->dt;
		sim->EphemeridesBody[i].position[Z] += sim->EphemeridesBody[i].velocity[Z] * sim->dt;
	}
}