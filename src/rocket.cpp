#include "../include/rocket.hpp"
#include "../include/raylib-utils.h"
#include <math.h>
#include <iostream>

// Getters and setters

void Rocket::set_pos(Rocket &r, float x, float y) {
	r.pos_x = x;
	r.pos_y = y;
}

void Rocket::set_vel(Rocket &r, float x, float y) {
	r.vel_x = x;
	r.vel_y = y;
}

void Rocket::set_dimensions(Rocket &r, float w, float h, float dm, float wm, float t) {
	r.width = w;
	r.height = h;
	r.dry_mass = dm;
	r.wet_mass = wm;
	r.fuel_mass = r.wet_mass - r.dry_mass;
	r.thrust = t;
}

void Rocket::set_thruster(Rocket &r, float tpx, float tpy, float tt){
	r.thruster_pos_y = tpy;
	r.thruster_pos_x = tpx;
	r.thruster_distance = sqrtf(tpx * tpx + tpy * tpy);
}

// Dynamics

float Rocket::mass(Rocket &r) {
	return r.dry_mass + r.fuel_mass * r.fuel;
}

float Rocket::inertia(Rocket &r) {
	if (r._inertia_multiplier == -1.0f)
		r._inertia_multiplier = (r.width * r.width + r.height * r.height) / 12.0f;
	return mass(r) * r._inertia_multiplier;
}

// void Rocket::control(Rocket &r, float dt) {
// 	// position_controler.kp = dry_mass / (fuel+1.0f);
// 	r.thruster_theta	= clip(PID::get(r.attitude_controler, r.theta+r.vel_x*0.1f+r.pos_x*0.02f, dt), -0.05f, 0.05f);

// 	float altitude_err = 4.0f - r.pos_y;
// 	r.throttle = lerp(r.throttle, clip(PID::get(r.throttle_controler, altitude_err, dt), 0.0f, 1.0f), dt*10.f);
// }

void Rocket::control(Rocket &r, float dt) {
	r.thruster_theta = lerp(r.thruster_theta, r.traj_states[r.next_step][0], clip(dt*10.0f, 0.0f, 1.0f));
	r.throttle = lerp(r.throttle, r.traj_states[r.next_step][1], clip(dt*10.0f, 0.0f, 1.0f));
}

void Rocket::update(Rocket &r, float dt) {
	// if (fuel < burn_through_rate * throttle * dt && fuel > 0.0f)
	// 	throttle = fuel / burn_through_rate;
	if (r.fuel <= 0.05f)
		r.throttle = 0.0f;

	r.fuel -= r.throttle * r.burn_through_rate * dt;

	// calculate the acceleration from the thruster
	float thrust_x = r.throttle * r.thrust * sin(r.theta + r.thruster_theta);
	float thrust_y = r.throttle * r.thrust * cos(r.theta + r.thruster_theta);

	// store old velocity values in temporary
	// variables for calculating the tiny triangle
	float dv_x = r.vel_x;
	float dv_y = r.vel_y;

	// apply acceleration
	r.vel_x += dt * (thrust_x / mass(r));
	r.vel_y += dt * (thrust_y / mass(r) - r.G);

	// calculate the tiny triangle
	dv_x = (r.vel_x - dv_x) / 2.0f;
	dv_y = (r.vel_y - dv_y) / 2.0f;

	// Torque from thruster
	r.theta_vel += abs(r.thruster_distance) * r.throttle * r.thrust * -sin(r.thruster_theta) / inertia(r) * dt;

	r.pos_x += (r.vel_x + dv_x) * dt;
	r.pos_y += (r.vel_y + dv_y) * dt;

	r.theta += r.theta_vel * dt;
	if (r.theta > 2*PI) r.theta -= 2*PI;
	if (r.theta < 0) r.theta += 2*PI;
}