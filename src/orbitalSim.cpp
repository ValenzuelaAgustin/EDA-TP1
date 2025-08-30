/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows
#define _USE_MATH_DEFINES

#include "orbitalSim.h"
#include "nmath.h"
#include <stdlib.h>
#include <math.h>

#define GRAVITATIONAL_CONSTANT 6.6743E-11
#define ASTEROIDS_MEAN_RADIUS 4E11F

static inline void calculateAccelerations(EphemeridesBody_t* body0, EphemeridesBody_t* body1);

static inline void updateSpeedAndPosition(EphemeridesBody_t* body, double dt);

/**
 * @brief Gets a uniform random value in a range
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return The random value
 */
static float getRandomFloat(float min, float max)
{
	return min + (max - min) * rand() / (float)RAND_MAX;
}

/**
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
static void configureAsteroid(EphemeridesBody_t* body, float centerMass)
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
	float v = sqrtf(centerMass / r) * getRandomFloat(0.6F, 1.2F);
	float vy = getRandomFloat(-1E2F, 1E2F);

	// Fill in with your own fields:
	body->mass = 1E12F * GRAVITATIONAL_CONSTANT;  // Typical asteroid weight: 1 billion tons
	body->radius = 2E3F; // Typical asteroid radius: 2km
	body->color = GRAY;
	body->position[X] = r * cosf(phi);
	body->position[Y] = 0;
	body->position[Z] = r * sinf(phi);

	body->velocity[X] = -v * sinf(phi);
	body->velocity[Y] = vy;
	body->velocity[Z] = v * cosf(phi);
}

OrbitalSim_t* constructOrbitalSim(double simulationSpeed, unsigned int asteroidsNum)
{
	if (simulationSpeed <= 0)
		return NULL;

	OrbitalSim_t* ptr = new OrbitalSim_t;
	if (!ptr)
		return NULL;

	ptr->bodyNum = SOLARSYSTEM_BODYNUM;
	ptr->asteroidsNum = asteroidsNum;
	ptr->EphemeridesBody = (EphemeridesBody_t*)malloc(sizeof(EphemeridesBody_t) * (ptr->bodyNum + ptr->asteroidsNum));
	ptr->Asteroids = (ptr->asteroidsNum) ? ptr->EphemeridesBody + ptr->bodyNum : NULL;

	if (!ptr->EphemeridesBody)
	{
		free(ptr->EphemeridesBody);
		delete ptr;
		return NULL;
	}

	ptr->dt = 0;
	ptr->simulationSpeed = simulationSpeed;

	unsigned int i;
	for (i = 0; i < SOLARSYSTEM_BODYNUM; i++)
	{
		ptr->EphemeridesBody[i] = solarSystem[i];
		ptr->EphemeridesBody[i].mass *= GRAVITATIONAL_CONSTANT;
	}
	for (i = 0; i < ptr->asteroidsNum; i++)
	{
		configureAsteroid(ptr->Asteroids + i, ptr->EphemeridesBody[SOL].mass);
	}

	return ptr; // This should return your orbital sim
}

void destroyOrbitalSim(OrbitalSim_t* sim)
{
	if (!sim)
		return;
	if (sim->EphemeridesBody)
		free(sim->EphemeridesBody);
	delete sim;
}

static inline void calculateAccelerations(EphemeridesBody_t* body0, EphemeridesBody_t* body1)
{
	double acceleration[3];
	double inverse_distance_cubed;

	acceleration[X] = body1->position[X] - body0->position[X];
	acceleration[Y] = body1->position[Y] - body0->position[Y];
	acceleration[Z] = body1->position[Z] - body0->position[Z];

	inverse_distance_cubed = Q_rsqrt(DOT_PRODUCT(acceleration, acceleration));
	inverse_distance_cubed = inverse_distance_cubed * inverse_distance_cubed * inverse_distance_cubed;
	//inverse_distance_cubed *= GRAVITATIONAL_CONSTANT;

	acceleration[X] *= inverse_distance_cubed;
	acceleration[Y] *= inverse_distance_cubed;
	acceleration[Z] *= inverse_distance_cubed;

	body0->acceleration[X] += body1->mass * acceleration[X];
	body0->acceleration[Y] += body1->mass * acceleration[Y];
	body0->acceleration[Z] += body1->mass * acceleration[Z];

	body1->acceleration[X] -= body0->mass * acceleration[X];
	body1->acceleration[Y] -= body0->mass * acceleration[Y];
	body1->acceleration[Z] -= body0->mass * acceleration[Z];
}

static inline void updateSpeedAndPosition(EphemeridesBody_t* body, double dt)
{
	body->velocity[X] += body->acceleration[X] * dt;
	body->velocity[Y] += body->acceleration[Y] * dt;
	body->velocity[Z] += body->acceleration[Z] * dt;

	body->position[X] += body->velocity[X] * dt;
	body->position[Y] += body->velocity[Y] * dt;
	body->position[Z] += body->velocity[Z] * dt;
}

void updateOrbitalSim(OrbitalSim_t* sim)
{
	//if (!sim || !sim->EphemeridesBody || sim->bodyNum < 1 || sim->dt <= 0)
	//	return;

	unsigned int i, j;

	for (i = 0, j = sim->bodyNum + sim->asteroidsNum; i < j; i++)
	{
		sim->EphemeridesBody[i].acceleration[X] = 0;
		sim->EphemeridesBody[i].acceleration[Y] = 0;
		sim->EphemeridesBody[i].acceleration[Z] = 0;
	}

	for (i = 0; i < sim->bodyNum; i++)
	{
		for (j = 0; j < sim->asteroidsNum; j++)
		{
			calculateAccelerations(sim->EphemeridesBody + i, sim->Asteroids + j);
		}
		for (j = i + 1; j < sim->bodyNum; j++)
		{
			calculateAccelerations(sim->EphemeridesBody + i, sim->EphemeridesBody + j);
		}
	}

	for (i = 0, j = sim->bodyNum + sim->asteroidsNum; i < j; i++)
	{
		updateSpeedAndPosition(sim->EphemeridesBody + i, sim->dt);
	}
}