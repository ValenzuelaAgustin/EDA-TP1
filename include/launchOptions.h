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
	EASTER_EGG
};

/**
 * @brief 
 * @param argc 
 * @param argv 
 * @param launchOptionsValues 
 */

 #ifdef __cplusplus
extern "C" {
#endif

void searchLaunchOptions(int argc, char* argv[], int* launchOptionsValues);

#ifdef __cplusplus
}
#endif

#endif