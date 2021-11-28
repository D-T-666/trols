#include "raylib.h"
#include <vector>

using std::vector;

namespace chart {
	struct Line {
		int pos_x, pos_y;
		int width, height;
		const char* title;

		float x_scale;
		int x_range;
		float min_y, max_y;

		int g = 0; // 01 hor & 10 ver
		float g_freq_x; // frequency of guides
		float g_freq_y;
		
		vector<vector<float>> ys;
		int head;
		int full = 0;

		int n;
		vector<Color> colors;

		bool has_previous_xys = false;
	};

	Line set_screen(Line &c, int x, int y, int w, int h);
	Line set_title(Line &c, const char* t);
	Line set_params(Line &c, float _x_scale, int n, Color* colors);
	Line set_guides(Line &c, int guides, float freq_x, float freq_y);
	void line_addData(Line &chart, float* ys);
	void line_display(chart::Line &chart);
};
