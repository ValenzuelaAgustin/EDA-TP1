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
	for(unsigned int i = 0; i < KEYBINDS_AMOUNT; i++)
	{
		if(!IsKeyPressed(keybinds[i].key))
		{
			continue;
		}
		switch (keybinds[i].key)
		{
		case TOGGLE_FULLSCREEN_KEY:
			keybindsValues[TOGGLE_FULLSCREEN] = 1;
			break;
		case SWITCH_BODY_CAMERA_KEY:
			keybindsValues[i]++;
			keybindsValues[i] = (keybindsValues[i] <= bodyNum) ? keybindsValues[i] : 0;
			break;
		default:
			keybindsValues[i] = !keybindsValues[i];
			break;
		}
	}
}

static void updateMovementKeysInptus()
{
	for (unsigned int i = 0; i < movementKeysAmount; i++)
	{
		movementKeyIsDown[i] = IsKeyDown(movementKeys[i]);
	}
}