/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIMVIEW_H
#define ORBITALSIMVIEW_H

#include "orbitalSim.h"
#include <raylib.h>

/**
 * The view data
 */
typedef struct
{
	Camera3D camera;
	int width, height;
} view_t;

/**
 * @brief Constructs an orbital simulation view
 *
 * @param fps Frames per second for the view
 * @return The view
 */
view_t* constructView(int fps, int fullscreen, int width, int height, int show_velocity_vectors, int show_acceleration_vectors);

/**
 * @brief Destroys an orbital simulation view
 *
 * @param view The view
 */
void destroyView(view_t* view);

/**
 * @brief Should the view still render?
 *
 * @return Should rendering continue?
 */
bool isViewRendering(view_t* view);

/**
 * Renders an orbital simulation
 *
 * @param view The view
 * @param sim The orbital sim
 */
int renderView(view_t* view, OrbitalSim_t* sim);

#endif