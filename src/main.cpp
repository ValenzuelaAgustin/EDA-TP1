/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "orbitalSim.h"
#include "view.h"

#define SECONDS_PER_DAY 86400
#define ASTEROIDS_AMMOUNT 10000

int main()
{
	int fps = 60;					// Frames per second
	double timeMultiplier = 50 * SECONDS_PER_DAY;	// Simulation speed: 100 days per simulation second
	double timeStep = timeMultiplier / fps;

	OrbitalSim_t* sim = constructOrbitalSim(timeStep, ASTEROIDS_AMMOUNT);
	view_t* view = constructView(fps);

	while (isViewRendering(view))
	{
		updateOrbitalSim(sim);
		renderView(view, sim);
	}

	destroyView(view);
	destroyOrbitalSim(sim);

	return 0;
}