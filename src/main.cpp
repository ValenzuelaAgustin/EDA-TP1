/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "launchOptions.h"
#include "orbitalSim.h"
#include "view.h"

#define SECONDS_PER_DAY ( 24 * 60 * 60)
#define MAX_FRAME_TIME ( 1.0f / 60.0f )

// #define WINDOW_WIDTH 2560
// #define WINDOW_HEIGHT 1600

/*
+fps_max 0 -fullscreen -w 2560 -h 1600 -asteroids_ammount 500 -show_velocity_vectors -show_acceleration_vectors
*/

int main(int argc, char* argv[])
{
	int launchOptionsValues[launchOptionsAmmount];
	double simulationSpeed = 50 * SECONDS_PER_DAY;	// Simulation speed: 100 days per simulation second
	float frametime;

	searchLaunchOptions(argc, argv, launchOptionsValues);
	OrbitalSim_t* sim = constructOrbitalSim(simulationSpeed, launchOptionsValues[ASTEROIDS_AMMOUNT]);
	view_t* view = constructView(	launchOptionsValues[FPS_MAX],
					launchOptionsValues[FULLSCREEN],
					launchOptionsValues[WIDTH],
					launchOptionsValues[HEIGHT],
					launchOptionsValues[SHOW_VELOCITY_VECTORS],
					launchOptionsValues[SHOW_ACCELERATION_VECTORS]);

	while (isViewRendering(view))
	{
		updateOrbitalSim(sim);
		renderView(view, sim);
		if ((frametime = GetFrameTime()) > MAX_FRAME_TIME)
		{
			frametime = MAX_FRAME_TIME;
		}
		sim->dt = sim->simulationSpeed * frametime;
	}

	destroyView(view);
	destroyOrbitalSim(sim);

	return 0;
}