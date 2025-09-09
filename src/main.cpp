#include <iostream>
#include <raylib.h>
#include <math.h>

#define PROJECT_NAME "Graph"
#define WIDTH 800
#define HEIGHT 600

struct Vec2 {
	float x;
	float y;
	Vec2 operator+(Vec2 other){
		return {this->x + other.x, this->y + other.y};
	}
};

void DrawArcBetweenPoints(Vec2 vp1, Vec2 vp2, float radius, Color color) {
	Vector2 p1 = {vp1.x, vp1.y};
	Vector2 p2 = {vp2.x, vp2.y};

    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float d = sqrtf(dx*dx + dy*dy);

    // Infinite radius means just draw a line
    if (radius == 0) {
        DrawLineV(p1, p2, color);
        return;
    }

    // If distance > diameter, no valid circle
    if (d > 2*radius) {
        DrawLineV(p1, p2, RED); // fallback
        return;
    }

    // Midpoint
    Vector2 m = { (p1.x + p2.x)/2, (p1.y + p2.y)/2 };

    // Distance from midpoint to circle center
    float h = sqrtf(radius*radius - (d/2)*(d/2));

    // Perpendicular normalized vector
    Vector2 perp = { -dy/d, dx/d };

    // Choose one of the two possible centers (here: +perp)
    Vector2 center = { m.x + h*perp.x, m.y + h*perp.y };

    // Compute angles
    float startAngle = atan2f(p1.y - center.y, p1.x - center.x) * RAD2DEG;
    float endAngle   = atan2f(p2.y - center.y, p2.x - center.x) * RAD2DEG;

    // Normalize angles to [0, 360)
    if (endAngle < startAngle) endAngle += 360;

    // Draw arc
    float angleStep = (endAngle - startAngle) / (float)40;
    Vector2 prevPoint = {
        center.x + cosf(DEG2RAD*startAngle) * radius,
        center.y + sinf(DEG2RAD*startAngle) * radius
    };

    for (int i = 1; i <= 40; i++) {
        float angle = startAngle + angleStep * i;
        Vector2 nextPoint = {
            center.x + cosf(DEG2RAD*angle) * radius,
            center.y + sinf(DEG2RAD*angle) * radius
        };
        DrawLineV(prevPoint, nextPoint, color);
        prevPoint = nextPoint;
    }
}


int main(int argc, char **argv) {
	InitWindow(WIDTH, HEIGHT, "GraphTheory Helper");
	while(!WindowShouldClose()){
		BeginDrawing();
		DrawFPS(20,20);
		ClearBackground(BLACK);

		Vec2 a = {200, 300};
		Vec2 b = {400, 350};

		DrawArcBetweenPoints(a, b, 150, BLUE);
		DrawArcBetweenPoints(a+Vec2{30,30}, b, 200, RED);

		EndDrawing();
	}
    return 0;
}
