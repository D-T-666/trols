#include "raylib.h"

namespace chart {
	struct Line {
		int pos_x;
		int pos_y;
		int width;
		int height;
		const char* title;

		float min_x;
		float min_y;
		float max_x;
		float max_y;

		int g = 0; // 0 _, 1 hor, 2 ver, 3 hor&ver
		float g_freq_x; // frequency of guides
		float g_freq_y;
		float g_off_x; // offset of guides
		float g_off_y;
		
		float xs[10];
		float ys[10];

		bool has_previous_xys = false;
	};

	Line set_screen(Line &c, int x, int y, int w, int h);
	Line set_title(Line &c, const char* t);
	Line set_params(Line &c, float _min_x, float _min_y, float _max_x, float _max_y);
	Line set_guides(Line &c, int guides, float freq_x, float freq_y, float off_x, float off_y);
};

void initializeLineChart(chart::Line chart);
void plotLineChart(chart::Line &chart, float* xs, float* ys, Color* colors, int n);