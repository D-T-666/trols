#include "../include/rocket.hpp"
#include "../include/raylib-utils.h"
#include <cmath>
#include <cstdint>
#include <math.h>
#include <iostream>

void Rocket::set_GNC(Rocket &r, int points, float timesteps) {
	r.traj_points = points;
	r.traj_timesteps = timesteps;

	for (int i = 0; i < points; i++) {
		r.traj[i][0] = r.pos_x;
		r.traj[i][1] = r.pos_y;
	}
}

float Rocket::landing_cost(Rocket &r) {
	float vx, vy, px, py, ro, rv, th, ac;
	vx = r.vel_x; vy = r.vel_y;
	px = r.pos_x; py = r.pos_y;
	ro = r.theta; rv = r.theta_vel;
	th = r.throttle;

	// float res = fabs(r.pos_y * r.throttle) * 0.007 + fabs(r.pos_x*0.1) + ro - fabs(r.vel_x*0.05) + fabs(r.vel_y*0.1) - fabs((r.pos_y-r.height/2)*0.1);//v;//ro + fabs(v/1000.0f);

	float res = ro * ro * rv * rv;

	return res;
}

void Rocket::predict(Rocket &ro, Rocket r, int depth, int &index, float &best_cost, int &best_cost_idnex) {
	float c;
	int i;

	if (depth > 0) {
		float child_best_cost = 1 << 30;
		int child_best_cost_index = 0;

		for (int i = 0; i < 4; i++) {
			Rocket tr = r;
			tr.next_step = i;
			control(tr, tr.traj_timesteps);
			update(tr, tr.traj_timesteps);
			predict(ro, tr, depth - 1, i, child_best_cost, child_best_cost_index);
		}

		c = child_best_cost;
		i = child_best_cost_index;
	} else {
		c = landing_cost(r);
		i = index;

		if (rand() % 10 == 0) {
			ro.traj[ro.temp_counter][0] = r.pos_x;
			ro.traj[ro.temp_counter][1] = r.pos_y;
			ro.traj[ro.temp_counter][2] = r.theta;
			ro.traj[ro.temp_counter][3] = r.thruster_theta;
			ro.traj[ro.temp_counter][4] = r.throttle;
			ro.traj[ro.temp_counter][5] = clip(c/10, 0.0, 1.0);
			ro.temp_counter++;
			ro.temp_counter = ro.temp_counter % 1024;
		}
	}

	if (c < best_cost) {
		best_cost = c;
		best_cost_idnex = i;
	}
}

void Rocket::foresee(Rocket &ro, int depth, bool viz) {
	float best_cost = 1 << 30;
	int best_cost_index = 0;
	
	predict(ro, ro, depth, best_cost_index, best_cost, best_cost_index);

	ro.next_step = best_cost_index;
}