/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "view.h"
#include "raymath.h"
#include "nmath.h"
#include <time.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 720
#define SHOW_VECT

/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *		to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * @return The ISO date (a raylib string)
 */
const char* getISODate(float timestamp)
{
	// Timestamp epoch: 1/1/2022
	struct tm unichEpochTM = {0, 0, 0, 1, 0, 122};

	// Convert timestamp to UNIX timestamp (number of seconds since 1/1/1970)
	time_t unixEpoch = mktime(&unichEpochTM);
	time_t unixTimestamp = unixEpoch + (time_t)timestamp;

	// Returns ISO date
	struct tm* localTM = localtime(&unixTimestamp);
	return TextFormat("%04d-%02d-%02d",
					1900 + localTM->tm_year, localTM->tm_mon + 1, localTM->tm_mday);
}

/**
 * @brief Constructs an orbital simulation view
 *
 * @param fps Frames per second for the view
 * @return The view
 */
view_t* constructView(int fps)
{
	view_t* view = new view_t();

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "EDA Orbital Simulation");
	SetTargetFPS(fps);
	DisableCursor();

	view->camera.position = {10.0f, 10.0f, 10.0f};
	view->camera.target = {0.0f, 0.0f, 0.0f};
	view->camera.up = {0.0f, 1.0f, 0.0f};
	view->camera.fovy = 45.0f;
	view->camera.projection = CAMERA_PERSPECTIVE;

	return view;
}

/**
 * @brief Destroys an orbital simulation view
 *
 * @param view The view
 */
void destroyView(view_t* view)
{
	CloseWindow();

	delete view;
}

/**
 * @brief Should the view still render?
 *
 * @return Should rendering continue?
 */
bool isViewRendering(view_t* view)
{
	return !WindowShouldClose();
}

/**
 * Renders an orbital simulation
 *
 * @param view The view
 * @param sim The orbital sim
 */
void renderView(view_t* view, OrbitalSim_t* sim)
{
	UpdateCamera(&view->camera, CAMERA_FREE);

	BeginDrawing();

	ClearBackground(BLACK);
	BeginMode3D(view->camera);

	// Fill in your 3D drawing code here:

	//DrawGrid(10, 10.0f);
	Vector3 position;

	#ifdef SHOW_VECT
		Vector3 velocity, acceleration;
	#endif

	for (int i = 0; i < sim->bodyNum; i++) //
	{
		position.x = sim->EphemeridesBody[i].position[X];
		position.y = sim->EphemeridesBody[i].position[Y];
		position.z = sim->EphemeridesBody[i].position[Z];

		DrawSphere(Vector3Scale(position, 1E-11), 0.005F * logf(sim->EphemeridesBody[i].radius), sim->EphemeridesBody[i].color);
		//DrawPoint3D(Vector3Scale(position, 1E-11), sim->EphemeridesBody[i].color);
		
		#ifdef SHOW_VECT
			velocity.x = sim->EphemeridesBody[i].velocity[X];
			velocity.y = sim->EphemeridesBody[i].velocity[Y];
			velocity.z = sim->EphemeridesBody[i].velocity[Z];

			acceleration.x = sim->EphemeridesBody[i].acceleration[X];
			acceleration.y = sim->EphemeridesBody[i].acceleration[Y];
			acceleration.z = sim->EphemeridesBody[i].acceleration[Z];

			DrawLine3D(	Vector3Scale(position, 1E-11),
					Vector3Scale(Vector3Add(position, Vector3Scale(acceleration, 1E14)), 1E-11),
					RED);
			DrawLine3D(	Vector3Scale(position, 1E-11),
					Vector3Scale(Vector3Add(position, Vector3Scale(velocity, 1E7)), 1E-11),
					BLUE);
		#endif
	}

	EndMode3D();

	// Fill in your 2D drawing code here:
	DrawFPS(10,10);
	DrawText(getISODate(clock()),10, 30, 20, RAYWHITE);

	EndDrawing();
}