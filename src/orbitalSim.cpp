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
/*void configureAsteroid(OrbitalBody* body, float centerMass)
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
	// body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
	// body->radius = 2E3F; // Typical asteroid radius: 2km
	// body->color = GRAY;
	// body->position = {r * cosf(phi), 0, r * sinf(phi)};
	// body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
}*/

/**
 * @brief Constructs an orbital simulation
 *
 * @param float The time step
 * @return The orbital simulation
 */
OrbitalSim_t* constructOrbitalSim(float timeStep)
{
	OrbitalSim_t* ptr = new OrbitalSim_t;
	if (!ptr)
		return NULL;

	ptr->dt = timeStep;
	ptr->bodyNum = SOLARSYSTEM_BODYNUM;
	ptr->EphemeridesBody = solarSystem;

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
	Vector3 acceleration;
	float inverse_distance_cubed;

	for (i = 0; i < sim->bodyNum; i++)
	{
		sim->EphemeridesBody[i].acceleration.x = 0;
		sim->EphemeridesBody[i].acceleration.y = 0;
		sim->EphemeridesBody[i].acceleration.z = 0;
	}

	for (i = 0; i < sim->bodyNum - 1; i++)
	{
		for (j = i + 1; j < sim->bodyNum; j++)
		{
			acceleration.x = sim->EphemeridesBody[j].position.x - sim->EphemeridesBody[i].position.x;
			acceleration.y = sim->EphemeridesBody[j].position.y - sim->EphemeridesBody[i].position.y;
			acceleration.z = sim->EphemeridesBody[j].position.z - sim->EphemeridesBody[i].position.z;

			inverse_distance_cubed = Q_rsqrt(DOT_PRODUCT(acceleration, acceleration));
			inverse_distance_cubed = inverse_distance_cubed * inverse_distance_cubed * inverse_distance_cubed;
			inverse_distance_cubed *= GRAVITATIONAL_CONSTANT;

			acceleration.x *= inverse_distance_cubed;
			acceleration.y *= inverse_distance_cubed;
			acceleration.z *= inverse_distance_cubed;

			sim->EphemeridesBody[i].acceleration.x += sim->EphemeridesBody[j].mass * acceleration.x;
			sim->EphemeridesBody[i].acceleration.y += sim->EphemeridesBody[j].mass * acceleration.y;
			sim->EphemeridesBody[i].acceleration.z += sim->EphemeridesBody[j].mass * acceleration.z;

			sim->EphemeridesBody[j].acceleration.x += sim->EphemeridesBody[i].mass * acceleration.x;
			sim->EphemeridesBody[j].acceleration.y += sim->EphemeridesBody[i].mass * acceleration.y;
			sim->EphemeridesBody[j].acceleration.z += sim->EphemeridesBody[i].mass * acceleration.z;
		}
	}

	for (i = 0; i < sim->bodyNum; i++)
	{
		sim->EphemeridesBody[i].velocity.x += sim->EphemeridesBody[i].acceleration.x * sim->dt;
		sim->EphemeridesBody[i].velocity.y += sim->EphemeridesBody[i].acceleration.y * sim->dt;
		sim->EphemeridesBody[i].velocity.z += sim->EphemeridesBody[i].acceleration.z * sim->dt;

		sim->EphemeridesBody[i].position.x += sim->EphemeridesBody[i].velocity.x * sim->dt;
		sim->EphemeridesBody[i].position.y += sim->EphemeridesBody[i].velocity.y * sim->dt;
		sim->EphemeridesBody[i].position.z += sim->EphemeridesBody[i].velocity.z * sim->dt;
	}
}