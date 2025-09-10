#include <objects.h>
#include <raylib.h>

class Renderer {
	public:
		void Update();
		void DrawGraph();
		void HandleInput();
	private:
		std::vector<Vertex*> vertices;
		std::vector<Edge*> edges;
		Vertex* closestVertexFromMouse = nullptr;
		Edge* closestEdgeFromMouse = nullptr;
		Vertex* startingEdge = nullptr;
		Vertex* startingInVertex = nullptr;
		Vertex* inVertex = nullptr;
		bool isStartingEdgeDrawing = false;
		bool isDrawingEdge = false;
		bool isCreatingInVertex = false;
		bool isPlacing = false;
};
