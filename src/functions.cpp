#include <functions.h>
#include <queue>
#include <limits>
#include <map>

const double doubleINFINITY = std::numeric_limits<double>::max();

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

std::map<Vertex*, std::pair<double, Vertex*>> Dijkstras(std::vector<Vertex*> vertices, Vertex* source) {
    if(source == nullptr)
        return {};
    std::map<Vertex*, std::pair<double, Vertex*>> table;

    for (auto& vertex : vertices)
        table[vertex] = { doubleINFINITY, nullptr };

    table[source] = { 0.0, nullptr };

    using Node = std::pair<double, Vertex*>;

    std::priority_queue<
        Node,
        std::vector<Node>,
        std::greater<Node>
    > q;

    q.push({ 0.0, source });

    while (!q.empty()) {
        Node top = q.top();
        q.pop();

        double currDist = top.first;
        Vertex* curr = top.second;

        if (currDist > table[curr].first) 
            continue;

        for (Edge* e : curr->edges) {
            Vertex* neighbor =
                (e->vertex1 == curr ? e->vertex2 : e->vertex1);

            double newDist = currDist + e->distance;

            if (newDist < table[neighbor].first) {
                table[neighbor].first = newDist;
                table[neighbor].second = curr;

                q.push({ newDist, neighbor });
            }
        }
    }

    return table;
}
