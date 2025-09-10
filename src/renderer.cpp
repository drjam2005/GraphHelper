#include <renderer.h>
#include <algorithm>
#include <math.h>
#include <functions.h>

float vecDistance(const Vec2& v1, const Vec2& v2){
    return sqrt((v2.x - v1.x) * (v2.x - v1.x) +
                (v2.y - v1.y) * (v2.y - v1.y));
}


float verDistance(Vertex& v1, Vertex& v2){
	return vecDistance(v1.pos, v2.pos);
}

void DrawEdgeFromVert(Vertex* v1, Vertex* v2){
	for(auto& edge : v1->edges){
		if(edge->vertex1 == v2 || edge->vertex2 == v2){
			Vector2 pos1, pos2;
			pos1 = {edge->vertex1->pos.x, edge->vertex1->pos.y};
			pos2 = {edge->vertex2->pos.x, edge->vertex2->pos.y};
			DrawLineEx(pos1, pos2, 10, BLUE);
		}
	}
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
			DrawText(TextFormat("%d", closestVertexFromMouse->ID), closestVertexFromMouse->pos.x+10, closestVertexFromMouse->pos.y+10, 25, PURPLE);
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
			DrawText(TextFormat("(%d,%d)", edge->ID.first, edge->ID.second), midPoint.x+10, midPoint.y+10, 25, PURPLE);
		}
	}

	if(nameState == DISPLAY_ALL){
		for(auto& edge : edges){
			float v1x = edge->vertex1->pos.x; float v2x = edge->vertex2->pos.x;
			float v1y = edge->vertex1->pos.y; float v2y = edge->vertex2->pos.y;
			Vec2 midPoint = {(v1x+v2x)/2.0f,(v1y+v2y)/2.0f};
			DrawCircle(midPoint.x, midPoint.y, 10, GREEN);
			DrawText(TextFormat("(%d,%d)", edge->ID.first, edge->ID.second), midPoint.x+10, midPoint.y+10, 40, PURPLE);
		}
		for(auto vertex : vertices){
			DrawCircle(vertex->pos.x, vertex->pos.y, 13, BLUE);
			DrawText(TextFormat("%d", vertex->ID), vertex->pos.x+10, vertex->pos.y+10, 40, PURPLE);
		}
	}
	if(nameState == DISPLAY_ALL_VERTICES){
		for(auto vertex : vertices){
			DrawCircle(vertex->pos.x, vertex->pos.y, 13, BLUE);
			DrawText(TextFormat("%d", vertex->ID), vertex->pos.x+10, vertex->pos.y+10, 40, PURPLE);
		}
	}
	if(nameState == DISPLAY_ALL_EDGES){
		for(auto& edge : edges){
			float v1x = edge->vertex1->pos.x; float v2x = edge->vertex2->pos.x;
			float v1y = edge->vertex1->pos.y; float v2y = edge->vertex2->pos.y;
			Vec2 midPoint = {(v1x+v2x)/2.0f,(v1y+v2y)/2.0f};
			DrawCircle(midPoint.x, midPoint.y, 10, GREEN);
			DrawText(TextFormat("(%d,%d)", edge->ID.first, edge->ID.second), midPoint.x+10, midPoint.y+10, 40, PURPLE);
		}
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && closestVertexFromMouse == nullptr && closestEdgeFromMouse == nullptr){
		int latestID = 0;
		if(vertices.size())
			latestID = vertices[vertices.size()-1]->ID;
		Vertex* newVertex = new Vertex;
		newVertex->pos.x = mousePos.x;
		newVertex->pos.y = mousePos.y;
		newVertex->ID = latestID + 1;
		vertices.push_back(newVertex);
	}
}

void Renderer::DrawGraph(){
	// Draw Edges
	DrawText(TextFormat("E: "), 50, 30, 15, WHITE);
	int yOffset = 50;
	for(auto edge : edges){
		DrawText(TextFormat("%d, %d", edge->ID.first, edge->ID.second), 60, yOffset, 15, WHITE);
		Vector2 pos1, pos2;
		pos1 = {edge->vertex1->pos.x, edge->vertex1->pos.y};
		pos2 = {edge->vertex2->pos.x, edge->vertex2->pos.y};
		DrawLineEx(pos1, pos2, 5, RED);
		yOffset+=15;
	}

	// Highlighted stuff
	if(toHighlight.size() > 0){
		int xOffset = 50;
		DrawText(TextFormat("Highlighted Path: "), 10, 470, 20, WHITE);
		for(size_t i = 0; i < toHighlight.size()-1; ++i){
			DrawEdgeFromVert(toHighlight[i], toHighlight[i+1]);
			DrawText(TextFormat("%d,",toHighlight[i]->ID ), xOffset, 500, 20, WHITE);
			xOffset+=19;
		}
		DrawText(TextFormat("%d",toHighlight[toHighlight.size()-1]->ID ), xOffset, 500, 20, WHITE);
	}

	// Draw Vertices
	DrawText(TextFormat("V: "), 10, 30, 15, WHITE);
	yOffset = 50;
	for(auto vertex : vertices){
		DrawText(TextFormat("%d", vertex->ID), 30, yOffset, 15, WHITE);
		DrawCircle(vertex->pos.x, vertex->pos.y, 10, WHITE);
		yOffset += 15;
	}

	// I know rendering feels backwards, but it looks better
}

void Renderer::HandleInput(){
	if(IsKeyPressed(KEY_S)){
		toHighlight = getShortestPath({}, vertices[0], vertices[1]);
	}
	if(IsKeyPressed(KEY_G)){
		toHighlight = getShortestPath({}, vertices[0], vertices[1]);
		for(size_t i = 0; i < vertices.size(); ++i){
			for(size_t j = 0; j < vertices.size(); ++j){
				if(i == j) continue;
				std::vector<Vertex*> temp = getShortestPath({}, vertices[i], vertices[j]);
				if(temp.size() > toHighlight.size()){
					toHighlight = temp;
				}
			}
		}
	}
	if(IsKeyPressed(KEY_N)){
		if(nameState == HOVER_ONLY){
			nameState = DISPLAY_ALL_VERTICES;
		}else if(nameState == DISPLAY_ALL_VERTICES){
			nameState = DISPLAY_ALL_EDGES;
		}else if(nameState == DISPLAY_ALL_EDGES){
			nameState = DISPLAY_ALL;
		}else if(nameState == DISPLAY_ALL){
			nameState = HOVER_ONLY;
		}
	}

	// Vertex handling
	if(IsKeyPressed(KEY_D)){
		toHighlight = {};
		if(closestVertexFromMouse && !closestEdgeFromMouse){
			std::vector<Edge*> edgesToRemove;
			for(auto& edge : edges){
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

			for(Edge* edge : edgesToRemove){
				for(Vertex* ver : vertices){
					for(Edge* verEdge : ver->edges){
						if(verEdge == edge){
							ver->edges.erase(std::remove(ver->edges.begin(), ver->edges.end(), edge), ver->edges.end());
							break;
						}
					}
				}
				edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
			}

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

	// Mouse input
	if((closestVertexFromMouse || startingEdge) && !startingInVertex){
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			toHighlight = {};
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
						isStartingEdgeDrawing = false;
						startingEdge = nullptr;
						return;
					}
				}

				Edge* newEdge = new Edge;
				startingEdge->edges.push_back(newEdge);
				closestVertexFromMouse->edges.push_back(newEdge);
				newEdge->vertex1 = startingEdge;
				newEdge->vertex2 = closestVertexFromMouse;
				newEdge->ID = {startingEdge->ID, closestVertexFromMouse->ID};
				edges.push_back(newEdge);
			}

			startingEdge = nullptr;
			closestVertexFromMouse = nullptr;
			isStartingEdgeDrawing = false;
			isDrawingEdge = false;
		}
	}

	if((closestEdgeFromMouse || startingInVertex) && !startingEdge){
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			toHighlight = {};
			if(!isCreatingInVertex){
				isCreatingInVertex = true;

				startingInVertex = new Vertex;
				int latestID = vertices[vertices.size()-1]->ID;
				startingInVertex->ID = latestID + 1;
				vertices.push_back(startingInVertex);

				Vertex* v1 = closestEdgeFromMouse->vertex1;
				Vertex* v2 = closestEdgeFromMouse->vertex2;

				Edge* newEdge1 = new Edge{v1, startingInVertex};
				newEdge1->ID.first  = v1->ID;
				newEdge1->ID.second = startingInVertex->ID;

				Edge* newEdge2 = new Edge{startingInVertex, v2};
				newEdge2->ID.first = startingInVertex->ID;
				newEdge2->ID.second  = v2->ID;

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
			closestEdgeFromMouse = nullptr;
		}else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && isCreatingInVertex){
			startingInVertex = nullptr;
			closestEdgeFromMouse = nullptr;
			isCreatingInVertex = false;
		}
	}
}
