/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 * 
 * @author Sofia Capiel
 * @author Agustin Tomas Valenzuela
 * @author Francisco Alonso Paredes
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
 * @brief Constructs an orbital simulation view.
 *
 * @param fps Frames per second for the view
 * @param fullscreen Sets the window to fullscreen.
 * @param width Sets a value to the window's width.
 * @param height Sets a value to the window's height.
 * @param show_velocity_vectors Activates the velocity vectors.
 * @param show_acceleration_vectors Activates the acceleration vectors.
 *
 * @return The view.
 */
view_t* constructView(int fps, int fullscreen, int width, int height, int show_velocity_vectors, int show_acceleration_vectors);

/**
 * @brief Destroys an orbital simulation view.
 *
 * @param view The view.
 */
void destroyView(view_t* view);

/**
 * @brief Checks if the view should still render.
 *
 * @param view Pointer to the view object.
 *
 * @return Returns 1 if the rendering should continue, in the opposite case returns 0.
 */
bool isViewRendering(view_t* view);

/**
 * @brief Renders an orbital simulation.
 *
 * @param view The view.
 * @param sim The orbital sim.
 */
void renderView(view_t* view, OrbitalSim_t* sim);

#endif