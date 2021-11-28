#include "raylib.h"
#include "../include/raylib-utils.h"
#include "../include/graphs.hpp"
#include <iostream>
#include <vector>
#include <math.h>

void drawGridAxis(float min, float max, float freq, int sw, int sh, int sx, int sy, bool horizontal) {
	int start = int(min / freq);
	int end = int(max / freq + 1.5f);
	for (int a = start; a < end; a++){
		int pos = (a * freq - min) / (max - min) * (horizontal ? sw : sh);
		if (horizontal)
			drawLineBounded(pos+sx, sy, pos+sx, sy+sh, sx, sy, sw, sh, {255, 255, 255, 60});
		else
			drawLineBounded(sx, sh-pos+sy, sx+sw, sh-pos+sy, sx, sy, sw, sh, {255, 255, 255, 60});
	}
} 

void drawGridLines(chart::Line c) {
	int x = (c.full > c.x_range) ? c.full - c.x_range : 0;

	if (c.g & 1) // X axis
		drawGridAxis(x, x+c.x_range, c.g_freq_x, c.width, c.height, c.pos_x, c.pos_y, true);
	if (c.g & 2) // Y axis
		drawGridAxis(c.min_y, c.max_y, c.g_freq_y, c.width, c.height, c.pos_x, c.pos_y, false);
}

void chart::line_addData(chart::Line &c, float *ys) {
	for (int i = 0; i < c.n; i++) {
		if (c.full < c.x_range) {
			c.ys[i].push_back(ys[i]);
		} else {
			c.ys[i][c.full%c.x_range] = ys[i];
		}


		if (c.full || i > 0) {
			float padding = (c.max_y - c.min_y)* 0.1f;
			c.min_y = fmin(c.min_y, ys[i] - padding);
			c.max_y = fmax(c.max_y, ys[i] + padding);
		} else {
			c.min_y = ys[i] - 0.1f;
			c.max_y = ys[i] + 0.1f;
		}
	}
	c.full++;
}

void chart::line_display(chart::Line &c){
	// frame
	DrawRectangle(c.pos_x - 1, c.pos_y - 1, c.width + 2, c.height + 2, WHITE);
	DrawRectangle(c.pos_x, c.pos_y, c.width, c.height, {22,22,22,255});
	// Guides - Grid lines
	drawGridLines(c);
	// name
	DrawText(c.title, c.pos_x, c.pos_y-20, 20, WHITE);

	// 
	for (int i = 0; i < c.n; i++) {
		for (int j = 0; j < c.x_range && j < c.full-1; j++) {
			float scale_y = float(c.height)/(c.max_y-c.min_y);
			
			int jj = j;

			if (c.full >= c.x_range) {
				jj = (j + c.full) % c.x_range;
			}

			int x = (c.full < c.x_range) ? jj : (c.x_range + jj - c.full % c.x_range) % c.x_range;

			int x1 = c.pos_x + c.x_scale * x;
			int y1 = c.pos_y + c.height - scale_y * (-c.min_y + c.ys[i][(jj)]);

			int x2 = c.pos_x + c.x_scale * x + c.x_scale;
			int y2 = c.pos_y + c.height - scale_y * (-c.min_y + c.ys[i][(jj+1)%c.x_range]);

			if (j!=c.x_range-1)
			drawLineBounded(
				x1, y1, x2, y2,
				c.pos_x, c.pos_y, c.width, c.height,
				c.colors[i]);
		}
	}
}

chart::Line chart::set_screen(chart::Line &c, int x, int y, int w, int h) {
	c.pos_x = x;
	c.pos_y = y;
	c.width = w;
	c.height = h;

	if (c.x_scale) {
		c.x_range = c.width / c.x_scale+1;
	}

	return c;
}

chart::Line chart::set_title(chart::Line &c, const char* t) {
	c.title = t;

	return c;
}

chart::Line chart::set_params(chart::Line &c, float _x_scale, int n, Color* colors) {
	c.n = n;
	
	for (int i = 0; i < n; i++) {
		c.colors.push_back(colors[i]);

		std::vector<float> a;
		c.ys.push_back(a);
	}

	c.x_scale = _x_scale;

	if (c.width)
		c.x_range = c.width/_x_scale+1;

	return c;
}

chart::Line chart::set_guides(chart::Line &c, int guides, float freq_x, float freq_y) {
	c.g = guides;
	c.g_freq_x = freq_x;
	c.g_freq_y = freq_y;

	return c;
}