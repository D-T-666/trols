#include "raylib.h"
#include "../headers/graphs.h"
#include <iostream>

void drawGridAxis(float min, float max, float freq, float off, int sw, int sh, int sx, int sy, bool horizontal) {
	int start = int((min - off) / freq);
	int end = int((max - off) / freq + 0.5f);
	for (int a = start; a < end; a++){
		int pos = (a * freq + off - min) / (max - min) * (horizontal ? sw : sh) + (horizontal ? sx : sy);
		if (horizontal)
			DrawLine(pos, sy, pos, sy+sh, DARKGRAY);
		else
			DrawLine(sx, pos, sx+sw, pos, DARKGRAY);
	}
} 

void drawGridLines(chart::Line c) {
	if (c.g_x) // X axis
		drawGridAxis(c.min_x, c.max_x, c.g_freq_x, c.g_off_x, c.width, c.height, c.pos_x, c.pos_y, true);
	if (c.g_y) // Y axis
		drawGridAxis(c.min_y, c.max_y, c.g_freq_y, c.g_off_y, c.width, c.height, c.pos_x, c.pos_y, false);
}

void initializeLineChart(chart::Line c) {
	drawGridLines(c);
	// frame
	DrawRectangleLines(c.pos_x - 1, c.pos_y - 1, c.width + 2, c.height + 2, WHITE);
	// name
	DrawText(c.name, c.pos_x, c.pos_y-20, 20, WHITE);
}

void plotLineChart(chart::Line c, float* xs, float* ys, int n){
	Color colors[10] = {
		{138, 79, 255, 255},
		{77, 139, 49, 255},
		{202, 46, 85, 255},
		{225, 206, 122, 255},
		{240, 101, 67, 255},
		{11, 32, 39, 255},
		{113, 0, 0, 255},
		{16, 69, 71, 255},
		{254, 147, 140, 255},
		{128, 206, 215, 255}
	};

	for (int i = 0; i < n; i++) {
		if (xs[i] > c.min_x && xs[i] < c.max_x && ys[i] > c.min_y && ys[i] < c.max_y) {
			float scale_x = float(c.width)/(c.max_x-c.min_x);
			float scale_y = float(c.height)/(c.max_y-c.min_y);
			// DrawLine(
			// 	c.pos_x + scale_x * (-c.min_x + c.xs[i]), 
			// 	c.pos_y + scale_y * (-c.min_y - c.ys[i]),
			// 	c.pos_x + scale_x * (-c.min_x + xs[i]),
			// 	c.pos_y + scale_y * (-c.min_y - ys[i]),
			// 	 GREEN);

			DrawPixel(
				c.pos_x + scale_x * (-c.min_x + xs[i]),
				c.pos_y + c.height - scale_y * (-c.min_y + ys[i]),
				colors[i]);
			c.ys[i] = ys[i];
			c.xs[i] = xs[i];
		}
	}
}
