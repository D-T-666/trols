#pragma once

namespace PID{
	struct PID{
		float kp, ki, kd;
		float p, i, d;
		float p_err;
	};

	float get(PID &pid, float n, float dt);
};
