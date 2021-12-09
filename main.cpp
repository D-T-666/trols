#include "include/graphs.hpp"
#include "include/world.hpp"
#include "include/raylib-utils.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <raylib.h>
// #include <raygui.h>
#include <string>


int main() {
	const int screenWidth = 1200;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "demo");

	int frameCount = 0;
	float time = 0.0f;

	Rocket::Rocket my_rocket;
	Rocket::set_dimensions(my_rocket, 2, 8, 90, 100, 2300);
	Rocket::set_thruster(my_rocket, 0, -4, 0.0f);
	my_rocket.burn_through_rate = 0.025f;
	Rocket::set_pos(my_rocket, -10, 200);
	Rocket::set_vel(my_rocket, -10, 0);
	Rocket::set_GNC(my_rocket, 9, 0.2);
	my_rocket.theta = -1.5f;
	
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
	chart::set_params(chart_cost, 1.0f, 2, new Color[2]{{60, 80, 255, 255}, {255, 255, 255, 255}});
	chart::set_guides(chart_cost, 3, 60.0f, 1.0f);
	
	chart::Line chart_throttle;
	chart::set_screen(chart_throttle, 900, 430, 290, 165);
	chart::set_title(chart_throttle, "throttle");
	chart::set_params(chart_throttle, 2.0f, 1, new Color[1]{{255, 80, 60, 255}});
	chart::set_guides(chart_throttle, 3, 60.0f, 0.1f);

	SetTargetFPS(25);

	float foresee_update_timer = 0;

	while(!WindowShouldClose()) {
		frameCount++;

		float time_step = 0.04f; // GetFrameTime(); //
		Rocket::control(my_rocket, time_step);

		Rocket::update(my_rocket, time_step);
		foresee_update_timer += time_step;
		Rocket::foresee(my_rocket, 7, foresee_update_timer >= my_rocket.traj_timesteps);
		if (foresee_update_timer >= my_rocket.traj_timesteps) {
			foresee_update_timer = 0;
		}
		time += time_step;

		chart::line_addData(chart_position, new float[2]{my_rocket.pos_x, my_rocket.pos_y});
		chart::line_addData(chart_velocity, new float[2]{my_rocket.vel_x, my_rocket.vel_y});
		chart::line_addData(chart_pid_attitude, new float[2]{my_rocket.theta, my_rocket.thruster_theta});
		chart::line_addData(chart_cost, new float[2]{Rocket::landing_cost(my_rocket), static_cast<float>(my_rocket.next_step)});
		chart::line_addData(chart_throttle, new float[1]{my_rocket.throttle});

		BeginDrawing();

			if(frameCount == 1) {
				ClearBackground({22,22,22,255});
			}


			DrawRectangle(0, 0, 300, 90, {22,22,22,255});


			// if (frameCount % 20 == 0)
			// 	drawWorld(my_world, my_rocket, false, false);
			updateWorld(my_world, my_rocket);
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

		// break;
	}
}