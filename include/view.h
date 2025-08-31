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

#define FULLSCREEN_KEY KEY_F11
#define TOGGLE_EBODIES_RENDER_MODE KEY_F7
#define TOGGLE_ASTEROIDS_RENDER_MODE KEY_F8
#define TOGGLE_SHOW_VELOCITY KEY_F9
#define TOGGLE_SHOW_ACCELERATION KEY_F10

/**
 * The view data
 */
typedef struct
{
	Camera3D camera;
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
void renderView(view_t* view, OrbitalSim_t* sim);

#endif