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
};

struct Edge {
	Vertex* vertex1;
	Vertex* vertex2;
};
