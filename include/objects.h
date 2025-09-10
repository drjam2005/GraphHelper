#include <vector>

struct Vertex;
struct Edge;
struct Vec2 {
	float x;
	float y;
};

struct Vertex {
	Vec2 pos;
	std::vector<Edge*> edges;
	int ID = 0;
};

struct Edge {
	Vertex* vertex1;
	Vertex* vertex2;
	std::pair<int, int> ID;
};
