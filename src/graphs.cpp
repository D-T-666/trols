#include "raylib.h"
#include "../headers/graphs.h"

void drawGridAxis(float min, float max, float freq, float off, int sw, int sh, int sx, int sy, bool horizontal) {
	int start = int((min - off) / freq + 0.5f);
	int end = int((max - off) / freq);
	for (int a = start; a < end; a++){
		int pos = (a * freq + off - min) / (max - min) * (horizontal ? sw : sh) + (horizontal ? sx : sy);
		if (horizontal)
			DrawLine(pos, sy, pos, sy+sh, DARKGRAY);
		else
			DrawLine(sx, pos, sx+sw, pos, DARKGRAY);
	}
} 

void drawGridLines(LineChart c) {
	if (c.g_x) // X axis
		drawGridAxis(c.min_x, c.max_x, c.g_freq_x, c.g_off_x, c.width, c.height, c.pos_x, c.pos_y, true);
	if (c.g_y) // Y axis
		drawGridAxis(c.min_y, c.max_y, c.g_freq_y, c.g_off_y, c.width, c.height, c.pos_x, c.pos_y, false);
}

void initializeLineChart(LineChart c) {
	drawGridLines(c);
	// frame
	DrawRectangleLines(c.pos_x - 1, c.pos_y - 1, c.width + 2, c.height + 2, WHITE);
}