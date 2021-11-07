#include "../headers/rocket.h"
#include <math.h>

Rocket::Rocket() {
  set_pos(0, 0);
  set_vel(0, 0);
//   throttle = 1.0f;
}

// Getters and setters

void Rocket::set_pos(float x, float y) {
	pos_x = x;
	pos_y = y;
}

void Rocket::set_vel(float x, float y) {
	vel_x = x;
	vel_y = y;
}

void Rocket::set_theta(float a) {
	theta = a;
}

float Rocket::get_theta() {
	return theta;
}

float Rocket::get_theta_vel() {
	return theta_vel;
}

void Rocket::set_dimensions(float w, float h, float dm, float wm, float t) {
	width = w;
	height = h;
	thruster_distance = -h/2.0f;
	thruster_pos_y = -h/2.0f;
	thruster_pos_x = 0.0f;
	dry_mass = dm;
	wet_mass = wm;
	fuel_mass = wet_mass - dry_mass;
	thrust = t;
}

// Dynamics

float Rocket::mass() {
	return dry_mass + fuel_mass * fuel;
}

float Rocket::inertia() {
	if (_inertia_multiplier == -1.0f)
		_inertia_multiplier = (width * width + height * height) / 12.0f;
	return mass() * _inertia_multiplier;
}

float clip(float a, float b, float c){
	return a < c ? ((a > b) ? a : b) : c;
}

void Rocket::control(float dt) {
	float vel_err = vel_x + clip(PID::get(position_controler, pos_x, dt), -25.0f, 25.0f);
	float theta_err = theta + clip(PID::get(velocity_controler, vel_err, dt), -0.2f, 0.2f);
	thruster_theta = clip(PID::get(attitude_controler, theta_err, dt), -0.2f, 0.2f);
}

void Rocket::update(float dt) {
	control(dt);

	// if (fuel < burn_through_rate * throttle * dt && fuel > 0.0f)
	// 	throttle = fuel / burn_through_rate;
	if (fuel <= 0.05f)
		throttle = 0.0f;

	fuel -= throttle * burn_through_rate * dt;

	// calculate the acceleration from the thruster
	float thrust_x = throttle * thrust * sin(theta + thruster_theta);
	float thrust_y = throttle * thrust * cos(theta + thruster_theta);

	// store old velocity values in temporary
	// variables for calculating the tiny triangle
	float dv_x = vel_x;
	float dv_y = vel_y;

	// apply acceleration
	vel_x += dt * (thrust_x / mass());
	vel_y += dt * (thrust_y / mass() - G);

	// calculate the tiny triangle
	dv_x = (vel_x - dv_x) / 2.0f;
	dv_y = (vel_y - dv_y) / 2.0f;

	// Torque from thruster
	theta_vel += abs(thruster_distance) * throttle * thrust * -sin(thruster_theta) / inertia() * dt;

	pos_x += (vel_x + dv_x) * dt;
	pos_y += (vel_y + dv_y) * dt;

	theta += theta_vel * dt;
}
