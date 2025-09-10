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

std::vector<Vertex*> getShortestPath(std::vector<Vertex*> passed, Vertex* v1, Vertex* v2) {
    if (v1 == v2) {
        passed.push_back(v2);
        return passed;
    }
    if (v1->edges.empty()) {
        return {};
    }
    std::vector<std::vector<Vertex*>> candidates;
    passed.push_back(v1);

    for (auto& edge : v1->edges) {
        Vertex* next = (edge->vertex1 == v1 ? edge->vertex2 : edge->vertex1);
        if (!isInVec(next, passed)) {
            auto path = getShortestPath(passed, next, v2);
            if (!path.empty()) {
                candidates.push_back(path);
            }
        }
    }

    if (candidates.empty())
        return {};

    auto best = candidates[0];
    for (auto& c : candidates) {
        if (!c.empty() && c.size() < best.size()) {
            best = c;
        }
    }
    return best;
}

