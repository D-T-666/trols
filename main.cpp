#include "include/raylib-utils.h"
#include "include/rocket.hpp"
#include "include/graphs.hpp"
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

void drawWorld(World wld, Rocket::Rocket rkt, bool clear = true) {
	if (clear) DrawRectangle(wld.pos_x, wld.pos_y, wld.w, wld.h, {22,22,22,255});
	DrawRectangleLines(wld.pos_x-1, wld.pos_y-1, wld.w+2, wld.h+2,WHITE);
	float pos[2] = {
		wld.pos_x + wld.x0 + rkt.pos_x * wld.scale,
		wld.pos_y + wld.y0 - rkt.pos_y * wld.scale
	};
	
	float up[2] = {
		rkt.height * 0.5f * wld.scale * -sin(rkt.theta),
		rkt.height * 0.5f * wld.scale * cos(rkt.theta)
	};

	float right[2] = {
		rkt.width * 0.5f * wld.scale * cos(rkt.theta),
		rkt.width * 0.5f * wld.scale * sin(rkt.theta)
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

	float th_x = rkt.thruster_pos_y * wld.scale *  sin(rkt.theta) + rkt.thruster_pos_x * wld.scale * cos(rkt.theta);
	float th_y = rkt.thruster_pos_y * wld.scale * -cos(rkt.theta) + rkt.thruster_pos_x * wld.scale * sin(rkt.theta);

	float th_to_x = -sin(rkt.thruster_theta + rkt.theta) * rkt.height * rkt.throttle * wld.scale;
	float th_to_y =  cos(rkt.thruster_theta + rkt.theta) * rkt.height * rkt.throttle * wld.scale;
	drawLineBounded(pos[0] + th_x, pos[1] + th_y, pos[0] + th_x + th_to_x, pos[1] + th_y + th_to_y, wld.pos_x, wld.pos_y, wld.w, wld.h, RED);
	
	// Trajectory

	for (int i = 0; i < rkt.traj_points; i++) {
		drawLineBounded(
			wld.pos_x + wld.x0 + rkt.traj[i][0] * wld.scale - rkt.height * sin(rkt.traj[i][2]) * wld.scale / 2, 
			wld.pos_y + wld.y0 - rkt.traj[i][1] * wld.scale - rkt.height * -cos(rkt.traj[i][2]) * wld.scale / 2,
			wld.pos_x + wld.x0 + rkt.traj[i][0] * wld.scale + rkt.height * sin(rkt.traj[i][2]) * wld.scale / 2, 
			wld.pos_y + wld.y0 - rkt.traj[i][1] * wld.scale + rkt.height * -cos(rkt.traj[i][2]) * wld.scale / 2,
			wld.pos_x, wld.pos_y, wld.w, wld.h, {60, 220, 255, 40});
		drawLineBounded(
			wld.pos_x + wld.x0 + rkt.traj[i][0] * wld.scale - rkt.height * sin(rkt.traj[i][2]) * wld.scale / 2, 
			wld.pos_y + wld.y0 - rkt.traj[i][1] * wld.scale - rkt.height * -cos(rkt.traj[i][2]) * wld.scale / 2,
			wld.pos_x + wld.x0 + rkt.traj[i][0] * wld.scale - rkt.height * sin(rkt.traj[i][2]) * wld.scale / 2 + rkt.height * -sin(rkt.traj[i][2] + rkt.traj[i][3]) * rkt.traj[i][4] * wld.scale,
			wld.pos_y + wld.y0 - rkt.traj[i][1] * wld.scale - rkt.height * -cos(rkt.traj[i][2]) * wld.scale / 2 + rkt.height * cos(rkt.traj[i][2] + rkt.traj[i][3]) * rkt.traj[i][4] * wld.scale,
			wld.pos_x, wld.pos_y, wld.w, wld.h, {255, 110, 20, 40});
	}

	// 
	
	DrawLine(wld.pos_x, wld.pos_y+wld.y0, wld.pos_x+wld.w, wld.pos_y+wld.y0, GREEN);
}

int main() {
	const int screenWidth = 1200;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "demo");

	int frameCount = 0;
	float time = 0.0f;

	Rocket::Rocket my_rocket;
	Rocket::set_dimensions(my_rocket, 2, 8, 90, 100, 2300);
	Rocket::set_thruster(my_rocket, 0, -4, 0.0f);
	my_rocket.thruster_theta = 0.0f;
	my_rocket.burn_through_rate = 0.025f;
	Rocket::set_pos(my_rocket, 0, 200);
	// Rocket::set_vel(my_rocket, 0, -50);
	Rocket::set_GNC(my_rocket, 25, 0.25);
	my_rocket.theta = 1.0f;
	
	World my_world{2.0f, 290, 560, 310, 10, 580, 580};

	chart::Line chart_position;
	chart::set_screen(chart_position, 10, 320, 290, 270);
	chart::set_title(chart_position, "position");
	chart::set_params(chart_position, 1.0f, 2, new Color[2]{{255, 80, 60, 255}, GREEN});
	chart::set_guides(chart_position, 3, 60.0f, 25.0f);
	
	chart::Line chart_velocity;
	chart::set_screen(chart_velocity, 10, 110, 290, 180);
	chart::set_title(chart_velocity, "velocity");
	chart::set_params(chart_velocity, 1.0f, 2, new Color[2]{{255, 80, 60, 255}, GREEN});
	chart::set_guides(chart_velocity, 3, 60.0f, 25.0f);

	chart::Line chart_pid_attitude;
	chart::set_screen(chart_pid_attitude, 900, 30, 290, 165);
	chart::set_title(chart_pid_attitude, "attitude");
	chart::set_params(chart_pid_attitude, 1.0f, 2, new Color[2]{{255, 80, 60, 255}, GREEN});
	chart::set_guides(chart_pid_attitude, 3, 60.0f, 25.0f);

	chart::Line chart_cost;
	chart::set_screen(chart_cost, 900, 230, 290, 165);
	chart::set_title(chart_cost, "landing cost");
	chart::set_params(chart_cost, 1.0f, 1, new Color[1]{{60, 80, 255, 255}});
	chart::set_guides(chart_cost, 3, 60.0f, 25.0f);
	
	chart::Line chart_throttle;
	chart::set_screen(chart_throttle, 900, 430, 290, 165);
	chart::set_title(chart_throttle, "throttle");
	chart::set_params(chart_throttle, 2.0f, 1, new Color[1]{{255, 80, 60, 255}});
	chart::set_guides(chart_throttle, 3, 60.0f, 0.1f);

	SetTargetFPS(25);

	float foresee_update_timer = 0;

	while(!WindowShouldClose()) {
		frameCount++;

		Rocket::control(my_rocket, 0.04);
		Rocket::update(my_rocket, 0.04);
		foresee_update_timer += 0.04f;
		Rocket::foresee(my_rocket, 3, foresee_update_timer >= my_rocket.traj_timesteps);
		if (foresee_update_timer >= my_rocket.traj_timesteps) {
			foresee_update_timer = 0;
		}
		time += 0.04;

		chart::line_addData(chart_position, new float[2]{my_rocket.pos_x, my_rocket.pos_y});
		chart::line_addData(chart_velocity, new float[2]{my_rocket.vel_x, my_rocket.vel_y});
		chart::line_addData(chart_pid_attitude, new float[2]{my_rocket.theta, my_rocket.thruster_theta});
		chart::line_addData(chart_cost, new float[1]{Rocket::landing_cost(my_rocket)});
		chart::line_addData(chart_throttle, new float[1]{my_rocket.throttle});

		BeginDrawing();

			if(frameCount == 1) {
				ClearBackground({22,22,22,255});
			}


			DrawRectangle(0, 0, 400, 90, {22,22,22,255});

			drawWorld(my_world, my_rocket);

			chart::line_display(chart_position);
			chart::line_display(chart_velocity);
			chart::line_display(chart_pid_attitude);
			chart::line_display(chart_cost);
			chart::line_display(chart_throttle);

			DrawText(TextFormat("y pos: %f", my_rocket.pos_y), 10, 10, 20,WHITE);
			DrawText(TextFormat("x pos: %f", my_rocket.pos_x), 10, 30, 20,WHITE);
			DrawText(TextFormat("y velocity: %f", my_rocket.vel_y), 10, 50, 20,WHITE);
			DrawText("fuel: ", 10, 70, 20,WHITE);
			DrawRectangle(60, 75, 100.0f*my_rocket.fuel, 10, BLUE);
			DrawRectangleLines(59, 74, 102, 12,WHITE);

		EndDrawing();
	}
}