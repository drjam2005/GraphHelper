#include <objects.h>
#include <cstdint>
#include <vector>
#include <tuple>
#include <unordered_map>

struct tuple_hash {
    std::size_t operator()(const std::tuple<int,uint64_t,int>& t) const noexcept {
        auto h1 = std::hash<int>()(std::get<0>(t));
        auto h2 = std::hash<uint64_t>()(std::get<1>(t));
        auto h3 = std::hash<int>()(std::get<2>(t));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

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

std::vector<Vertex*> getShortestPath(
    std::vector<Vertex*> passed,
    Vertex* v1,
    Vertex* v2
) {
    uint64_t mask = 0;
    for (auto* v : passed) {
        mask |= (1ULL << v->ID);
    }

    static std::unordered_map<std::tuple<int,uint64_t,int>, std::vector<Vertex*>, tuple_hash> memo;

    auto key = std::make_tuple(v1->ID, mask, v2->ID);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }

    if (v1 == v2) {
        passed.push_back(v2);
        memo[key] = passed;
        return passed;
    }
    if (v1->edges.empty()) {
        memo[key] = {};
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

    if (candidates.empty()) {
        memo[key] = {};
        return {};
    }

    auto best = candidates[0];
    for (auto& c : candidates) {
        if (!c.empty() && c.size() < best.size()) {
            best = c;
        }
    }

    memo[key] = best;
    return best;
}
