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
};

void drawPoly(int* xs, int* ys, int n, Color color) {
	for (int i = 0; i < n; i++)
		DrawLine(xs[i], ys[i], xs[(i+1)%n], ys[(i+1)%n], color);
}

void drawGround(World wld, int width, Color color) {
	DrawLine(0, wld.y0, width, wld.y0, color);
}

void drawRocket(World wld, Rocket rkt) {
	float pos[2] = {
		wld.x0 + rkt.pos_x/wld.scale,
		wld.y0 - rkt.pos_y/wld.scale
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

	drawPoly(xs, ys, 4, WHITE);

	float th_x = rkt.thruster_pos_y / wld.scale *  sin(rkt.theta) + rkt.thruster_pos_x / wld.scale * cos(rkt.theta);
	float th_y = rkt.thruster_pos_y / wld.scale * -cos(rkt.theta) + rkt.thruster_pos_x / wld.scale * sin(rkt.theta);

	float th_to_x = -sin(rkt.thruster_theta + rkt.theta) * rkt.height * rkt.throttle / wld.scale;
	float th_to_y =  cos(rkt.thruster_theta + rkt.theta) * rkt.height * rkt.throttle / wld.scale;
	DrawLine(pos[0] + th_x, pos[1] + th_y, pos[0] + th_x + th_to_x, pos[1] + th_y + th_to_y, RED);
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
	
	World my_world{0.15f, 600, 580};

	LineChart chart1{
		10, 90,
		200, 200,
		"Demo Chart",

		-1, -1, 11, 11,

		true, true, 2.0f, 2.0f, 0.0f, 0.0f
	};

	SetTargetFPS(100);

	while(!WindowShouldClose()) {
		frameCount++;

		my_rocket.update(GetFrameTime());
		time += GetFrameTime();

		if(time > 1.f) my_rocket.throttle = 1.0f;

		BeginDrawing();

			// if(frameCount == 1)
			ClearBackground(BLACK);

			initializeLineChart(chart1);

			drawRocket(my_world, my_rocket);

			drawGround(my_world, screenWidth, GREEN);

			DrawText(TextFormat("y pos: %f", my_rocket.pos_y), 10, 10, 20, WHITE);
			DrawText(TextFormat("x pos: %f", my_rocket.pos_x), 10, 30, 20, WHITE);
			DrawText(TextFormat("y velocity: %f", my_rocket.vel_y), 10, 50, 20, WHITE);
			DrawText("fuel: ", 10, 70, 20, WHITE);
			DrawRectangle(60, 75, 100.0f*my_rocket.fuel, 10, BLUE);
			DrawRectangleLines(59, 74, 102, 12, WHITE);

		EndDrawing();
	}
}