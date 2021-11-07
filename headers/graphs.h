#include "raylib.h"

namespace chart {
	struct Line {
		int pos_x;
		int pos_y;
		int width;
		int height;
		const char* name;

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
		
		float xs[10];
		float ys[10];
	};

};

void initializeLineChart(chart::Line chart);
void plotLineChart(chart::Line chart, float* xs, float* ys, int n);