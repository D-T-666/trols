#include "control.h"

namespace Rocket {
	struct Rocket {
		float pos_x = 0.0f;			// position
		float pos_y = 0.0f;			// position
		float vel_x = 0.0f;			// previous position
		float vel_y = 0.0f;			// previous position
		float theta = 0.0f;			// rotation
		float theta_vel = 0.0f;		// previous rotation

		float width = 0.0f;
		float height = 0.0f;
		float thruster_pos_x = 0.0f;// position of the thruster on the rocket
		float thruster_pos_y = 0.0f;// position of the thruster on the rocket
		float thruster_distance=.0f;// distance from CG to thruster
		float thruster_theta = 0.0f;// rotation of the thruster relative to the rocket
		float p_gm_d = 0.0f; 		// previous gimball delta
		float thruster_max_theta = 80.0 * 3.1415926 / 180.f;
		float dry_mass = 0.0f;
		float wet_mass = 0.0f;
		float fuel_mass = 0.0f;
		float G = 9.806f;			// gravitational acceleration

		float thrust = 0.0f;		// thrust in  newtons
		float fuel = 1.0f;			// fuel amount in liters
		float burn_through_rate;	// fuel bur through rate in liters per second
		float throttle = 0.0f;		// current throttle level (0.0f to 1.0f)
		float p_th_d = 0.0f;		// previous throttle delta

		// Function specific varialbes (super private)
		float _inertia_multiplier = -1.0f;

		// GNC
		int traj_points = 0;
		float traj_timesteps = 1.0f;
		float traj[1024][6];
		int next_step;

		int temp_counter = 0;
	};
	
	// Getters and setters
	void set_pos(Rocket &r, float x, float y);
	void set_vel(Rocket &r, float x, float y);
	void set_thruster(Rocket &r, float tpx, float tpy, float tt);
	void set_dimensions(Rocket &r, float w, float h, float dm, float wm, float f);
	void set_GNC(Rocket &r, int points, float timesteps);

	// Dynamic values
	float inertia(Rocket &r);
	float mass(Rocket &r);

	// Updating
	void control(Rocket &r, float dt);
	void update(Rocket &r, float dt);

	// Metastats
	float landing_cost(Rocket &r);
	void predict(Rocket &ro, Rocket r, int depth, int &index, float &best_cost, int &best_cost_idnex);
	void foresee(Rocket &ro, int depth, bool viz = false);
}