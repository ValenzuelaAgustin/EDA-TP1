/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "launchOptions.h"
#include "orbitalSim.h"
#include "view.h"

#define SECONDS_PER_DAY 86400

// #define WINDOW_WIDTH 2560
// #define WINDOW_HEIGHT 1600

// +fps_max 0 -fullscreen -w 2560 -h 1600 -asteroids_ammount 500 -show_velocity -show_acceleration

int main(int argc, char* argv[])
{
	int launchOptionsValues[launchOptionsAmmount];
	double timeMultiplier = 1 * SECONDS_PER_DAY;	// Simulation speed: 100 days per simulation second
	double timeStep = timeMultiplier;

	searchLaunchOptions(argc, argv, launchOptionsValues);
	OrbitalSim_t* sim = constructOrbitalSim(timeStep, launchOptionsValues[ASTEROIDS_AMMOUNT]);
	view_t* view = constructView(	launchOptionsValues[FPS_MAX],
					launchOptionsValues[FULLSCREEN],
					launchOptionsValues[WIDTH],
					launchOptionsValues[HEIGHT]);

	while (isViewRendering(view))
	{
		updateOrbitalSim(sim);
		renderView(view, sim);
	}

	destroyView(view);
	destroyOrbitalSim(sim);

	return 0;
}