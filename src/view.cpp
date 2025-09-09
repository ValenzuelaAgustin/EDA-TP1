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

#include "view.h"
#include "vector3D.h"
#include "keyBinds.h"
#include <math.h>
#include <raymath.h>
#include <time.h>
#include <stdio.h>

#define SECONDS_PER_DAY (time_t)(24 * 60 * 60)
#define SECONDS_PER_YEAR (time_t)(365 * SECONDS_PER_DAY)

#define MIN_WIDTH 800
#define MIN_HEIGHT 600
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define WINDOW_TITLE "EDA Orbital Simulation"

enum
{
	QUALITY,
	PERFORMANCE
};

// Spaceship constants
#define CAMERA_DISTANCE 1.5f

// Asteroids constants
#define ASTEROIDS_COLOR GRAY
#define ASTEROIDS_RADIUS 2E3F

// Scale factors for display purposes
#define POSITION_SCALE_FACTOR 1E-11
#define VELOCITY_SCALE_FACTOR 1E-4
#define ACCELERATION_SCALE_FACTOR 1E3

// Controls
#define CONTROLS_X_MARGIN 370
#define CONTROLS_Y_MARGIN 20
#define CONTROLS_COLOR CLITERAL(Color){0, 228, 48, 150}


/**
 * Private variables
*/

static Vector3 last_camera_position, last_camera_target;
static int camera_mode = CAMERA_FREE;
static char buffer[128];

/**
 * Private function declarations
 */

/**
 * @brief Returns the vector parsed in the raylib data type Vector3
 * 
 * @param vector The vector wishing to convert.
 * 
 * @return The values of the vector entered in a Vector3 data type.
 */
static Vector3 toVector3(vector3D_t vector);

/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *		to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * 
 * @return The ISO date (a raylib string)
 */
static const char* getISODate(time_t timestamp);

/**
 * @brief Gets the elapsed time of the simulation.
 * 
 * @param timestamp The current time stamp of the simulation.
 * @param buffer The buffer where the return string is stored.
 * 
 * @return Returns the value of the buffer formatted in a message to print.
 */
static const char* getElapsedSimTime(time_t timestamp, char buffer[]);

/**
 * @brief Updates the settings of the camera.
 * 
 * @param view Pointer to the view object containing the camera.
 * @param sim Pointer to the simulation.
 */
static void updateCameraSettings(view_t* view, OrbitalSim_t* sim);

/**
 * @brief Draws a body in the simulation.
 * 
 * @param body The body wishing to draw.
 * @param radius The radius of the body.
 * @param color The color of the body.
 * @param render_mode The mode of the rendering (QUALITY or PERFORMANCE).
 */
static void drawBody(Body_t* body, float radius, Color color, unsigned int render_mode);

/**
 * @brief Draws all the entities of the simulation.
 * 
 * @param sim Pointer to the simulation.
 */
static void drawOrbitalSimuationEntities(OrbitalSim_t* sim);

/**
 * @brief Prints the keybinds to show the features available.
 * 
 * @param view Pointer to view object containing the camera.
 */
static void printKeybinds(view_t* view);

/**
 * Public function definitions.
 */

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

	keybinds_values[TOGGLE_SHOW_VELOCITY_VECTORS] = show_velocity_vectors;
	keybinds_values[TOGGLE_SHOW_ACCELERATION_VECTORS] = show_acceleration_vectors;
	SetTargetFPS(fps);

	DisableCursor();
	view->camera.position = {10.0f, 10.0f, 10.0f};
	view->camera.target = {0.0f, 0.0f, 0.0f};
	view->camera.up = {0.0f, 1.0f, 0.0f};
	view->camera.fovy = 45.0f;
	view->camera.projection = CAMERA_PERSPECTIVE;
	view->width = width;
	view->height = height;

	last_camera_position = view->camera.position;
	last_camera_target = view->camera.target;
	keybinds_values[ASTEROIDS_RENDER_MODE] = PERFORMANCE;

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

void renderView(view_t* view, OrbitalSim_t* sim)
{
	if (keybinds_values[TOGGLE_FULLSCREEN])
	{
		ToggleFullscreen();
		keybinds_values[TOGGLE_FULLSCREEN] = 0;
	}

	updateCameraSettings(view, sim);
	UpdateCamera(&view->camera, camera_mode);

	BeginDrawing();

	ClearBackground(BLACK);

	BeginMode3D(view->camera);
	DrawGrid(10, 10.0f);
	drawOrbitalSimuationEntities(sim);
	EndMode3D();

	DrawFPS(10,10);
	DrawText(getISODate((time_t)sim->time_elapsed),10, 30, 20, RAYWHITE);
	DrawText(getElapsedSimTime((time_t)sim->time_elapsed, buffer), 10, 50, 20, RAYWHITE);
	printKeybinds(view);
	EndDrawing();
}

/**
 * Private function definitions
 */

static Vector3 toVector3(vector3D_t vector)
{
	return Vector3
	{
		(float)vector.x,
		(float)vector.y,
		(float)vector.z
	};
}

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
	time_t years = timestamp / SECONDS_PER_YEAR;
	time_t days = (timestamp % SECONDS_PER_YEAR) / SECONDS_PER_DAY;

	sprintf(buffer, "Elapsed Sim Time: %04lld years, %03lld days", years, days);
	return buffer;
}

static void updateCameraSettings(view_t* view, OrbitalSim_t* sim)
{
	if (!keybinds_values[CAMERA_MODE])
	{
		if (camera_mode == CAMERA_THIRD_PERSON)
		{
			view->camera.position = last_camera_position;
			view->camera.target = last_camera_target;
		}
		camera_mode = CAMERA_FREE;
		return;
	}
	if (camera_mode == CAMERA_FREE)
	{
		last_camera_position = view->camera.position;
		last_camera_target = view->camera.target;
	}
	camera_mode = CAMERA_THIRD_PERSON;

	unsigned int target_body = keybinds_values[SWITCH_BODY];
	Vector3 position, normalized_velocity;
	Body_t body;
	float norm;

	if (target_body < sim->bodyNum)
	{
		body = sim->PlanetarySystem[target_body].body;
	}
	else
	{
		body = sim->Spaceship.body;
	}

	norm = 1.0f / sqrtf(DOT_PRODUCT(body.velocity, body.velocity));
	position = Vector3Scale(toVector3(body.position), POSITION_SCALE_FACTOR);
	normalized_velocity = Vector3Scale(toVector3(body.velocity), norm);

	view->camera.position = Vector3Add(position, Vector3Scale(normalized_velocity, -CAMERA_DISTANCE));
	view->camera.target = position;
}

static void drawBody(Body_t* body, float radius, Color color, unsigned int render_mode)
{
	Vector3 position;

	position.x = body->position.x * POSITION_SCALE_FACTOR;
	position.y = body->position.y * POSITION_SCALE_FACTOR;
	position.z = body->position.z * POSITION_SCALE_FACTOR;

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

	if (keybinds_values[TOGGLE_SHOW_VELOCITY_VECTORS])
	{
		Vector3 velocity;
		velocity.x = body->velocity.x * VELOCITY_SCALE_FACTOR + position.x;
		velocity.y = body->velocity.y * VELOCITY_SCALE_FACTOR + position.y;
		velocity.z = body->velocity.z * VELOCITY_SCALE_FACTOR + position.z;

		DrawLine3D(position, velocity, BLUE);
	}
	if (keybinds_values[TOGGLE_SHOW_ACCELERATION_VECTORS])
	{
		Vector3 acceleration;
		acceleration.x = body->acceleration.x * ACCELERATION_SCALE_FACTOR + position.x;
		acceleration.y = body->acceleration.y * ACCELERATION_SCALE_FACTOR + position.y;
		acceleration.z = body->acceleration.z * ACCELERATION_SCALE_FACTOR + position.z;

		DrawLine3D(position, acceleration, RED);
	}
}

static void drawOrbitalSimuationEntities(OrbitalSim_t* sim)
{
	for (unsigned int i = 0; i < sim->bodyNum; i++) 
	{
		drawBody(&sim->PlanetarySystem[i].body, sim->PlanetarySystem[i].radius, sim->PlanetarySystem[i].color, keybinds_values[EBODIES_RENDER_MODE]);
	}
	for (unsigned int i = 0; i < sim->asteroidsNum; i++) 
	{
		drawBody(sim->Asteroids + i, ASTEROIDS_RADIUS, ASTEROIDS_COLOR, keybinds_values[ASTEROIDS_RENDER_MODE]);
	}
	drawBody(&sim->Spaceship.body, sim->Spaceship.radius, sim->Spaceship.color, keybinds_values[SPACESHIP_RENDER_MODE]);
	drawBody(&sim->BlackHole.body, sim->BlackHole.absorbRadius, PINK, QUALITY);
}

static void printKeybinds(view_t* view)
{
	DrawText(keybinds[SHOW_KEYBINDS].description, view->width - CONTROLS_X_MARGIN, 10, 20, CONTROLS_COLOR);

	if(!keybinds_values[SHOW_KEYBINDS])
		return;

	int yCoord = 30;
	DrawText("Move Spaceship: U, I, O, J, K, L", view->width - CONTROLS_X_MARGIN, yCoord, 20, CONTROLS_COLOR);
	for (unsigned int i = SHOW_KEYBINDS + 1; i < KEYBINDS_AMMOUNT; i++)
	{
		DrawText(keybinds[i].description, view->width - CONTROLS_X_MARGIN, yCoord + CONTROLS_Y_MARGIN * i, 20, CONTROLS_COLOR);
	}
}