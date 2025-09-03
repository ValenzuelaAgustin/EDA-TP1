#ifndef LAUNCH_OPTIONS_H
#define LAUNCH_OPTIONS_H

typedef struct
{
	const char* name;
	int hasArgument;
	int defaultValue;
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
	SPAWN_BLACKHOLE
};

void searchLaunchOptions(int argc, char* argv[], int* launchOptionsValues);

#endif