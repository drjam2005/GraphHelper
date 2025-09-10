#include <renderer.h>
#include <iostream>
#include <algorithm>
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
		if(distanceFromVertex < 20.0f){
			closestVertexFromMouse = vertex;
			DrawCircle(closestVertexFromMouse->pos.x, closestVertexFromMouse->pos.y, 20, BLUE);
		}
	}

	for(auto edge : edges){
		Vec2 tempPos = {mousePos.x, mousePos.y};
		float v1x = edge->vertex1->pos.x; float v2x = edge->vertex2->pos.x;
		float v1y = edge->vertex1->pos.y; float v2y = edge->vertex2->pos.y;

		Vec2 midPoint = {(v1x+v2x)/2.0f,(v1y+v2y)/2.0f};

		float distanceFromMouse = vecDistance(tempPos, midPoint);
		if(distanceFromMouse <= 20.f){
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
		DrawLineEx(pos1, pos2, 5, GRAY);
	}

	// Draw Vertices
	for(auto vertex : vertices){
		DrawCircle(vertex->pos.x, vertex->pos.y, 10, WHITE);
	}
	
	// I know rendering feels backwards, but it looks better
}

void Renderer::HandleInput(){
	// Vertex handling
	if(IsKeyPressed(KEY_D)){
		if(closestVertexFromMouse && !closestEdgeFromMouse){
			std::cout << "Removing Vertex!" << std::endl;
			std::vector<Edge*> edgesToRemove;
			for(auto& edge : edges){
				std::cout << "removing vert" << std::endl;
				if(edge->vertex1 == closestVertexFromMouse){
					edge->vertex1 = nullptr;
					edge->vertex2 = nullptr;
					edgesToRemove.push_back(edge);
				}
				if(edge->vertex2 == closestVertexFromMouse){
					edge->vertex1 = nullptr;
					edge->vertex2 = nullptr;
					edgesToRemove.push_back(edge);
				}
			}

			for(Edge* edge : edgesToRemove)
				edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
			vertices.erase(std::remove(vertices.begin(), vertices.end(), closestVertexFromMouse), vertices.end());
			return;
		}

		if(closestEdgeFromMouse && !closestVertexFromMouse){
			closestEdgeFromMouse->vertex1->edges.erase(std::remove(closestEdgeFromMouse->vertex1->edges.begin(), closestEdgeFromMouse->vertex1->edges.end(), closestEdgeFromMouse), closestEdgeFromMouse->vertex1->edges.end());
			closestEdgeFromMouse->vertex2->edges.erase(std::remove(closestEdgeFromMouse->vertex2->edges.begin(), closestEdgeFromMouse->vertex2->edges.end(), closestEdgeFromMouse), closestEdgeFromMouse->vertex2->edges.end());
			edges.erase(std::remove(edges.begin(), edges.end(), closestEdgeFromMouse), edges.end());
			return;
		}
		return;
	}
	if((closestVertexFromMouse || startingEdge) && !startingInVertex){
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			if(!isStartingEdgeDrawing){
				startingEdge = closestVertexFromMouse;
				isStartingEdgeDrawing = true;
			}
			if(startingEdge)
				DrawLineEx(Vector2{startingEdge->pos.x, startingEdge->pos.y}, GetMousePosition(), 2, RED);
			isDrawingEdge = true;
		}else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && closestVertexFromMouse){
			closestVertexFromMouse->pos = Vec2{GetMousePosition().x, GetMousePosition().y};
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
	}

	if((closestEdgeFromMouse || startingInVertex) && !startingEdge){
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			if(!isCreatingInVertex){
				isCreatingInVertex = true;

				startingInVertex = new Vertex;
				vertices.push_back(startingInVertex);

				Vertex* v1 = closestEdgeFromMouse->vertex1;
				Vertex* v2 = closestEdgeFromMouse->vertex2;

				Edge* newEdge1 = new Edge{v1, startingInVertex};
				Edge* newEdge2 = new Edge{startingInVertex, v2};

				v1->edges.push_back(newEdge1);
				v2->edges.push_back(newEdge2);
				edges.push_back(newEdge1);
				edges.push_back(newEdge2);

				v1->edges.erase(std::remove(v1->edges.begin(), v1->edges.end(), closestEdgeFromMouse), v1->edges.end());
				v2->edges.erase(std::remove(v2->edges.begin(), v2->edges.end(), closestEdgeFromMouse), v2->edges.end());
				edges.erase(std::remove(edges.begin(), edges.end(), closestEdgeFromMouse), edges.end());

				startingInVertex->edges.push_back(newEdge1);
				startingInVertex->edges.push_back(newEdge2);

			}

			Vector2 mousePos = GetMousePosition();
			Vec2 tempPos = {mousePos.x, mousePos.y};
			startingInVertex->pos = tempPos;

			DrawLineEx(GetMousePosition(), Vector2{startingInVertex->pos.x, startingInVertex->pos.y}, 2, RED);
			isCreatingInVertex = true;
		}else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && isCreatingInVertex){
			startingInVertex = nullptr;
			closestEdgeFromMouse = nullptr;
			isCreatingInVertex = false;
		}
	}
}
