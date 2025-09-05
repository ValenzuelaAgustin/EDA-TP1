/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "view.h"
#include "nmath.h"
#include <math.h>
#include <time.h>
#include <stdio.h>

#define SECONDS_PER_DAY (time_t)(24 * 60 * 60)
#define SECONDS_PER_YEAR (time_t)(365 * SECONDS_PER_DAY)

#define MIN_WIDTH 800
#define MIN_HEIGHT 600
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define WINDOW_TITLE "EDA Orbital Simulation"

#define ASTEROIDS_COLOR GRAY
#define ASTEROIDS_RADIUS 2E3F

// Controls
#define CONTROLS_X_MARGIN 370
#define CONTROLS_Y_MARGIN 20
#define CONTROLS_COLOR CLITERAL(Color){0, 228, 48, 150} 

enum
{
	SHOW_CONTROLS,
	SPACESHIP_CAMERA_MODE,
	SPACESHIP_RENDER_MODE,
	EBODIES_RENDER_MODE,
	ASTEROIDS_RENDER_MODE,
	SHOW_VELOCITY_VECTORS,
	SHOW_ACCELERATION_VECTORS,
	TOGGLE_REWIND,
	CONTROLS_AMMOUNT // Dejar siempre al final (cantidad total de controles)
};

enum
{
	QUALITY,
	PERFORMANCE
};

typedef struct 
{
	int key, value;
	char description[50];
} control_t;

static char buffer[128];
static control_t controls[] =
{
	// SHOW_CONTROLS
	{
		TOGGLE_SHOW_CONTROLS_KEY,
		1,
		"Show/Hide Controls: F4"
	},
	// SPACESHIP_CAMERA_MODE
	{
		TOGGLE_SPACESHIP_CAMERA_KEY,
		0,
		"Toggle Spaceship Camera: F5"
	},
	// SPACESHIP_RENDER_MODE
	{
		TOGGLE_SPACESHIP_RENDER_MODE_KEY,
		QUALITY,
		"Toggle Spaceship Render Mode: F6"
	},
	// EBODIES_RENDER_MODE
	{
		TOGGLE_EBODIES_RENDER_MODE_KEY,
		QUALITY,
		"Toggle Bodies Render Mode: F7"
	},
	// ASTEROIDS_RENDER_MODE
	{
		TOGGLE_ASTEROIDS_RENDER_MODE_KEY,
		PERFORMANCE,
		"Toggle Asteroids Render Mode: F8"
	},
	// SHOW_VELOCITY_VECTORS
	{
		TOGGLE_SHOW_VELOCITY_KEY,
		0,
		"Show/Hide Velocities: F9"
	},
	// SHOW_ACCELERATION_VECTORS
	{
		TOGGLE_SHOW_ACCELERATION_KEY,
		0,
		"Show/Hide Accelerations: F10"
	},
	// REWIND
	{
		TOGGLE_REWIND_KEY,
		0,
		"Toggle Rewind"
	}
};


/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *		to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * @return The ISO date (a raylib string)
 */
static const char* getISODate(time_t timestamp);

/**
 * 
 */
static void drawBody(Body_t* body, float radius, Color color, int render_mode);

/**
 * 
 */
static const char* getElapsedSimTime(time_t timestamp, char buffer[]);


static const char* getISODate(time_t timestamp)
{
	// Timestamp epoch: 1/1/2022
	struct tm unichEpochTM = {0, 0, 0, 1, 0, 122};

	// Convert timestamp to UNIX timestamp (number of seconds since 1/1/1970)
	time_t unixEpoch = mktime(&unichEpochTM);
	time_t unixTimestamp = unixEpoch + timestamp;

	// Returns ISO date
	struct tm* localTM = localtime(&unixTimestamp);
	return TextFormat("%04d-%02d-%02d",
					1900 + localTM->tm_year, localTM->tm_mon + 1, localTM->tm_mday);
}

static const char* getElapsedSimTime(time_t timestamp, char buffer[])
{
	time_t years = timestamp / SECONDS_PER_YEAR;	// Falta agregar el caso de que el anio sea bisiesto ( agregarle un parametro a la funcion )
	time_t days = (timestamp % SECONDS_PER_YEAR) / SECONDS_PER_DAY;

	sprintf(buffer, "Elapsed Sim Time: %04lld years, %03lld days", years, days);
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

	controls[SHOW_VELOCITY_VECTORS].value = show_velocity_vectors;
	controls[SHOW_ACCELERATION_VECTORS].value = show_acceleration_vectors;
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

	if (controls[SHOW_VELOCITY_VECTORS].value)
	{
		velocity.x = body->velocity.x * 1E-4 + position.x;
		velocity.y = body->velocity.y * 1E-4 + position.y;
		velocity.z = body->velocity.z * 1E-4 + position.z;

		DrawLine3D(position, velocity, BLUE);
	}
	if (controls[SHOW_ACCELERATION_VECTORS].value)
	{
		acceleration.x = body->acceleration.x * 1E3 + position.x;
		acceleration.y = body->acceleration.y * 1E3 + position.y;
		acceleration.z = body->acceleration.z * 1E3 + position.z;

		DrawLine3D(position, acceleration, RED);
	}
}

int renderView(view_t* view, OrbitalSim_t* sim)
{
	if (IsKeyPressed(TOGGLE_FULLSCREEN_KEY))
	{
		ToggleFullscreen();
	}

	// Check for controls toggle
	for(unsigned i = 0; i < CONTROLS_AMMOUNT; i++)
	{
		if(IsKeyPressed(controls[i].key))
		{
			controls[i].value = !controls[i].value;
		}
	}

	UpdateCamera(&view->camera, CAMERA_FREE);

	BeginDrawing();

	ClearBackground(BLACK);
	BeginMode3D(view->camera);

	// Fill in your 3D drawing code here:

	//DrawGrid(10, 10.0f);

	for (unsigned int i = 0; i < sim->bodyNum; i++) 
	{
		drawBody(&sim->PlanetarySystem[i].body, sim->PlanetarySystem[i].radius, sim->PlanetarySystem[i].color, controls[EBODIES_RENDER_MODE].value);
	}
	for (unsigned int i = 0; i < sim->asteroidsNum; i++) 
	{
		drawBody(sim->Asteroids + i, ASTEROIDS_RADIUS, ASTEROIDS_COLOR, controls[ASTEROIDS_RENDER_MODE].value);
	}
	drawBody(&sim->Spaceship.body, sim->Spaceship.radius, sim->Spaceship.color, controls[SPACESHIP_RENDER_MODE].value);

	EndMode3D();

	// Fill in your 2D drawing code here:
	DrawFPS(10,10);

	DrawText(getISODate((time_t)sim->time_elapsed),10, 30, 20, RAYWHITE);
	DrawText(getElapsedSimTime((time_t)sim->time_elapsed, buffer), 10, 50, 20, RAYWHITE);

	// Show or hide controls menu
	DrawText(controls[SHOW_CONTROLS].description, view->width - CONTROLS_X_MARGIN, 10, 20, CONTROLS_COLOR);
	if(controls[SHOW_CONTROLS].value)
	{
		DrawText("Move Spaceship: U, I, O, J, K, L", view->width - CONTROLS_X_MARGIN, 30, 20, CONTROLS_COLOR);
		int yCoord = 50;
		for (unsigned i = 0; i < CONTROLS_AMMOUNT; i++)
		{
			if(controls[i].key == TOGGLE_SHOW_CONTROLS_KEY)
			{
				yCoord -= 20; // Avoid blank space between control descriptions.
				continue;
			}
			DrawText(controls[i].description, view->width - CONTROLS_X_MARGIN, yCoord + CONTROLS_Y_MARGIN * i, 20, CONTROLS_COLOR);
		}

		DrawText("Toggle Fullscreen: F11", view->width - CONTROLS_X_MARGIN, yCoord + CONTROLS_Y_MARGIN * CONTROLS_AMMOUNT, 20, CONTROLS_COLOR);
	}

	EndDrawing();

	return controls[TOGGLE_REWIND].value;
}