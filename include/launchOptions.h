#ifndef LAUNCH_OPTIONS_H
#define LAUNCH_OPTIONS_H

typedef struct
{
	const char* name;
	int hasArgument;
	int defaultValue;
	int valueRange[2];	// Closed Interval
} launchOptions_t;

extern const launchOptions_t launchOptions[];
extern const int launchOptionsAmmount;

enum
{
	TARGET_FPS,
	FULLSCREEN,
	WIDTH,
	HEIGHT,
	DAYS_PER_SIMULATION_SECOND,
	ASTEROIDS_AMMOUNT,
	SHOW_VELOCITY_VECTORS,
	SHOW_ACCELERATION_VECTORS,
	MASSIVE_JUPITER,
	SPAWN_BLACKHOLE,
	EASTER_EGG,
	SYSTEM
};

/**
 * @brief Validates the entered parameters (launch options) in the execution of the program.
 *
 * @param argc The ammount of the entered parameters.
 * @param argv The array of the entered parameters.
 * @param launchOptionsValues Array with the values of each parameter (parsed to assign each value).
 */
void searchLaunchOptions(int argc, char* argv[], int* launchOptionsValues);

#endif