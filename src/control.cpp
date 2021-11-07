#include "../headers/control.h"

float PID::get(PID &pid, float err, float dt) {
	pid.p = pid.kp * err;
	pid.i += pid.ki * err * dt;
	pid.d = pid.kd * (err - pid.p_err) / dt;

	pid.p_err = err;

	return pid.p + pid.i + pid.d;
}