/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "launchOptions.h"
#include "orbitalSim.h"
#include "view.h"
#include <stdio.h>

//#define TEST_UPDATE_ORBITAL_SIM
#ifdef TEST_UPDATE_ORBITAL_SIM
	#include <time.h>
#endif

#define INITIAL_SIM_UPDATES_PER_FRAME 100
#define SECONDS_PER_DAY ( 24 * 60 * 60 )

/*
+fps_target 0 -fullscreen -w 2560 -h 1600 -days_per_simulation_second 10 -asteroids_ammount 500 -show_velocity_vectors -show_acceleration_vectors
*/

double getInitialSimUpdatesPerFrame(OrbitalSim_t* sim, view_t* view, double target_frametime, double PIDC);
double frametime_PID(double target_frametime, double frametime);

int main(int argc, char* argv[])
{
	int i;
	int launchOptionsValues[launchOptionsAmmount];
	int time_direction = 0;
	int prev_time_direction = 0;
	double simulationSpeed;
	double target_frametime;
	double sim_updates_per_frame;
	double PIDC;

	searchLaunchOptions(argc, argv, launchOptionsValues);
	simulationSpeed = launchOptionsValues[DAYS_PER_SIMULATION_SECOND] * SECONDS_PER_DAY;
	solarSystem[JUPITER].body.mass_GC *= (launchOptionsValues[MASSIVE_JUPITER]) ? 1E3 : 1.0;

	OrbitalSim_t* sim = constructOrbitalSim(launchOptionsValues[ASTEROIDS_AMMOUNT], launchOptionsValues[EASTER_EGG]);

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
	printf("\nsim_updates_per_frame = %.0lf\ndt = %.15lf seconds\n", sim_updates_per_frame, sim->dt);

	while (isViewRendering(view))
	{
		for (i = 0; i < sim_updates_per_frame; i++)
			updateOrbitalSim(sim);

		prev_time_direction = time_direction;
		time_direction = renderView(view, sim);
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

double getInitialSimUpdatesPerFrame(OrbitalSim_t* sim, view_t* view, double target_frametime, double PIDC)
{
	double frametime = 0;
	double sim_updates_per_frame = INITIAL_SIM_UPDATES_PER_FRAME;
	int i;
	int end_condition = 0;

	sim->dt = 0;

	while (isViewRendering(view) && end_condition < 100)
	{
		for (i = 0; i < sim_updates_per_frame; i++)
			updateOrbitalSim(sim);
		renderView(view, sim);

		frametime = GetFrameTime();
		sim_updates_per_frame += PIDC * frametime_PID(target_frametime, frametime);
		sim_updates_per_frame = (sim_updates_per_frame > 1) ? sim_updates_per_frame : 1;

		end_condition = (frametime >= 0.9 * target_frametime && frametime <= 1.1 * target_frametime) ? end_condition + 1 : end_condition;
	}

	return sim_updates_per_frame;
}

double frametime_PID(double target_frametime, double frametime)
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