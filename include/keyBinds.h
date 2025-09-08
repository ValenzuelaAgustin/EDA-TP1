/**
 * @brief Key binds for orbital simulation
 * 
 * @author Sofia Capiel
 * @author Agustin Tomas Valenzuela
 * @author Francisco Alonso Paredes
 */
#ifndef KEYBINDS_H
#define KEYBINDS_H
#include <raylib.h>

#define SWITCH_BODY_CAMERA_KEY KEY_T
#define TOGGLE_FULLSCREEN_KEY KEY_F11

typedef struct 
{
	int key;
	const char* description;
} keybind_t;

extern const keybind_t keybinds[];
extern unsigned int keybinds_values[];

extern const int movement_keys[];
extern int movement_key_is_down[];
extern const unsigned int movement_keys_ammount;

enum
{
	SHOW_KEYBINDS,
	CAMERA_MODE,
	SPACESHIP_RENDER_MODE,
	EBODIES_RENDER_MODE,
	ASTEROIDS_RENDER_MODE,
	TOGGLE_SHOW_VELOCITY_VECTORS,
	TOGGLE_SHOW_ACCELERATION_VECTORS,
	TOGGLE_REWIND,
	SWITCH_BODY,
	TOGGLE_FULLSCREEN,
	KEYBINDS_AMMOUNT // Dejar siempre al final (cantidad total de keybinds)
};

#endif