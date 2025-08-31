/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "launchOptions.h"
#include "orbitalSim.h"
#include "view.h"

#define DAYS_PER_SECOND 10

#define INITIAL_SIM_UPDATES_PER_FRAME 100
#define DEFAULT_FPS_TARGET 60
#define SECONDS_PER_DAY ( 24 * 60 * 60 )
#define DEFAULT_FRAME_TIME ( 1.0 / DEFAULT_FPS_TARGET )

/*
+fps_max 0 -fullscreen -w 2560 -h 1600 -asteroids_ammount 500 -show_velocity_vectors -show_acceleration_vectors
*/

double getInitialSimUpdatesPerFrame(OrbitalSim_t* sim, view_t* view, double target_frametime, double PIDC);
double frametime_PID(double target_frametime, double frametime);

int main(int argc, char* argv[])
{
	int launchOptionsValues[launchOptionsAmmount];
	double simulationSpeed = DAYS_PER_SECOND * SECONDS_PER_DAY;	// Simulation speed: 100 days per simulation second

	double target_frametime;
	double frametime;
	int i;
	double sim_updates_per_frame;
	double PIDC;

	searchLaunchOptions(argc, argv, launchOptionsValues);
	OrbitalSim_t* sim = constructOrbitalSim(simulationSpeed, launchOptionsValues[ASTEROIDS_AMMOUNT]);
	view_t* view = constructView(	launchOptionsValues[TARGET_FPS],
					launchOptionsValues[FULLSCREEN],
					launchOptionsValues[WIDTH],
					launchOptionsValues[HEIGHT],
					launchOptionsValues[SHOW_VELOCITY_VECTORS],
					launchOptionsValues[SHOW_ACCELERATION_VECTORS]);

	PIDC = (sim->asteroidsNum == 0) ? 1E4 : 1E4 / sim->asteroidsNum;
	PIDC = (PIDC < 1) ? 1 : PIDC;

	target_frametime = (launchOptionsValues[TARGET_FPS] == 0) ? DEFAULT_FRAME_TIME : (1.0F / launchOptionsValues[TARGET_FPS]);
	sim_updates_per_frame = getInitialSimUpdatesPerFrame(sim, view, target_frametime, PIDC);

	while (isViewRendering(view))
	{
		for (i = 0; i < sim_updates_per_frame; i++)
			updateOrbitalSim(sim);
		renderView(view, sim);

		frametime = GetFrameTime();
		sim_updates_per_frame += PIDC * frametime_PID(target_frametime, frametime);
		sim_updates_per_frame = (sim_updates_per_frame > 1) ? sim_updates_per_frame : 1;
		sim->dt = sim->simulationSpeed * frametime / sim_updates_per_frame;
	}

	destroyView(view);
	destroyOrbitalSim(sim);

	return 0;
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

		end_condition = (frametime >= 0.9 * target_frametime && frametime <= 1.1 * target_frametime) ? end_condition + 1 : 0;
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