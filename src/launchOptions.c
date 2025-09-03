#include "launchOptions.h"

#define NAN ( 1 << (sizeof(int) * 8 - 1) )
#define IS_A_NUMBER(c) ( (c) <= '9' && (c) >= '0')

const launchOptions_t launchOptions[] =
{
	{
		"+fps_target",
		1,
		0
	},
	{
		"-fullscreen",
		0,
		0
	},
	{
		"-w",
		1,
		1280
	},
	{
		"-h",
		1,
		720
	},
	{
		"-days_per_simulation_second",
		1,
		10
	},
	{
		"-asteroids_ammount",
		1,
		0
	},
	{
		"-show_velocity_vectors",
		0,
		0
	},
	{
		"-show_acceleration_vectors",
		0,
		0
	},
	{
		"-massive_jupiter",
		0,
		0
	},
	{
		"-spawn_blackhole",
		0,
		0
	}
};

const int launchOptionsAmmount = sizeof(launchOptions) / sizeof(launchOptions[0]);

static int scmp(const char* S0, const char* S1);
static int str2int(const char* str, int* number);

void searchLaunchOptions(int argc, char* argv[], int* launchOptionsValues)
{
	if (argc < 1 || !argv || !launchOptionsValues)
		return;

	int i, j;

	for (i = 0; i < launchOptionsAmmount; i++)
	{
		launchOptionsValues[i] = launchOptions[i].defaultValue;

		for (j = 1; j < argc; j++)
		{
			if (scmp(launchOptions[i].name, argv[j]))
				continue;
			if (!launchOptions[i].hasArgument)
			{
				launchOptionsValues[i] = 1;
				break;
			}
			if (j + 1 < argc && !str2int(argv[j + 1], launchOptionsValues + i))
				break;
		}
	}
}

static int scmp(const char* S0, const char* S1)
{
	if (!S0 || !S1)
		return 0;

	for (; *S0 && *S1 && *S0 == *S1; S0++, S1++);

	return *S0 - *S1;
}

static int str2int(const char* str, int* number)
{
	if (!str)
		return NAN;

	int n = 0;
	int sign = (*str == '+') - (*str == '-');
	if (sign)
		str++;
	else
		sign = 1;

	while (IS_A_NUMBER(*str))
	{
		n *= 10;
		n += *str - '0';
		str++;
	}

	if (*str)
		return NAN;

	*number = n * sign;
	return 0;
}