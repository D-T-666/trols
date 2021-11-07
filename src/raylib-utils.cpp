#include "raylib.h"
#include "math.h"

void drawLineBounded(int x1, int y1, int x2, int y2, int x_min, int y_min, int w, int h, Color color) {
	if (!(x2-x1)) { // Vertical line
		if (x1 - x_min < w && x1 - x_min >= 0 && !((y1 < y_min || y1 > y_min + h) && (y2 < y_min || y2 > y_min + h))) {
			int _y1 = y1 < y2 ? y1 : y2;
			int _y2 = y1 > y2 ? y1 : y2;

			_y1 = _y1 > y_min ? _y1 : y_min;
			_y2 = _y2 < (y_min + h) ? _y2 : y_min + h;

			DrawLine(x1, _y1, x1, _y2, color);
		}
	} else { // Non-vertical line
		float k = float(y2-y1) / float(x2-x1);
		float b = float(y1)-k*float(x1);

		int x_max = x_min + w;
		int y_max = y_min + h;

		DrawLine(0, y_min, 10000, y_min, RED);
		DrawLine(0, y_max, 10000, y_max, RED);
		DrawLine(x_min, 0, x_min, 10000, RED);
		DrawLine(x_max, 0, x_max, 10000, RED);

		bool intersects = false;

		float i1 = (float(y_min)-(float(h)/float(w))*x_min-b) / (k - (float(h)/float(w)));
		float i2 = (float(y_max)-(-float(h)/float(w))*x_min-b) / (k - (-float(h)/float(w)));

		if (x_min < i1 && i1 < x_max) intersects = true;
		if (x_min < i2 && i2 < x_max) intersects = true;

		if (intersects) {
			int x_min_x = x_min,
				x_min_y = float(x_min) * k + b,
				x_max_x = x_max,
				x_max_y = float(x_max) * k + b,
				y_min_x = (abs(k) > 0.0001f) ? float(y_min) / k - b / k : 0,
				y_min_y = y_min,
				y_max_x = (abs(k) > 0.0001f) ? float(y_max) / k - b / k : w,
				y_max_y = y_max;

			int _x1 = x1,
				_y1 = y1,
				_x2 = x2,
				_y2 = y2;

			if (x_min_x > _x1 && x_min_x < _x2) {
				_x1 = x_min_x;
				_y1 = x_min_y;
			}
			if (x_max_x > _x1 && x_max_x < _x2) {
				_x2 = x_max_x;
				_y2 = x_max_y;
			}
			if (y_min_x > _x1 && y_min_x < _x2) {
				if (k > 0) {
					_x1 = y_min_x;
					_y1 = y_min_y;
				} else {
					_x2 = y_min_x;
					_y2 = y_min_y;
				}
			}
			if (y_max_x > _x1 && y_max_x < _x2) {
				if (k > 0) {
					_x2 = y_max_x;
					_y2 = y_max_y;
				} else {
					_x1 = y_max_x;
					_y1 = y_max_y;
				}
			}

			DrawLine(_x1, _y1, _x2, _y2, color);
		}
	}
}