#include <objects.h>
#include <raylib.h>

class Renderer {
	public:
		void Update();
		void DrawGraph();
		void HandleInput();
	private:
		name_state nameState = HOVER_ONLY;

		std::vector<Vertex*> vertices;
		std::vector<Edge*> edges;
		std::vector<Vertex*> toHighlight = {};

		Vertex* closestVertexFromMouse = nullptr;
		Edge* closestEdgeFromMouse = nullptr;

		Vertex* startingEdge = nullptr;
		Vertex* startingInVertex = nullptr;
        Vertex* inVertex = nullptr;

        Edge* tempEdge1 = nullptr;
        Edge* tempEdge2 = nullptr;

        Vertex* start = nullptr;
        Vertex* end = nullptr;

		bool isStartingEdgeDrawing = false;
		bool isDrawingEdge = false;
		bool isCreatingInVertex = false;
		bool isPlacing = false;
};
