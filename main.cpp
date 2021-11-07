#include "headers/raylib-utils.h"
#include "headers/rocket.h"
#include "headers/graphs.h"
#include <cstdio>
#include <iostream>
#include "raylib.h"
#include <math.h>
#include <string>

struct World{
	float scale;
	int x0;
	int y0;
	int pos_x, pos_y;
	int w, h;
};

void drawPoly(int* xs, int* ys, int n, Color color) {
	for (int i = 0; i < n; i++)
		DrawLine(xs[i], ys[i], xs[(i+1)%n], ys[(i+1)%n], color);
}

void drawPolyBounded(int* xs, int* ys, int n, World wld, Color color) {
	for (int i = 0; i < n; i++)
		drawLineBounded(xs[i], ys[i], xs[(i+1)%n], ys[(i+1)%n], wld.pos_x, wld.pos_y, wld.w, wld.h, color);
}

void drawWorld(World wld, Rocket rkt) {
	DrawRectangle(wld.pos_x, wld.pos_y, wld.w, wld.h, BLACK);
	DrawRectangleLines(wld.pos_x-1, wld.pos_y-1, wld.w+2, wld.h+2, WHITE);
	float pos[2] = {
		wld.pos_x + wld.x0 + rkt.pos_x/wld.scale,
		wld.pos_y + wld.y0 - rkt.pos_y/wld.scale
	};
	
	float up[2] = {
		rkt.height * 0.5f / wld.scale * -sin(rkt.theta),
		rkt.height * 0.5f / wld.scale * cos(rkt.theta)
	};

	float right[2] = {
		rkt.width * 0.5f / wld.scale * cos(rkt.theta),
		rkt.width * 0.5f / wld.scale * sin(rkt.theta)
	};

	int xs[4] = {
		static_cast<int>(pos[0] + right[0] + up[0]),
		static_cast<int>(pos[0] + right[0] - up[0]),
		static_cast<int>(pos[0] - right[0] - up[0]),
		static_cast<int>(pos[0] - right[0] + up[0])
	};

	int ys[4] = {
		static_cast<int>(pos[1] + right[1] + up[1]),
		static_cast<int>(pos[1] + right[1] - up[1]),
		static_cast<int>(pos[1] - right[1] - up[1]),
		static_cast<int>(pos[1] - right[1] + up[1])
	};

	drawPolyBounded(xs, ys, 4, wld, WHITE);

	float th_x = rkt.thruster_pos_y / wld.scale *  sin(rkt.theta) + rkt.thruster_pos_x / wld.scale * cos(rkt.theta);
	float th_y = rkt.thruster_pos_y / wld.scale * -cos(rkt.theta) + rkt.thruster_pos_x / wld.scale * sin(rkt.theta);

	float th_to_x = -sin(rkt.thruster_theta + rkt.theta) * rkt.height * rkt.throttle / wld.scale;
	float th_to_y =  cos(rkt.thruster_theta + rkt.theta) * rkt.height * rkt.throttle / wld.scale;
	drawLineBounded(pos[0] + th_x, pos[1] + th_y, pos[0] + th_x + th_to_x, pos[1] + th_y + th_to_y, wld.pos_x, wld.pos_y, wld.w, wld.h, RED);
	DrawLine(wld.pos_x, wld.pos_y+wld.y0, wld.pos_x+wld.w, wld.pos_y+wld.y0, GREEN);
}

int main() {
	const int screenWidth = 1200;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "demo");

	int frameCount = 0;
	float time = 0.0f;

	Rocket my_rocket;
	my_rocket.set_dimensions(2, 8, 100, 110, 1100);
	my_rocket.thruster_theta = 0.05f;
	my_rocket.burn_through_rate = 0.5f;
	my_rocket.set_pos(0, 50);
	
	World my_world{0.15f, 200, 350, 500, 100, 400, 400};

	chart::Line chart1{
		10, 110,
		200, 200,
		"Demo Chart",

		-1, -1, 11, 51,

		true, true, 2.0f, 2.0f, 0.0f, 0.0f
	};

	SetTargetFPS(100);

	while(!WindowShouldClose()) {
		frameCount++;

		my_rocket.update(GetFrameTime());
		time += GetFrameTime();

		if(time > 1.f) my_rocket.throttle = 1.0f;

		BeginDrawing();

			if(frameCount == 1) {
				ClearBackground(BLACK);

				initializeLineChart(chart1);
			}

			DrawRectangle(0, 0, 400, 90, BLACK);

			plotLineChart(chart1, new float[2]{time, time}, new float[2]{my_rocket.pos_y, -my_rocket.vel_y}, 2);

			drawWorld(my_world, my_rocket);

			DrawText(TextFormat("y pos: %f", my_rocket.pos_y), 10, 10, 20, WHITE);
			DrawText(TextFormat("x pos: %f", my_rocket.pos_x), 10, 30, 20, WHITE);
			DrawText(TextFormat("y velocity: %f", my_rocket.vel_y), 10, 50, 20, WHITE);
			DrawText("fuel: ", 10, 70, 20, WHITE);
			DrawRectangle(60, 75, 100.0f*my_rocket.fuel, 10, BLUE);
			DrawRectangleLines(59, 74, 102, 12, WHITE);

			// DrawFPS(10, 10);

		EndDrawing();
	}
}