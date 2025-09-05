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
#include <stdarg.h>
#include <stdio.h>

#define MIN_WIDTH 800
#define MIN_HEIGHT 600
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define WINDOW_TITLE "EDA Orbital Simulation"

#define ASTEROIDS_COLOR GRAY
#define ASTEROIDS_RADIUS 2E3F

// Controls
#define CONTROLS_X_MARGIN 370
#define CONTROLS_COLOR CLITERAL(Color){0, 228, 48, 150} 

enum
{
	EBODIES_RENDER_MODE,
	ASTEROIDS_RENDER_MODE,
	SPACESHIP_RENDER_MODE,
	SHOW_VELOCITY_VECTORS,
	SHOW_ACCELERATION_VECTORS,
	SHOW_CONTROLS
};

enum
{
	QUALITY,
	PERFORMANCE
};

static int EBodies_render_mode = QUALITY;
static int Asteroids_render_mode = PERFORMANCE;
static int Spaceship_render_mode = QUALITY;
static int show_velocity_v = 0;
static int show_acceleration_v = 0;
static int show_controls = 1;
static char buffer[128];

static void drawBody(Body_t* body, float radius, Color color, int render_mode);

/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *		to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * @param started wether the simulation has started
 * @return The ISO date (a raylib string)
 */
static const char* getISODate(float timestamp, int started)
{
	static time_t old_timestamp = 0.0;
	time_t new_timestamp = 0.0;
	if (!started)
	{
		old_timestamp = (time_t)timestamp;
		return TextFormat("0000-00-00");
	}
	new_timestamp = (time_t)timestamp - old_timestamp;
	if(started)
	{
		// Timestamp epoch: 1/1/2022
		struct tm unichEpochTM = {0, 0, 0, 1, 0, 122};

		// Convert timestamp to UNIX timestamp (number of seconds since 1/1/1970)
		time_t unixEpoch = mktime(&unichEpochTM);
		time_t unixTimestamp = unixEpoch + (time_t)new_timestamp;

		// Returns ISO date
		struct tm* localTM = localtime(&unixTimestamp);
		return TextFormat("%04d-%02d-%02d",
						1900 + localTM->tm_year, localTM->tm_mon + 1, localTM->tm_mday);
	}
}

static const char* get_simTime_elapsed (float timestamp, int started)
{
	static float old_timestamp;
	float new_timestamp;

	if(!started)
	{
		old_timestamp = timestamp;
	}
	new_timestamp = timestamp - old_timestamp;
	if(started)
	{
		time_t copy_timestamp = (time_t)new_timestamp;

		struct tm* localTM = localtime(&copy_timestamp);
		return TextFormat("%04d years, %02d months, %02d days", localTM->tm_year - 70, localTM->tm_mon, localTM->tm_mday);
	}
	else
		return TextFormat("0000 years, 00 months, 00 days");
}

char* print_simTime_elapsed (const char* simTime, char buffer [])
{
	sprintf(buffer, "Elapsed Sim Time: %s ", simTime);
	return buffer;
}

view_t* constructView(int fps, int fullscreen, int width, int height, int show_velocity_vectors, int show_acceleration_vectors)
{
	if (width < MIN_WIDTH)
		width = DEFAULT_WINDOW_WIDTH;
	if (height < MIN_HEIGHT)
		height = DEFAULT_WINDOW_HEIGHT;

	view_t* view = new view_t();
	if (!view)
		return NULL;

	InitWindow(width, height, WINDOW_TITLE);
	if (fullscreen)
		ToggleFullscreen();

	show_velocity_v = show_velocity_vectors;
	show_acceleration_v = show_acceleration_vectors;
	SetTargetFPS(fps);

	DisableCursor();
	view->camera.position = {10.0f, 10.0f, 10.0f};
	view->camera.target = {0.0f, 0.0f, 0.0f};
	view->camera.up = {0.0f, 1.0f, 0.0f};
	view->camera.fovy = 45.0f;
	view->camera.projection = CAMERA_PERSPECTIVE;
	view->width = width;
	view->height = height;

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

static void drawBody(Body_t* body, float radius, Color color, int render_mode)
{
	Vector3 position, velocity, acceleration;

	position.x = body->position.x * 1E-11;
	position.y = body->position.y * 1E-11;
	position.z = body->position.z * 1E-11;

	switch (render_mode)
	{
	default:
	case QUALITY:
		DrawSphereEx(position, 0.005F * logf(radius), 5, 7, color);
		break;
	case PERFORMANCE:
		DrawPoint3D(position, color);
		break;
	}

	if (show_velocity_v)
	{
		velocity.x = body->velocity.x * 1E-4 + position.x;
		velocity.y = body->velocity.y * 1E-4 + position.y;
		velocity.z = body->velocity.z * 1E-4 + position.z;

		DrawLine3D(position, velocity, BLUE);
	}
	if (show_acceleration_v)
	{
		acceleration.x = body->acceleration.x * 1E3 + position.x;
		acceleration.y = body->acceleration.y * 1E3 + position.y;
		acceleration.z = body->acceleration.z * 1E3 + position.z;

		DrawLine3D(position, acceleration, RED);
	}
}

void renderView(view_t* view, OrbitalSim_t* sim)
{
	if (IsKeyPressed(FULLSCREEN_KEY))
	{
		ToggleFullscreen();
	}
	if (IsKeyPressed(TOGGLE_SHOW_VELOCITY))
	{
		show_velocity_v = !show_velocity_v;
	}
	if (IsKeyPressed(TOGGLE_SHOW_ACCELERATION))
	{
		show_acceleration_v = !show_acceleration_v;
	}
	if (IsKeyPressed(TOGGLE_EBODIES_RENDER_MODE))
	{
		EBodies_render_mode = !EBodies_render_mode;
	}
	if (IsKeyPressed(TOGGLE_ASTEROIDS_RENDER_MODE))
	{
		Asteroids_render_mode = !Asteroids_render_mode;
	}
	if(IsKeyPressed(TOGGLE_SHOW_CONTROLS))
	{
		show_controls = !show_controls;
	}

	UpdateCamera(&view->camera, CAMERA_FREE);

	BeginDrawing();

	ClearBackground(BLACK);
	BeginMode3D(view->camera);

	// Fill in your 3D drawing code here:

	//DrawGrid(10, 10.0f);

	for (unsigned int i = 0; i < sim->bodyNum; i++) 
	{
		drawBody(&sim->PlanetarySystem[i].body, sim->PlanetarySystem[i].radius, sim->PlanetarySystem[i].color, EBodies_render_mode);
	}
	for (unsigned int i = 0; i < sim->asteroidsNum; i++) 
	{
		drawBody(sim->Asteroids + i, ASTEROIDS_RADIUS, ASTEROIDS_COLOR, Asteroids_render_mode);
	}
	drawBody(&sim->Spaceship.body, sim->Spaceship.radius, sim->Spaceship.color, Spaceship_render_mode);

	EndMode3D();

	// Fill in your 2D drawing code here:
	DrawFPS(10,10);

	if(sim->started)
		DrawText(getISODate(clock(), sim->started),10, 30, 20, RAYWHITE);
	else
		DrawText("0000-00-00", 10, 30, 20, RAYWHITE);

	DrawText(print_simTime_elapsed(get_simTime_elapsed(clock(), sim->started), buffer), 10, 50, 17, RAYWHITE);

	// Show or hide controls menu
	DrawText("Show/Hide Controls: F6", view->width - CONTROLS_X_MARGIN, 10, 20, CONTROLS_COLOR);
	if(show_controls)
	{
		DrawText("Move Spaceship: U, I, O, J, K, L", view->width - CONTROLS_X_MARGIN, 30, 20, CONTROLS_COLOR);
		DrawText("Toggle Bodies Render Mode: F7", view->width - CONTROLS_X_MARGIN, 50, 20, CONTROLS_COLOR);
		DrawText("Toggle Asteroids Render Mode: F8", view->width - CONTROLS_X_MARGIN, 70, 20, CONTROLS_COLOR);
		DrawText("Show/Hide Velocities: F9", view->width - CONTROLS_X_MARGIN, 90, 20, CONTROLS_COLOR);
		DrawText("Show/Hide Accelerations: F10", view->width - CONTROLS_X_MARGIN, 110, 20, CONTROLS_COLOR);
		DrawText("Toggle Fullscreen: F11", view->width - CONTROLS_X_MARGIN, 130, 20, CONTROLS_COLOR);
	}

	EndDrawing();
}