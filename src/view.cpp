/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */
s
#include "view.h"
#include "raymath.h"
#include "nmath.h"
#include <time.h>

#define MIN_WIDTH 800
#define MIN_HEIGHT 600
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define WINDOW_TITLE "EDA Orbital Simulation"

// Controls
#define CONTROLS_X_MARGIN 370
#define CONTROLS_COLOR CLITERAL(Color){0, 228, 48, 150} 

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

static void drawBody(EphemeridesBody_t* body, int render_mode);

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

static const char* get_simTime_elapsed (float timestamp)
{
	time_t new_timestamp = (time_t)timestamp;

	struct tm* localTM = localtime(&new_timestamp);
	return TextFormat("%04d years, %02d months, %02d hours", localTM->tm_year - 70, localTM->tm_mon, localTM->tm_mday);
}
char* print_simTime_elapsed (const char* simTime, char buffer [], int flag)
{
	if(flag)
	sprintf(buffer, "Elapsed Sim Time: %s ", simTime);
	else
	sprintf(buffer, "Elapsed Sim Time: 0000 years, 00 months, 00 seconds");
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

static void drawBody(EphemeridesBody_t* body, int render_mode)
{
	Vector3 position, velocity, acceleration;

	position.x = body->position[X] * 1E-11;
	position.y = body->position[Y] * 1E-11;
	position.z = body->position[Z] * 1E-11;

	switch (render_mode)
	{
	default:
	case QUALITY:
		DrawSphereEx(position, 0.005F * logf(body->radius), 5, 7, body->color);
		break;
	case PERFORMANCE:
		DrawPoint3D(position, body->color);
		break;
	}

	if (show_velocity_v)
	{
		velocity.x = body->velocity[X] * 1E-4 + position.x;
		velocity.y = body->velocity[Y] * 1E-4 + position.y;
		velocity.z = body->velocity[Z] * 1E-4 + position.z;

		DrawLine3D(position, velocity, BLUE);
	}
	if (show_acceleration_v)
	{
		acceleration.x = body->acceleration[X] * 1E3 + position.x;
		acceleration.y = body->acceleration[Y] * 1E3 + position.y;
		acceleration.z = body->acceleration[Z] * 1E3 + position.z;

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
		drawBody(sim->EphemeridesBody + i, EBodies_render_mode);
	}
	for (unsigned int i = 0; i < sim->asteroidsNum; i++) 
	{
		drawBody(sim->Asteroids + i, Asteroids_render_mode);
	}
	drawBody(&sim->spaceship, Spaceship_render_mode);

	EndMode3D();

	// Fill in your 2D drawing code here:
	DrawFPS(10,10);

	if(sim->started)
		DrawText(getISODate(clock()),10, 30, 20, RAYWHITE);
	else
		DrawText("0000-00-00", 10, 30, 20, RAYWHITE);

	DrawText(print_simTime_elapsed(get_simTime_elapsed(clock()), buffer, sim->started), 10, 50, 17, RAYWHITE);

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