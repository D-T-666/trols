#include "rocket.hpp"

struct World {
	float scale;
	int x0, y0, x, y, w, h;
};

void updateWorld(World &wld, Rocket::Rocket rkt);
void drawWorld(World wld, Rocket::Rocket rkt, bool clear = true, bool show_predictions = true);
