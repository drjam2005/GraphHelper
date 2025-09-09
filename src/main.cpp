#include <iostream>
#include <raylib.h>
#include <math.h>

#define PROJECT_NAME "Graph"
#define WIDTH 800
#define HEIGHT 600

int main(int argc, char **argv) {
	InitWindow(WIDTH, HEIGHT, "GraphTheory Helper");
	while(!WindowShouldClose()){
		BeginDrawing();
		DrawFPS(20,20);
		ClearBackground(BLACK);

		EndDrawing();
	}
    return 0;
}
