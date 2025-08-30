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

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
//#define SHOW_VECT

static void drawBody(EphemeridesBody_t* body);

/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *		to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * @return The ISO date (a raylib string)
 */
static const char* getISODate(float timestamp)
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

void destroyView(view_t* view)
{
	CloseWindow();

	delete view;
}

bool isViewRendering(view_t* view)
{
	return !WindowShouldClose();
}

static void drawBody(EphemeridesBody_t* body)
{
	Vector3 position;
	#ifdef SHOW_VECT
		Vector3 velocity, acceleration;
	#endif

	position.x = body->position[X];
	position.y = body->position[Y];
	position.z = body->position[Z];

	DrawSphereEx(Vector3Scale(position, 1E-11), 0.005F * logf(body->radius), 5, 7, body->color);
	//DrawPoint3D(Vector3Scale(position, 1E-11), body->color);

	#ifdef SHOW_VECT
		velocity.x = body->velocity[X];
		velocity.y = body->velocity[Y];
		velocity.z = body->velocity[Z];

		acceleration.x = body->acceleration[X];
		acceleration.y = body->acceleration[Y];
		acceleration.z = body->acceleration[Z];

		DrawLine3D(	Vector3Scale(position, 1E-11),
				Vector3Scale(Vector3Add(position, Vector3Scale(acceleration, 1E14)), 1E-11),
				RED);
		DrawLine3D(	Vector3Scale(position, 1E-11),
				Vector3Scale(Vector3Add(position, Vector3Scale(velocity, 1E7)), 1E-11),
				BLUE);
	#endif
}

void renderView(view_t* view, OrbitalSim_t* sim)
{
	UpdateCamera(&view->camera, CAMERA_FREE);

	BeginDrawing();

	ClearBackground(BLACK);
	BeginMode3D(view->camera);

	// Fill in your 3D drawing code here:

	//DrawGrid(10, 10.0f);

	for (int i = 0; i < sim->bodyNum + sim->asteroidsNum; i++) 
	{
		drawBody(sim->EphemeridesBody + i);
	}

	EndMode3D();

	// Fill in your 2D drawing code here:
	DrawFPS(10,10);
	DrawText(getISODate(clock()),10, 30, 20, RAYWHITE);

	EndDrawing();
}