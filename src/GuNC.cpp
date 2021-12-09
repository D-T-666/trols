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

	float res = ro * ro + fabs(ro) * 10 * (fabs(ro) > .2)
				+ rv * rv
				+ 0.1 * fabs(vy)
				+ 0.05 * fabs(vx)
		
				+ 0.1 * fabs(py - r.height / 2)
				+ 100 * (py - r.height / 2 < 0)
				- vx / px * (fabs(px) > 10) + vx / 10 * (fabs(px) < 10)
				+ 0.3 * fabs(px) * clip(10 / fabs(py), 0.5, 3.0)
	;

	return res;
}

int Rocket::predict(Rocket &ro, int depth) {
	float cost = 1 << 30;
	int best_path;

	for (int l = 0; l < (1 << depth) * (1 << depth); l++) {
		Rocket r = ro;
		for (int t = 0; t < depth; t++) {
			int next_step = (l >> t * 2) & 3;

			r.next_step = next_step;
			control(r, r.traj_timesteps);
			update(r, r.traj_timesteps);
		}

		float r_cost = landing_cost(r);
		if (r_cost < cost) {
			cost = r_cost;
			best_path = l;
		}
	}

	return best_path;
}

void Rocket::foresee(Rocket &ro, int depth, bool viz) {
	ro.next_step = predict(ro, depth);

	Rocket r = ro;
	for (int t = 0; t < depth; t++) {
		int next_step = (ro.next_step >> t * 2);

		r.next_step = next_step;
		control(r, r.traj_timesteps);
		update(r, r.traj_timesteps);

		ro.traj[t][0] = r.pos_x;
		ro.traj[t][1] = r.pos_y;
		ro.traj[t][2] = r.theta;
		ro.traj[t][3] = r.thruster_theta;
		ro.traj[t][4] = r.throttle;
		ro.traj[t][5] = clip(landing_cost(r)/10, 0.0, 1.0);
	}
}