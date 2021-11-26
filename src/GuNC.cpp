#include "../include/rocket.hpp"
#include <cmath>
#include <math.h>
#include <iostream>

void Rocket::set_GNC(Rocket &r, int points, float timesteps) {
	r.traj_points = points;
	r.traj_timesteps = timesteps;

	int i = 0;
	for (float gimball = -0.05f; gimball <= 0.05f; gimball += 0.025f) {
		for (float throttle = 0.0f; throttle <= 1.0f; throttle += 0.25f) {
			r.traj_states[i][0] = gimball*2;
			r.traj_states[i][1] = throttle;
			i++;
		}
	}

	r.traj_states_total = i;
}

float Rocket::landing_cost(Rocket &r) {
	float ro = fabs(r.theta);
	float rv = fabs(r.theta_vel);
	// std::cout << rv << "\n";

	// float d = (r.pos_x*r.pos_x + (r.pos_y+r.height/2)*(r.pos_y+r.height/2));
	// float v = sqrt(r.vel_x * r.vel_x + r.vel_y * r.vel_y);

	float thracc = r.throttle*r.thrust*sin(r.theta+r.thruster_theta)/mass(r) - r.G;

	float res = fabs(r.pos_y * r.throttle) * 0.007 + fabs(r.pos_x*0.1) + ro - fabs(r.vel_x*0.05) + fabs(r.vel_y*0.1) - fabs((r.pos_y-r.height/2)*0.1);//v;//ro + fabs(v/1000.0f);
	
	// if (fabs(r.vel_x) < 1 && fabs(r.theta) < 0.1 && fabs(r.theta_vel) < 0.05)
	// 	res = fabs((sqrt(r.vel_y)/thracc)*sqrt(r.vel_y)/2+sqrt(r.pos_y));

	return res;
}

void Rocket::predict(Rocket r, int &depth, int &index, float &best_cost, int &best_cost_idnex) {
	
	update(r, r.traj_timesteps);

	float c;
	int i;

	if (depth > 0) {

		float child_best_cost = 1 << 30;
		int child_best_cost_index;
		int child_depth = depth-1;

		for (int i = 0; i < r.traj_states_total; i++) {
			r.thruster_theta = r.traj_states[i][0];
			r.throttle = r.traj_states[i][1];

			predict(r, child_depth, i, child_best_cost, child_best_cost_index);
		}

		c = child_best_cost;
		i = child_best_cost_index;

	} else {
		
		c = landing_cost(r);
		i = index;
	
	}

	if (c < best_cost) {
		best_cost = c;
		best_cost_idnex = i;
	}

}

void Rocket::foresee(Rocket &ro, int depth) {
	Rocket r = ro;

	ro.traj[0][0] = r.pos_x;
	ro.traj[0][1] = r.pos_y;

	for (int i = 1; i < r.traj_points; i++) {
		float best_cost = 1 << 30;
		int best_cost_index = 0;
		
		predict(r, depth, depth, best_cost, best_cost_index);

		r.thruster_theta = r.traj_states[best_cost_index][0];
		r.throttle = r.traj_states[best_cost_index][1];
		
		update(r, r.traj_timesteps);

		ro.traj[i][0] = r.pos_x;
		ro.traj[i][1] = r.pos_y;

		if (!(i-1)) {
			ro.next_step = best_cost_index;
			// std::cout << best_cost << "\n";
		}
	}
}