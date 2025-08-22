/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIMVIEW_H
#define ORBITALSIMVIEW_H

#include <raylib.h>
#include "OrbitalSim.h"

/**
 * The view data
 */
typedef struct
{
	Camera3D camera;
} view_t;

view_t* constructView(int fps);
void destroyView(view_t* view);

bool isViewRendering(view_t* view);
void renderView(view_t* view, OrbitalSim_t* sim);

#endif