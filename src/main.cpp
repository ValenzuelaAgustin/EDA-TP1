/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "orbitalSim.h"
#include "view.h"

#define SECONDS_PER_DAY 86400

int main()
{
	int fps = 223;					// Frames per second
	double timeMultiplier = 100 * SECONDS_PER_DAY;	// Simulation speed: 100 days per simulation second
	double timeStep = timeMultiplier / fps;

	OrbitalSim_t* sim = constructOrbitalSim(timeStep);
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