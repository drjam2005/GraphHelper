#include <objects.h>
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

bool isInVec(Edge* edge, std::vector<Edge*> list){
	if(list.empty())
		return false;
	for(auto& item : list){
		if(item == edge)
			return true;
	}

	return false;
}


std::vector<Vertex*> getShortestPath(std::vector<Vertex*> passed, Vertex* v1, Vertex* v2){
    if (v1 == v2) {
        passed.push_back(v2);
        return passed;
    }
    if (v1->edges.empty()) {
        return {};
    }

    std::vector<std::vector<Vertex*>> smallest;
    passed.push_back(v1);

    for (auto& edge : v1->edges) {
        if (edge->vertex1 == v1) {
            if (!isInVec(edge->vertex2, passed)) {
                auto branchPassed = passed; // 👈 copy here
                std::vector<Vertex*> thing = getShortestPath(branchPassed, edge->vertex2, v2);
                smallest.push_back(thing);
            }
        }

        if (edge->vertex2 == v1) {
            if (!isInVec(edge->vertex1, passed)) {
                auto branchPassed = passed; // 👈 copy here
                std::vector<Vertex*> thing = getShortestPath(branchPassed, edge->vertex1, v2);
                smallest.push_back(thing);
            }
        }
    }

    if (!smallest.size())
        return {};

    std::vector<Vertex*> smallestPath = smallest[0];
    for (auto& vec : smallest) {
        if (vec.size() < smallestPath.size() && vec.size() != 0) {
            smallestPath = vec;
        }
    }

    if (smallestPath.size())
        return smallestPath;
    return {};
}

