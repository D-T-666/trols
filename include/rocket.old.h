#include "control.h"

class Rocket {
	public:
		float pos_x;				// position
		float pos_y;				// position
		float vel_x;				// previous position
		float vel_y;				// previous position
		float theta;				// rotation
		float theta_vel;				// previous rotation

		float width;
		float height;
		float thruster_pos_x;		// position of the thruster on the rocket
		float thruster_pos_y;		// position of the thruster on the rocket
		float thruster_distance;	// distance from CG to thruster
		float thruster_theta;		// rotation of the thruster relative to the rocket
		float dry_mass;
		float wet_mass;
		float fuel_mass;
		float G = 9.806f;			// gravitational acceleration

		float thrust;				// thrust in  newtons
		float fuel = 1.0f;			// fuel amount in liters
		float burn_through_rate;	// fuel bur through rate in liters per second
		float throttle = 0.0f;		// current throttle level (0.0f to 1.0f)

		PID::PID attitude_controler{1.5f, 0.05f, 4.5f};
		PID::PID position_controler{0.0f, 0.0f, 0.0f};
		PID::PID throttle_controler{0.5f, 0.1f, 3.5f};

	private:
		// Function specific varialbes (super private)
		float _inertia_multiplier = -1.0f;

	private:
		// Updating
		float inertia();
		float mass();
		void control(float dt);

	public:
		Rocket();
		// Getters and setters
		void set_pos(float x, float y);
		void set_vel(float x, float y);
		void set_thruster(float tpx, float tpy, float tt);
		void set_dimensions(float w, float h, float dm, float wm, float f);
		// Updating
		void update(float dt);
};