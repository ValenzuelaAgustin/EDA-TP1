#include "controller.h"

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
			ToggleFullscreen();
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

static void updateMovementKeysInptus()
{
	for (unsigned int i; i < movement_keys_ammount; i++)
	{
		movement_key_is_down[i] = IsKeyDown(movement_keys[i]);
	}
}