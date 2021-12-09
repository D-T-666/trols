#include "../include/raylib-utils.h"
#include "../include/world.hpp"
#include <math.h>

void updateWorld(World &wld, Rocket::Rocket rkt) {
	wld.x0 = wld.w / 2 - rkt.pos_x / 2;
}

void drawWorld(World wld, Rocket::Rocket rkt, bool clear, bool show_predictions) {
	if (clear) DrawRectangle(wld.x, wld.y, wld.w, wld.h, {22,22,22,255});
	DrawRectangleLines(wld.x-1, wld.y-1, wld.w+2, wld.h+2,WHITE);
	float pos[2] = {
		wld.x + wld.x0 + rkt.pos_x * wld.scale,
		wld.y + wld.y0 - rkt.pos_y * wld.scale
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

	drawPolyBounded(xs, ys, 4, wld.x, wld.y, wld.w, wld.h, {255, 255, 255, 127});

	float th_x = rkt.thruster_pos_y * wld.scale *  sin(rkt.theta) + rkt.thruster_pos_x * wld.scale * cos(rkt.theta);
	float th_y = rkt.thruster_pos_y * wld.scale * -cos(rkt.theta) + rkt.thruster_pos_x * wld.scale * sin(rkt.theta);

	float th_to_x = -sin(rkt.thruster_theta + rkt.theta) * rkt.height * rkt.throttle * wld.scale;
	float th_to_y =  cos(rkt.thruster_theta + rkt.theta) * rkt.height * rkt.throttle * wld.scale;
	drawLineBounded(pos[0] + th_x, pos[1] + th_y, pos[0] + th_x + th_to_x, pos[1] + th_y + th_to_y, wld.x, wld.y, wld.w, wld.h, RED);
	
	// Trajectory

	for (int i = 0; i < rkt.traj_points && show_predictions; i++) {
		drawLineBounded(
			wld.x + wld.x0 + rkt.traj[i][0] * wld.scale - rkt.height * sin(rkt.traj[i][2]) * wld.scale / 2, 
			wld.y + wld.y0 - rkt.traj[i][1] * wld.scale - rkt.height * -cos(rkt.traj[i][2]) * wld.scale / 2,
			wld.x + wld.x0 + rkt.traj[i][0] * wld.scale + rkt.height * sin(rkt.traj[i][2]) * wld.scale / 2, 
			wld.y + wld.y0 - rkt.traj[i][1] * wld.scale + rkt.height * -cos(rkt.traj[i][2]) * wld.scale / 2,
			wld.x, wld.y, wld.w, wld.h, {static_cast<unsigned char>(255 * rkt.traj[i][5]), static_cast<unsigned char>(255 - 255 * rkt.traj[i][5]), 0, 2});
		drawLineBounded(
			wld.x + wld.x0 + rkt.traj[i][0] * wld.scale - rkt.height * sin(rkt.traj[i][2]) * wld.scale / 2, 
			wld.y + wld.y0 - rkt.traj[i][1] * wld.scale - rkt.height * -cos(rkt.traj[i][2]) * wld.scale / 2,
			wld.x + wld.x0 + rkt.traj[i][0] * wld.scale - rkt.height * sin(rkt.traj[i][2]) * wld.scale / 2 + rkt.height * -sin(rkt.traj[i][2] + rkt.traj[i][3]) * rkt.traj[i][4] * wld.scale,
			wld.y + wld.y0 - rkt.traj[i][1] * wld.scale - rkt.height * -cos(rkt.traj[i][2]) * wld.scale / 2 + rkt.height * cos(rkt.traj[i][2] + rkt.traj[i][3]) * rkt.traj[i][4] * wld.scale,
			wld.x, wld.y, wld.w, wld.h, {255, 255, 20, 2});
	}

	// 
	int landing_pad_width = 20; // meters
	DrawLine(wld.x, wld.y+wld.y0, wld.x+wld.w, wld.y+wld.y0, GREEN);
	DrawLine(wld.x + wld.x0 - landing_pad_width / 2, wld.y + wld.y0 - 1, wld.x + wld.x0 + landing_pad_width/2, wld.y + wld.y0 - 1, {255, 255, 255, 255});
}