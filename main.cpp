#include "headers/raylib-utils.h"
#include "headers/rocket.h"
#include "headers/graphs.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <raylib.h>
// #include <raygui.h>
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

void drawWorld(World wld, Rocket rkt, bool clear = true) {
	if (clear) DrawRectangle(wld.pos_x, wld.pos_y, wld.w, wld.h, {22,22,22,255});
	DrawRectangleLines(wld.pos_x-1, wld.pos_y-1, wld.w+2, wld.h+2,WHITE);
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

	drawPolyBounded(xs, ys, 4, wld,WHITE);

	float th_x = rkt.thruster_pos_y / wld.scale *  sin(rkt.theta) + rkt.thruster_pos_x / wld.scale * cos(rkt.theta);
	float th_y = rkt.thruster_pos_y / wld.scale * -cos(rkt.theta) + rkt.thruster_pos_x / wld.scale * sin(rkt.theta);

	std::cout << rkt.throttle << "\n";
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
	my_rocket.set_dimensions(2, 8, 90, 100, 2300);
	my_rocket.set_thruster(0, -4, 0.0f);
	my_rocket.thruster_theta = 0.0f;
	my_rocket.burn_through_rate = 0.025f;
	my_rocket.set_pos(0, 200);
	my_rocket.theta = 1.0f;
	
	World my_world{0.5f, 290, 560, 310, 10, 580, 580};

	chart::Line chart_position;
	chart::set_screen(chart_position, 10, 320, 290, 270);
	chart::set_title(chart_position, "position");
	chart::set_params(chart_position, -1.0f, -51.0f, 21.0f, 51.0f);
	chart::set_guides(chart_position, 3, 2.0f, 5.0f, 0.0f, 0.0f);
	
	chart::Line chart_velocity;
	chart::set_screen(chart_velocity, 10, 110, 290, 180);
	chart::set_title(chart_velocity, "velocity");
	chart::set_params(chart_velocity, -1.0f, -40.0f, 21.0f, 40.0f);
	chart::set_guides(chart_velocity, 3, 2.0f, 5.0f, 0.0f, 0.0f);

	chart::Line chart_pid_attitude;
	chart::set_screen(chart_pid_attitude, 900, 30, 290, 165);
	chart::set_title(chart_pid_attitude, "pid - attitude");
	chart::set_params(chart_pid_attitude, -1.0f, -1.0f, 21.0f, 1.0f);
	chart::set_guides(chart_pid_attitude, 3, 2.0f, 5.0f, 0.0f, 0.0f);

	chart::Line chart_pid_throttle;
	chart::set_screen(chart_pid_throttle, 900, 230, 290, 165);
	chart::set_title(chart_pid_throttle, "pid - throttle");
	chart::set_params(chart_pid_throttle, -1.0f, -100.0f, 21.0f, 100.0f);
	chart::set_guides(chart_pid_throttle, 3, 2.0f, 5.0f, 0.0f, 0.0f);
	
	chart::Line chart_pid_position;
	chart::set_screen(chart_pid_position, 900, 430, 290, 165);
	chart::set_title(chart_pid_position, "pid - lateral");
	chart::set_params(chart_pid_position, -1.0f, -10.0f, 21.0f, 10.0f);
	chart::set_guides(chart_pid_position, 3, 2.0f, 5.0f, 0.0f, 0.0f);

	SetTargetFPS(60);

	while(!WindowShouldClose()) {
		frameCount++;

		my_rocket.update(GetFrameTime());
		time += GetFrameTime();

		// if(time > 2.f) my_rocket.throttle = 1.0f;

		BeginDrawing();

			if(frameCount == 1) {
				ClearBackground({22,22,22,255});

				initializeLineChart(chart_position);
				initializeLineChart(chart_velocity);
				initializeLineChart(chart_pid_attitude);
				initializeLineChart(chart_pid_throttle);
				initializeLineChart(chart_pid_position);
			}


			DrawRectangle(0, 0, 400, 90, {22,22,22,255});

			plotLineChart(
				chart_position,
				new float[2]{time, time},
				new float[2]{my_rocket.pos_x, my_rocket.pos_y-200},
				new Color[2]{{255, 80, 60, 255}, GREEN},
				2);
			plotLineChart(
				chart_velocity,
				new float[2]{time, time},
				new float[2]{my_rocket.vel_x, my_rocket.vel_y},
				new Color[2]{{255, 80, 60, 255}, GREEN},
				2);
			plotLineChart(
				chart_pid_attitude,
				new float[3]{time, time, time},
				new float[3]{my_rocket.attitude_controler.p, my_rocket.attitude_controler.i, my_rocket.attitude_controler.d},
				new Color[3]{{255, 80, 60, 255}, GREEN, {60, 80, 255, 255}},
				3);
			plotLineChart(
				chart_pid_throttle,
				new float[3]{time, time, time},
				new float[3]{my_rocket.throttle_controler.p, my_rocket.throttle_controler.i, my_rocket.throttle_controler.d},
				new Color[3]{{255, 80, 60, 255}, GREEN, {60, 80, 255, 255}},
				3);
			plotLineChart(
				chart_pid_position,
				new float[3]{time, time, time},
				new float[3]{my_rocket.position_controler.p, my_rocket.position_controler.i, my_rocket.position_controler.d},
				new Color[3]{{255, 80, 60, 255}, GREEN, {60, 80, 255, 255}},
				3);

			drawWorld(my_world, my_rocket);

			// DrawFPS(10, 10);
			DrawText(TextFormat("y pos: %f", my_rocket.pos_y), 10, 10, 20,WHITE);
			DrawText(TextFormat("x pos: %f", my_rocket.pos_x), 10, 30, 20,WHITE);
			DrawText(TextFormat("y velocity: %f", my_rocket.vel_y), 10, 50, 20,WHITE);
			DrawText("fuel: ", 10, 70, 20,WHITE);
			DrawRectangle(60, 75, 100.0f*my_rocket.fuel, 10, BLUE);
			DrawRectangleLines(59, 74, 102, 12,WHITE);

		EndDrawing();
	}
}