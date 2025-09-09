/**
 * @brief  Updates keybinds values based on user input
 * 
 * @author Sofia Capiel
 * @author Agustin Tomas Valenzuela
 * @author Francisco Alonso Paredes
 */

#include "controller.h"

/**
 * @brief Updates the controls values based on the user's input.
 * @param bodyNum The number of bodies in the simulation.
 */
static void updateMovementKeysInptus();

void updateUserInputs(unsigned int bodyNum)
{
	updateMovementKeysInptus();
	for(unsigned int i = 0; i < KEYBINDS_AMMOUNT; i++)
	{
		if(!IsKeyPressed(keybinds[i].key))
		{
			continue;
		}
		switch (keybinds[i].key)
		{
		case TOGGLE_FULLSCREEN_KEY:
			keybinds_values[TOGGLE_FULLSCREEN] = 1;
			break;
		case SWITCH_BODY_CAMERA_KEY:
			keybinds_values[i]++;
			keybinds_values[i] = (keybinds_values[i] <= bodyNum) ? keybinds_values[i] : 0;
			break;
		default:
			keybinds_values[i] = !keybinds_values[i];
			break;
		}
	}
}

/**
 * @brief Updates the value of the keys used to control the spaceship.
 **/
static void updateMovementKeysInptus()
{
	for (unsigned int i = 0; i < movement_keys_ammount; i++)
	{
		movement_key_is_down[i] = IsKeyDown(movement_keys[i]);
	}
}