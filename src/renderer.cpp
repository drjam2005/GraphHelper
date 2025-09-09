#include <renderer.h>
#include <iostream>
#include <math.h>

float vecDistance(const Vec2& v1, const Vec2& v2){
    return sqrt((v2.x - v1.x) * (v2.x - v1.x) +
                (v2.y - v1.y) * (v2.y - v1.y));
}


float verDistance(Vertex& v1, Vertex& v2){
	return vecDistance(v1.pos, v2.pos);
}

void Renderer::Update(){
	Vector2 mousePos = GetMousePosition();
	closestVertexFromMouse = nullptr;
	closestEdgeFromMouse = nullptr;
	
	for(auto vertex : vertices){
		Vec2 tempPos = {mousePos.x, mousePos.y};
		float distanceFromVertex = vecDistance(tempPos, vertex->pos);
		if(distanceFromVertex < 10.0f){
			closestVertexFromMouse = vertex;
			DrawCircle(closestVertexFromMouse->pos.x, closestVertexFromMouse->pos.y, 15, BLUE);
		}
	}

	for(auto edge : edges){
		Vec2 tempPos = {mousePos.x, mousePos.y};
		float v1x = edge->vertex1->pos.x; float v2x = edge->vertex2->pos.x;
		float v1y = edge->vertex1->pos.y; float v2y = edge->vertex2->pos.y;

		Vec2 midPoint = {(v1x+v2x)/2.0f,(v1y+v2y)/2.0f};

		float distanceFromMouse = vecDistance(tempPos, midPoint);
		if(distanceFromMouse <= 10.f){
			closestEdgeFromMouse = edge;
			DrawCircle(midPoint.x, midPoint.y, 15, GREEN);
		}
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && closestVertexFromMouse == nullptr && closestEdgeFromMouse == nullptr){
		Vertex* newVertex = new Vertex;
		newVertex->pos.x = mousePos.x;
		newVertex->pos.y = mousePos.y;
		vertices.push_back(newVertex);
	}
}

void Renderer::DrawGraph(){

	// Draw Edges
	for(auto edge : edges){
		Vector2 pos1, pos2;
		pos1 = {edge->vertex1->pos.x, edge->vertex1->pos.y};
		pos2 = {edge->vertex2->pos.x, edge->vertex2->pos.y};
		DrawLineEx(pos1, pos2, 2, GRAY);
	}

	// Draw Vertices
	for(auto vertex : vertices){
		DrawCircle(vertex->pos.x, vertex->pos.y, 5, WHITE);
	}
	
	// I know rendering feels backwards, but it looks better
}

void Renderer::HandleEdge(){
	if(closestVertexFromMouse || startingEdge){

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			if(!isStartingEdgeDrawing){
				startingEdge = closestVertexFromMouse;
				isStartingEdgeDrawing = true;
			}
			if(startingEdge)
				DrawLineEx(Vector2{startingEdge->pos.x, startingEdge->pos.y}, GetMousePosition(), 2, RED);
			isDrawingEdge = true;
		}

		if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && isDrawingEdge){
			if(startingEdge && closestVertexFromMouse && startingEdge != closestVertexFromMouse){

				for(auto& edge : edges){
					if((edge->vertex1 == startingEdge && edge->vertex2 == closestVertexFromMouse) ||
					   (edge->vertex1 == closestVertexFromMouse && edge->vertex2 == startingEdge)){
						isDrawingEdge = false;
						startingEdge = nullptr;
						return;
					}
				}

				Edge* newEdge = new Edge;
				newEdge->vertex1 = startingEdge;
				newEdge->vertex2 = closestVertexFromMouse;

				edges.push_back(newEdge);
			}

			startingEdge = nullptr;
			isStartingEdgeDrawing = false;
			isDrawingEdge = false;
		}
	}else if(closestEdgeFromMouse || startingInVertex){
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			if(!isCreatingInVertex){
				std::cout << "test" << std::endl;
				startingInVertex = new Vertex;
				isCreatingInVertex = true;
			}

			Vector2 mousePos = GetMousePosition();
			Vec2 tempPos = {mousePos.x, mousePos.y};
			startingInVertex->pos = tempPos;

			DrawLineEx(Vector2{startingInVertex->pos.x, startingInVertex->pos.y}, GetMousePosition(), 2, RED);
			isCreatingInVertex = true;
		}
	}
}
