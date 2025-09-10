#include <objects.h>
#include <iostream>
#include <algorithm>
#include <vector>

bool isInVec(Vertex* vec, std::vector<Vertex*> list){
	if(list.empty())
		return false;
	for(auto& item : list){
		if(item == vec)
			return true;
	}

	return false;
}

std::vector<Vertex*> getGeodesic(std::vector<Vertex*> passed, Vertex* v1, Vertex* v2){
	if(v1 == v2){
		std::cout << "yipi" << std::endl;
		return passed;
	}
	if(v1->edges.empty()){
		std::cout << "empy" << std::endl;
		return {};
	}

	std::vector<std::vector<Vertex*>> smallest;
	for(auto& edge : v1->edges){
		if(edge->vertex1 == v1){
			if(!isInVec(edge->vertex1, passed)){
				passed.push_back(v1);
				std::vector<Vertex*> thing = (getGeodesic(passed, edge->vertex2, v2));
				smallest.push_back(thing);
			}
		}

		if(edge->vertex2 == v1){
			if(!isInVec(edge->vertex2, passed)){
				passed.push_back(v1);
				std::vector<Vertex*> thing = (getGeodesic(passed, edge->vertex1, v2));
				smallest.push_back(thing);
			}
		}
	}
	if(!smallest.size())
		return {};

	std::vector<Vertex*> smallestPath = smallest[0];
	for(auto& vec : smallest){
		if(vec.size() < smallestPath.size() && vec.size() != 0){
			smallestPath = vec;
		}
	}

	if(smallestPath.size())
		return smallestPath;
	return {};
}
