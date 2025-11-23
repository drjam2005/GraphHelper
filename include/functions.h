#include <objects.h>
#include <cstdint>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <map>

struct tuple_hash {
    std::size_t operator()(const std::tuple<int,uint64_t,int>& t) const noexcept {
        auto h1 = std::hash<int>()(std::get<0>(t));
        auto h2 = std::hash<uint64_t>()(std::get<1>(t));
        auto h3 = std::hash<int>()(std::get<2>(t));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

bool isInVec(Vertex* vec, std::vector<Vertex*> list);

bool isInVec(Edge* edge, std::vector<Edge*> list);

std::vector<Vertex*> getShortestPath( std::vector<Vertex*> passed, Vertex* v1, Vertex* v2);

std::map<Vertex*, std::pair<double, Vertex*>> Dijkstras(std::vector<Vertex*> vertices, Vertex* source);

