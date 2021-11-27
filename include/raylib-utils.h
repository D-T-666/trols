#include "raylib.h"

float clip(float a, float b, float c);
float lerp(float a, float b, float t);
void drawLineBounded(int x1, int y1, int x2, int y2, int x_min, int y_min, int w, int h, Color color);