/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 * 
 * @author Sofia Capiel
 * @author Agustin Tomas Valenzuela
 * @author Francisco Alonso Paredes
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows
#define _USE_MATH_DEFINES

#include "orbitalSim.h"
#include "vector3D.h"
#include "keyBinds.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define ASTEROIDS_MEAN_RADIUS 4E11F

// Spaceship defines
#define SPACESHIP_ACCELERATION 1E-3

/**
 * Private function definitions.
 */

/**
 * @brief Gets a uniform random value in a range.
 *
 * @param min Minimum value.
 * @param max Maximum value.
 *
 * @return The random value.
 */
static float getRandomFloat(float min, float max);

/**
 * @brief Configures an asteroid.
 *
 * @param body An orbital body.
 * @param centerMass The mass of the most massive object in the star system.
 */
static void configureAsteroid(Body_t* body, float centerMass, int easter_egg);

/**
 * @brief Sets PlanetarySystem, Asteroids and SpaceShip accelerations to 0
 *		Must be called before updateAccelerations.
 *
 * @param sim Pointer to the simulation.
 */
static inline void initializeAccelerations(OrbitalSim_t* sim);

/**
 * @brief Calculates the accelerations between two bodies.
 *
 * @param body0 First body.
 * @param body1 Second body.
 */
static inline void calculateAccelerations(Body_t* body0, Body_t* body1);

/**
 * @brief Calculates the acceleration of one body
 * @param body0 First body.
 * @param body1 Second body.
 */
static inline void calculateAccelerationsOneWay(Body_t* body0, Body_t* body1);

/**
 * @brief Calculates the acceleration for every body in the simulation.
 *
 * @param sim Pointer to the simulation.
 */
static inline void updateAccelerations(OrbitalSim_t* sim);

/**
 * @brief Calculates the new speed and position for a given body.
 *
 * @param body Pointer to the body.
 * @param dt Time step used to calculate discrete integrals.
 */
static inline void calculateSpeedAndPosition(Body_t* body, double dt);

/**
 * @brief Calculates the speed and position for every body in the simulation.
 *
 * @param sim Pointer to the simulation.
 */
static inline void updateSpeedsAndPositions(OrbitalSim_t* sim);

/**
 * @brief Add the accelerations produced by the spaceship's engines.
 *
 * @param sim Pointer to the simulation.
 */
static inline void updateSpaceShipUserInputs(OrbitalSim_t* sim);

/**
 * @brief Removes a body of the simulation.
 * @param sim Pointer to the simulation.
 */
static inline void removeBody(OrbitalSim_t* sim);

/**
 * Public function definitions.
 */

OrbitalSim_t* constructOrbitalSim(unsigned int asteroidsNum, int easter_egg, int System, int spawnBlackHole)
{
	OrbitalSim_t* sim = new OrbitalSim_t;
	if (!sim)
		return NULL;

	sim->bodyNum = (System) ? ALPHACENTAURISYSTEM_BODYNUM : SOLARSYSTEM_BODYNUM;
	sim->asteroidsNum = asteroidsNum;
	sim->PlanetarySystem = (System) ? alphaCentauriSystem : solarSystem;
	sim->Asteroids = (Body_t*) ((sim->asteroidsNum) ? (malloc(sizeof(Body_t) * sim->asteroidsNum)) : NULL);

	if (sim->asteroidsNum && !sim->Asteroids)
	{
		delete sim;
		return NULL;
	}

	sim->dt = 0.0;
	sim->time_elapsed = 0.0;

	for (unsigned int i = 0; i < sim->asteroidsNum; i++)
	{
		configureAsteroid(sim->Asteroids + i, sim->PlanetarySystem[0].body.mass_GC, easter_egg);
	}

	if(spawnBlackHole)
		sim->BlackHole = BlackHole;
	else
		sim->BlackHole = BlackHole_t{};

	configureAsteroid(&sim->Spaceship.body, sim->PlanetarySystem[0].body.mass_GC, 0);
	sim->Spaceship.color = GREEN;
	sim->Spaceship.radius = 120;
	sim->Spaceship.body.mass_GC = 5E6 * GRAVITATIONAL_CONSTANT;

	return sim;
}

void destroyOrbitalSim(OrbitalSim_t* sim)
{
	if (!sim)
		return;
	if (sim->Asteroids)
		free(sim->Asteroids);
	delete sim;
}

void updateOrbitalSim(OrbitalSim_t* sim, int spawnBH)
{
	sim->time_elapsed += sim->dt;
	initializeAccelerations(sim);
	updateSpaceShipUserInputs(sim);

	updateAccelerations(sim);
	updateSpeedsAndPositions(sim);
	if(spawnBH)
		removeBody(sim);
}

static float getRandomFloat(float min, float max)
{
	return min + (max - min) * rand() / (float)RAND_MAX;
}

static void configureAsteroid(Body_t* body, float centerMass, int easter_egg)
{
	// Logit distribution
	float x = getRandomFloat(0, 1);
	float l = logf(x) - logf(1 - x) + 1;

	// https://mathworld.wolfram.com/DiskPointPicking.html
	float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabsf(l));
	float phi = getRandomFloat(0, 2.0F * (float)M_PI);

	// Surprise!
	if (easter_egg)
		phi = 0;

	// https://en.wikipedia.org/wiki/Circular_orbit#Velocity
	float v = sqrtf(centerMass / r) * getRandomFloat(0.6F, 1.2F);
	float vy = getRandomFloat(-1E2F, 1E2F);

	// Fill in with your own fields:
	body->mass_GC = 1E12 * GRAVITATIONAL_CONSTANT;  // Typical asteroid weight: 1 billion tons
	body->position.x = r * cosf(phi);
	body->position.y = 0;
	body->position.z = r * sinf(phi);

	body->velocity.x = -v * sinf(phi);
	body->velocity.y = vy;
	body->velocity.z = v * cosf(phi);
}

static inline void initializeAccelerations(OrbitalSim_t* sim)
{
	unsigned int i;

	for (i = 0; i < sim->bodyNum; i++)
	{
		sim->PlanetarySystem[i].body.acceleration.x = 0.0;
		sim->PlanetarySystem[i].body.acceleration.y = 0.0;
		sim->PlanetarySystem[i].body.acceleration.z = 0.0;
	}
	for (i = 0; i < sim->asteroidsNum; i++)
	{
		sim->Asteroids[i].acceleration.x = 0.0;
		sim->Asteroids[i].acceleration.y = 0.0;
		sim->Asteroids[i].acceleration.z = 0.0;
	}

	sim->Spaceship.body.acceleration.x = 0.0;
	sim->Spaceship.body.acceleration.y = 0.0;
	sim->Spaceship.body.acceleration.z = 0.0;

	sim->BlackHole.body.acceleration.x = 0.0;
	sim->BlackHole.body.acceleration.y = 0.0;
	sim->BlackHole.body.acceleration.z = 0.0;
}

static inline void calculateAccelerations(Body_t* body0, Body_t* body1)
{
	vector3D_t acceleration;
	double inverse_distance_cubed;

	acceleration.x = body1->position.x - body0->position.x;
	acceleration.y = body1->position.y - body0->position.y;
	acceleration.z = body1->position.z - body0->position.z;

	inverse_distance_cubed = 1 / sqrt(DOT_PRODUCT(acceleration, acceleration));
	inverse_distance_cubed = inverse_distance_cubed * inverse_distance_cubed * inverse_distance_cubed;

	acceleration.x *= inverse_distance_cubed;
	acceleration.y *= inverse_distance_cubed;
	acceleration.z *= inverse_distance_cubed;

	body0->acceleration.x += body1->mass_GC * acceleration.x;
	body0->acceleration.y += body1->mass_GC * acceleration.y;
	body0->acceleration.z += body1->mass_GC * acceleration.z;

	body1->acceleration.x -= body0->mass_GC * acceleration.x;
	body1->acceleration.y -= body0->mass_GC * acceleration.y;
	body1->acceleration.z -= body0->mass_GC * acceleration.z;
}

static inline void calculateAccelerationsOneWay(Body_t* body0, Body_t* body1)
{
    vector3D_t acceleration;
    double inverse_distance_cubed;

    acceleration.x = body1->position.x - body0->position.x;
    acceleration.y = body1->position.y - body0->position.y;
    acceleration.z = body1->position.z - body0->position.z;

    inverse_distance_cubed = 1 / sqrt(DOT_PRODUCT(acceleration, acceleration));
    inverse_distance_cubed = inverse_distance_cubed * inverse_distance_cubed * inverse_distance_cubed;

    acceleration.x *= inverse_distance_cubed;
    acceleration.y *= inverse_distance_cubed;
    acceleration.z *= inverse_distance_cubed;

    body0->acceleration.x += body1->mass_GC * acceleration.x;
    body0->acceleration.y += body1->mass_GC * acceleration.y;
    body0->acceleration.z += body1->mass_GC * acceleration.z;
}

static inline void updateAccelerations(OrbitalSim_t* sim)
{
	unsigned int i, j;

	for (i = 0; i < sim->bodyNum; i++)
	{
		for (j = 0; j < sim->asteroidsNum; j++)
		{
			calculateAccelerations(&sim->PlanetarySystem[i].body, sim->Asteroids + j);
		}
		for (j = i + 1; j < sim->bodyNum; j++)
		{
			calculateAccelerations(&sim->PlanetarySystem[i].body, &sim->PlanetarySystem[j].body);
		}
		calculateAccelerations(&sim->PlanetarySystem[i].body, &sim->Spaceship.body);
		calculateAccelerationsOneWay(&sim->PlanetarySystem[i].body, &sim->BlackHole.body);
	}
	for (j = 0; j < sim->asteroidsNum; j++)
    {
        calculateAccelerationsOneWay(sim->Asteroids + j, &sim->BlackHole.body);
    }
}

static inline void calculateSpeedAndPosition(Body_t* body, double dt)
{
	body->velocity.x += body->acceleration.x * dt;
	body->velocity.y += body->acceleration.y * dt;
	body->velocity.z += body->acceleration.z * dt;

	body->position.x += body->velocity.x * dt;
	body->position.y += body->velocity.y * dt;
	body->position.z += body->velocity.z * dt;
}

static inline void updateSpeedsAndPositions(OrbitalSim_t* sim)
{
	unsigned int i;

	for (i = 0; i < sim->bodyNum; i++)
	{
		calculateSpeedAndPosition(&sim->PlanetarySystem[i].body, sim->dt);
	}
	for (i = 0; i < sim->asteroidsNum; i++)
	{
		calculateSpeedAndPosition(sim->Asteroids + i, sim->dt);
	}
	calculateSpeedAndPosition(&sim->Spaceship.body, sim->dt);
	calculateSpeedAndPosition(&sim->BlackHole.body, sim->dt);
}

static inline void updateSpaceShipUserInputs(OrbitalSim_t* sim)
{
	double* acceleration = (double*) &sim->Spaceship.body.acceleration;

	for (int i = 0, axis = 0; i < 6; i++, axis = i % 3)
	{
		// i = 0,1,2 Para los ejes en sentido positivo (teclas U, I y O)
		// i = 3,4,5 Para los ejes en sentido negativo (teclas J, K y L)
		if (movement_key_is_down[i])
		{
			acceleration[axis] += (i < 3) ? SPACESHIP_ACCELERATION : -SPACESHIP_ACCELERATION;
		}
	}
}

static inline void removeBody (OrbitalSim_t* sim)
{
	vector3D_t diff;
	unsigned int i, j;
	double distance_squared;
	double absorbRadius_squared = sim->BlackHole.absorbRadius * sim->BlackHole.absorbRadius;

	for(i = 0; i < sim->bodyNum; i++)
	{
		diff.x = sim->PlanetarySystem[i].body.position.x - sim->BlackHole.body.position.x;
		diff.y = sim->PlanetarySystem[i].body.position.y - sim->BlackHole.body.position.y;
		diff.z = sim->PlanetarySystem[i].body.position.z - sim->BlackHole.body.position.z;

		distance_squared = DOT_PRODUCT(diff, diff);

		if(distance_squared > absorbRadius_squared)
			continue;

		for(j = i; j < sim->bodyNum - 1; j++)
		{
			sim->PlanetarySystem[j] = sim->PlanetarySystem[j+1];
		}
		sim->bodyNum--;
		i--;
	}

	for(i = 0; i < sim->asteroidsNum; i++)
	{
		diff.x = sim->Asteroids[i].position.x - sim->BlackHole.body.position.x;
		diff.y = sim->Asteroids[i].position.y - sim->BlackHole.body.position.y;
		diff.z = sim->Asteroids[i].position.z - sim->BlackHole.body.position.z;

		distance_squared = DOT_PRODUCT(diff, diff);

		if(distance_squared > absorbRadius_squared)
			continue;

		for(j = i; j < sim->asteroidsNum - 1; j++)
		{
			sim->Asteroids[j] = sim->Asteroids[j+1];
		}
		sim->asteroidsNum--;
		i--;
	}
	
}