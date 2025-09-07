#include "keyBinds.h"

#define TOGGLE_REWIND_KEY KEY_R
#define TOGGLE_SHOW_KEYBINDS_KEY KEY_F4
#define TOGGLE_CAMERA_MODE_KEY KEY_F5
#define TOGGLE_SPACESHIP_RENDER_MODE_KEY KEY_F6
#define TOGGLE_EBODIES_RENDER_MODE_KEY KEY_F7
#define TOGGLE_ASTEROIDS_RENDER_MODE_KEY KEY_F8
#define TOGGLE_SHOW_VELOCITY_KEY KEY_F9
#define TOGGLE_SHOW_ACCELERATION_KEY KEY_F10

#define SPACESHIP_XP_KEY KEY_U
#define SPACESHIP_YP_KEY KEY_I
#define SPACESHIP_ZP_KEY KEY_O
#define SPACESHIP_XN_KEY KEY_J
#define SPACESHIP_YN_KEY KEY_K
#define SPACESHIP_ZN_KEY KEY_L

const keybind_t keybinds[KEYBINDS_AMMOUNT] =
{
	// SHOW_KEYBINDS
	{
		TOGGLE_SHOW_KEYBINDS_KEY,
		"Show/Hide Key Binds: F4"
	},
	// CAMERA_MODE
	{
		TOGGLE_CAMERA_MODE_KEY,
		"Toggle Camera Mode: F5"
	},
	// SPACESHIP_RENDER_MODE
	{
		TOGGLE_SPACESHIP_RENDER_MODE_KEY,
		"Toggle Spaceship Render Mode: F6"
	},
	// EBODIES_RENDER_MODE
	{
		TOGGLE_EBODIES_RENDER_MODE_KEY,
		"Toggle Bodies Render Mode: F7"
	},
	// ASTEROIDS_RENDER_MODE
	{
		TOGGLE_ASTEROIDS_RENDER_MODE_KEY,
		"Toggle Asteroids Render Mode: F8"
	},
	// TOGGLE_SHOW_VELOCITY_VECTORS
	{
		TOGGLE_SHOW_VELOCITY_KEY,
		"Show/Hide Velocities: F9"
	},
	// TOGGLE_SHOW_ACCELERATION_VECTORS
	{
		TOGGLE_SHOW_ACCELERATION_KEY,
		"Show/Hide Accelerations: F10"
	},
	// REWIND
	{
		TOGGLE_REWIND_KEY,
		"Toggle Rewind: R"
	},
	// SWITCH_BODY
	{
		SWITCH_BODY_CAMERA_KEY,
		"Switch Camera Target: T"
	},
	// TOGGLE_FULLSCREEN
	{
		TOGGLE_FULLSCREEN_KEY,
		"Toggle Fullscreen: F11"
	}
};

unsigned int keybinds_values[KEYBINDS_AMMOUNT];

const int movement_keys[] =
{
	SPACESHIP_XP_KEY, SPACESHIP_YP_KEY, SPACESHIP_ZP_KEY,
	SPACESHIP_XN_KEY, SPACESHIP_YN_KEY, SPACESHIP_ZN_KEY
};

const unsigned int movement_keys_ammount = sizeof(movement_keys) / sizeof(movement_keys[0]);

int movement_key_is_down[movement_keys_ammount];