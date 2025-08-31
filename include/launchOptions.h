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
	FPS_MAX,
	FULLSCREEN,
	WIDTH,
	HEIGHT,
	ASTEROIDS_AMMOUNT,
	SHOW_VELOCITY_VECTORS,
	SHOW_ACCELERATION_VECTORS
};

void searchLaunchOptions(int argc, char* argv[], int* launchOptionsValues);

#endif