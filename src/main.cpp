#include <iostream>
#include <raylib.h>
#include <math.h>
#include <renderer.h>

#define PROJECT_NAME "Graph"
#define WIDTH 800
#define HEIGHT 600

int main(int argc, char **argv) {
	InitWindow(WIDTH, HEIGHT, "GraphTheory Helper");
	Renderer window;
	while(!WindowShouldClose()){
		BeginDrawing();
		DrawFPS(20,20);
		ClearBackground(BLACK);

		window.HandleInput();
		window.Update();
		window.DrawGraph();

		EndDrawing();
	}
    return 0;
}
