#include "raylib.h"
#include "../include/raylib-utils.h"
#include "../include/graphs.h"
#include <iostream>

void drawGridAxis(float min, float max, float freq, float off, int sw, int sh, int sx, int sy, bool horizontal) {
	int start = int((min - off) / freq);
	int end = int((max - off) / freq + 0.5f);
	for (int a = start; a < end; a++){
		int pos = (a * freq + off - min) / (max - min) * (horizontal ? sw : sh) + (horizontal ? sx : sy);
		if (horizontal)
			DrawLine(pos, sy, pos, sy+sh, {60, 60, 60, 255});
		else
			DrawLine(sx, pos, sx+sw, pos, {60, 60, 60, 255});
	}
} 

void drawGridLines(chart::Line c) {
	if (c.g & 1) // X axis
		drawGridAxis(c.min_x, c.max_x, c.g_freq_x, c.g_off_x, c.width, c.height, c.pos_x, c.pos_y, true);
	if (c.g & 2) // Y axis
		drawGridAxis(c.min_y, c.max_y, c.g_freq_y, c.g_off_y, c.width, c.height, c.pos_x, c.pos_y, false);
}

void initializeLineChart(chart::Line c) {
	drawGridLines(c);
	// frame
	DrawRectangleLines(c.pos_x - 1, c.pos_y - 1, c.width + 2, c.height + 2,WHITE);
	// name
	DrawText(c.title, c.pos_x, c.pos_y-20, 20,WHITE);
}

void plotLineChart(chart::Line &c, float* xs, float* ys, Color* colors, int n){
	for (int i = 0; i < n; i++) {
		if (c.has_previous_xys && xs[i] > c.min_x && xs[i] < c.max_x && ys[i] > c.min_y && ys[i] < c.max_y) {
			float scale_x = float(c.width)/(c.max_x-c.min_x);
			float scale_y = float(c.height)/(c.max_y-c.min_y);
			drawLineBounded(
				c.pos_x + scale_x * (-c.min_x + c.xs[i]), 
				c.pos_y + c.height - scale_y * (-c.min_y + c.ys[i]),
				c.pos_x + scale_x * (-c.min_x + xs[i]),
				c.pos_y + c.height - scale_y * (-c.min_y + ys[i]),
				c.pos_x, c.pos_y, c.width, c.height,
				colors[i]);
		}
		c.ys[i] = ys[i];
		c.xs[i] = xs[i];
	}
	c.has_previous_xys = true;
}

chart::Line chart::set_screen(chart::Line &c, int x, int y, int w, int h) {
	c.pos_x = x;
	c.pos_y = y;
	c.width = w;
	c.height = h;

	return c;
}

chart::Line chart::set_title(chart::Line &c, const char* t) {
	c.title = t;

	return c;
}

chart::Line chart::set_params(chart::Line &c, float _min_x, float _min_y, float _max_x, float _max_y) {
	c.min_x = _min_x;
	c.min_y = _min_y;
	c.max_x = _max_x;
	c.max_y = _max_y;

	return c;
}

chart::Line chart::set_guides(chart::Line &c, int guides, float freq_x, float freq_y, float off_x, float off_y) {
	c.g = guides;
	c.g_freq_x = freq_x;
	c.g_freq_y = freq_y;
	c.g_off_x = off_x;
	c.g_off_y = off_y;

	return c;
}