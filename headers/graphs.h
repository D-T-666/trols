#include "raylib.h"

struct LineChart {
	int pos_x;
	int pos_y;
	int width;
	int height;
	char* name;

	float min_x;
	float min_y;
	float max_x;
	float max_y;

	bool g_x; // guides 
	bool g_y;
	float g_freq_x; // frequency of guides
	float g_freq_y;
	float g_off_x; // offset of guides
	float g_off_y;
};

void initializeLineChart(LineChart chart);