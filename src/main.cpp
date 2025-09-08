/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "launchOptions.h"
#include "orbitalSim.h"
#include "view.h"
#include "controller.h"
#include <stdio.h>

//#define TEST_UPDATE_ORBITAL_SIM
#ifdef TEST_UPDATE_ORBITAL_SIM
	#include <time.h>
#endif

#define INITIAL_SIM_UPDATES_PER_FRAME 100
#define SECONDS_PER_DAY ( 24 * 60 * 60 )

/**
 * @brief 
 *
 * @param sim 
 * @param view 
 * @param target_frametime 
 * @param PIDC 
 *
 * @return 
 */
static int getInitialSimUpdatesPerFrame(OrbitalSim_t* sim, view_t* view, double target_frametime, double PIDC);

/**
 * @brief 
 *
 * @param target_frametime 
 * @param frametime 
 *
 * @return 
 */
static double frametime_PID(double target_frametime, double frametime);

int main(int argc, char* argv[])
{
	int launchOptionsValues[launchOptionsAmmount];
	int time_direction = 0;
	int prev_time_direction = 0;
	int sim_updates_per_frame;
	double simulationSpeed;
	double target_frametime;
	double PIDC;

	searchLaunchOptions(argc, argv, launchOptionsValues);
	simulationSpeed = launchOptionsValues[DAYS_PER_SIMULATION_SECOND] * SECONDS_PER_DAY;
	solarSystem[JUPITER].body.mass_GC *= (launchOptionsValues[MASSIVE_JUPITER]) ? 1E3 : 1.0;

	OrbitalSim_t* sim = constructOrbitalSim(launchOptionsValues[ASTEROIDS_AMMOUNT], launchOptionsValues[EASTER_EGG], launchOptionsValues[SYSTEM]);

#ifndef TEST_UPDATE_ORBITAL_SIM
	view_t* view = constructView(	0,
					launchOptionsValues[FULLSCREEN],
					launchOptionsValues[WIDTH],
					launchOptionsValues[HEIGHT],
					launchOptionsValues[SHOW_VELOCITY_VECTORS],
					launchOptionsValues[SHOW_ACCELERATION_VECTORS]);

	PIDC = (sim->asteroidsNum == 0) ? 1E4 : 1E4 / sim->asteroidsNum;
	PIDC = (PIDC < 1) ? 1 : PIDC;

	target_frametime = 1.0 / launchOptionsValues[TARGET_FPS];
	sim_updates_per_frame = getInitialSimUpdatesPerFrame(sim, view, target_frametime, PIDC);
	sim->dt = simulationSpeed * target_frametime / sim_updates_per_frame;
	printf("\nsim_updates_per_frame = %d\ndt = %.15lf seconds\n", sim_updates_per_frame, sim->dt);

	while (isViewRendering(view))
	{
		updateUserInputs(sim->bodyNum);

		for (int i = 0; i < sim_updates_per_frame; i++)
			updateOrbitalSim(sim);

		renderView(view, sim);
		prev_time_direction = time_direction;
		time_direction = keybinds_values[TOGGLE_REWIND];
		if (time_direction == prev_time_direction)
			continue;

		sim->dt = -sim->dt;
	}

	destroyView(view);
	destroyOrbitalSim(sim);

	return 0;
#else
	#define TEST_TIME (60 * 20)
	time_t t0, t1;
	size_t counter = 0;
	t0 = time(NULL);

	do
	{
		updateOrbitalSim(sim);
		counter++;
		t1 = time(NULL);
	} while (difftime(t1,t0) < TEST_TIME);

	printf("\nUpdates:\t%zu\nTime:\t%lld\n", counter, t1 - t0);
	printf("\nUpdates per second:\t%zu\n",counter / TEST_TIME);
	printf("\nUpdates per frame:\t%zu\n",counter / (TEST_TIME * launchOptionsValues[TARGET_FPS]));

	return 0;
#endif
}

static int getInitialSimUpdatesPerFrame(OrbitalSim_t* sim, view_t* view, double target_frametime, double PIDC)
{
	double sim_updates_per_frame = INITIAL_SIM_UPDATES_PER_FRAME;
	double frametime = 0;

	sim->dt = 0;

	while (isViewRendering(view) && ((frametime < 0.999 * target_frametime) || (frametime > 1.001 * target_frametime)))
	{
		updateUserInputs(sim->bodyNum);

		for (int i = 0; i < sim_updates_per_frame; i++)
			updateOrbitalSim(sim);

		renderView(view, sim);
		frametime = GetFrameTime();
		if (sim_updates_per_frame < 2.0 && frametime > target_frametime)	// if target frametime can not be obtained
			break;

		sim_updates_per_frame += PIDC * frametime_PID(target_frametime, frametime);
		sim_updates_per_frame = (sim_updates_per_frame > 1) ? sim_updates_per_frame : 1;
	}

	return (int)sim_updates_per_frame;
}

static double frametime_PID(double target_frametime, double frametime)
{
	static double PC = 10;
	static double IC = 10;
	static double DC = 10;

	static double I = 0;
	static double prev_dif = 0;

	double dif = target_frametime - frametime;
	double D = dif - prev_dif;

	prev_dif = dif;
	I += dif;

	return dif * PC + I * IC + D * DC;
}