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
#include "keyBinds.h"
#include <stdlib.h>
#include <math.h>

#define ASTEROIDS_MEAN_RADIUS 4E11F

// Spaceship defines
#define SPACESHIP_ACCELERATION 1E-3

/**
 * @brief Gets a uniform random value in a range
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return The random value
 */
static float getRandomFloat(float min, float max);

/**
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
static void configureAsteroid(Body_t* body, float centerMass, int easter_egg);

/**
 * @brief Sets PlanetarySystem, Asteroids and SpaceShip accelerations to 0
 * @param sim Pointer to the simulation
 */
static inline void initializeAccelerations(OrbitalSim_t* sim);

/**
 * @brief Calculates the accelerations between two bodies
 * @param body0
 * @param body1
 */
static inline void calculateAccelerations(Body_t* body0, Body_t* body1);

/**
 * @brief 
 *
 * @param sim 
 */
static inline void updateAccelerations(OrbitalSim_t* sim);

/**
 * @brief Calculates the new speed and position for a given body
 * @param body pointer to the body
 * @param dt time step used to calculate discrete integrals
 */
static inline void calculateSpeedAndPosition(Body_t* body, double dt);

/**
 * @brief 
 *
 * @param sim
 */
static inline void updateSpeedsAndPositions(OrbitalSim_t* sim);

/**
 * @brief 
 * @param sim
 */
static inline void updateSpaceShipUserInputs(OrbitalSim_t* sim);


OrbitalSim_t* constructOrbitalSim(unsigned int asteroidsNum, int easter_egg, int System)
{
	OrbitalSim_t* ptr = new OrbitalSim_t;
	if (!ptr)
		return NULL;

	ptr->bodyNum = (System) ? ALPHACENTAURISYSTEM_BODYNUM : SOLARSYSTEM_BODYNUM;
	ptr->asteroidsNum = asteroidsNum;
	ptr->PlanetarySystem = (EphemeridesBody_t*)malloc(sizeof(EphemeridesBody_t) * ptr->bodyNum + sizeof(Body_t) * ptr->asteroidsNum);
	ptr->Asteroids = (Body_t*) ((ptr->asteroidsNum) ? (ptr->PlanetarySystem + ptr->bodyNum) : NULL);

	if (!ptr->PlanetarySystem)
	{
		free(ptr->PlanetarySystem);
		delete ptr;
		return NULL;
	}

	ptr->dt = 0.0;
	ptr->time_elapsed = 0.0;

	unsigned int i;
	for (i = 0; i < ptr->bodyNum; i++)
	{
		ptr->PlanetarySystem[i] = (System) ? alphaCentauriSystem[i] : solarSystem[i];
	}
	for (i = 0; i < ptr->asteroidsNum; i++)
	{
		configureAsteroid(ptr->Asteroids + i, ptr->PlanetarySystem[SOL].body.mass_GC, easter_egg);
	}

	configureAsteroid(&ptr->Spaceship.body, ptr->PlanetarySystem[SOL].body.mass_GC, 0);
	ptr->Spaceship.color = GREEN;
	ptr->Spaceship.radius = 120;
	ptr->Spaceship.body.mass_GC = 5E6 * GRAVITATIONAL_CONSTANT;

	return ptr; // This should return your orbital sim
}

void destroyOrbitalSim(OrbitalSim_t* sim)
{
	if (!sim)
		return;
	if (sim->PlanetarySystem)
		free(sim->PlanetarySystem);
	delete sim;
}

void updateOrbitalSim(OrbitalSim_t* sim)
{
	//if (!sim || !sim->EphemeridesBody || sim->bodyNum < 1 || sim->dt <= 0)
	//	return;

	sim->time_elapsed += sim->dt;
	initializeAccelerations(sim);
	updateSpaceShipUserInputs(sim);

	updateAccelerations(sim);
	updateSpeedsAndPositions(sim);
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
}

static inline void calculateAccelerations(Body_t* body0, Body_t* body1)
{
	vector3D_t acceleration;
	double inverse_distance_cubed;

	acceleration.x = body1->position.x - body0->position.x;
	acceleration.y = body1->position.y - body0->position.y;
	acceleration.z = body1->position.z - body0->position.z;

	inverse_distance_cubed = 1 / sqrt(DOT_PRODUCT(acceleration, acceleration));
//	inverse_distance_cubed = Q_rsqrt(DOT_PRODUCT(acceleration, acceleration));
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
}

static inline void updateSpaceShipUserInputs(OrbitalSim_t* sim)
{
	int i, axis;
	double* acceleration = (double*) &sim->Spaceship.body.acceleration;

	for (i = 0, axis = 0; i < 6; i++, axis = i % 3)
	{
		// i = 0,1,2 Para los ejes en sentido positivo (teclas U, I y O)
		// i = 3,4,5 Para los ejes en sentido negativo (teclas J, K y L)
		if (movement_key_is_down[i])
		{
			acceleration[axis] += (i < 3) ? SPACESHIP_ACCELERATION : -SPACESHIP_ACCELERATION;
		}
	}
}