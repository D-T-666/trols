#include "raylib.h"
#include "math.h"


int get_out_code(int x, int y, int x_min, int y_min, int x_max, int y_max) {
	int code = 0;
	if (x < x_min)           // to the left of clip window
		code |= 1;
	else if (x > x_max)      // to the right of clip window
		code |= 2;
	if (y < y_min)           // below the clip window
		code |= 4;
	else if (y > y_max)      // above the clip window
		code |= 8;
	return code;
}

void drawLineBounded(int x1, int y1, int x2, int y2, int x_min, int y_min, int w, int h, Color color) {
	int x_max = x_min + w;
	int y_max = y_min + h;
	
	int out_code_1 = get_out_code(x1, y1, x_min, y_min, x_max, y_max);
	int out_code_2 = get_out_code(x2, y2, x_min, y_min, x_max, y_max);

	while (true) {
		if (!(out_code_1 | out_code_2)) {
			DrawLine(x1, y1, x2, y2, color);
			break;
		} else if (out_code_1 & out_code_2) {
			break;
		} else {
			int out_code_out = out_code_1 > out_code_2 ? out_code_1 : out_code_2;

			int x, y;

			if (out_code_out & 8) {			// point is above the clip window
				x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
				y = y_max;
			} else if (out_code_out & 4) {	// point is below the clip window
				x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
				y = y_min;
			} else if (out_code_out & 2) {	// point is to the right of clip window
				y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
				x = x_max;
			} else if (out_code_out & 1) {	// point is to the left of clip window
				y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
				x = x_min;
			}

			if (out_code_out == out_code_1) {
				x1 = x;
				y1 = y;
				out_code_1 = get_out_code(x1, y1, x_min, y_min, x_max, y_max);
			} else {
				x2 = x;
				y2 = y;
				out_code_2 = get_out_code(x2, y2, x_min, y_min, x_max, y_max);
			}
		}
	}
}