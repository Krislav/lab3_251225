#include <string>
#include <random>
#include <queue>
#include <map>
#include <limits>
#include "..\Graph\Graph.hpp"

struct Path {
    std::vector<std::string> verts;
    std::vector<int> distance;
    int sum_distance = 0;   

    Path() = default;
};

class TransportMap {
private:
    Graph<std::string, int>* Map;

public:
    TransportMap(const std::vector<std::string>& places) {
        Map = new Graph<std::string, int>(places);
    }

    ~TransportMap () {
        delete Map;
    }

    void AddVertex(std::string name) {
        Map->AddVertex(name);
    }

    void AddEdge(const int& distance, const std::string& start, const std::string& finish) {
        auto vertices = Map->GetAllVertices();
        Map->AddEdge(distance, vertices.at(start), vertices.at(finish));
    }

    bool HasVertex(const std::string& name) {
        return Map->ContainVertex(name);
    }

    Vertex<std::string, int>* GetVertexPtr(const std::string& name) {
        auto all_vertices = Map->GetAllVertices();
        if (all_vertices.find(name) != all_vertices.end()) return all_vertices.at(name);
        return nullptr;
    }

    Path DijkstraAlgorithm(const std::string& start, const std::string& finish) {
        if (!Map->ContainVertex(start) || !Map->ContainVertex(finish)) throw ErrorCode::VERTEX_DOESNT_EXIST;

        std::unordered_map<std::string, int> distances;
        std::unordered_map<std::string, std::string> previous;
        auto all_vertices = Map->GetAllVertices();
        
        for (auto const& [name, ptr] : all_vertices) {
            distances[name] = std::numeric_limits<int>::max();
        }
        
        distances[start] = 0;

        using Node = std::pair<int, std::string>;
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            int d = pq.top().first;
            std::string u_name = pq.top().second;
            pq.pop();

            if (d > distances[u_name]) continue;
            if (u_name == finish) break;

            auto u_vertex = all_vertices.at(u_name);

            for (auto* edge : u_vertex->related_edges) {
                Vertex<std::string, int>* neighbor_ptr = (edge->start->data == u_name) ? edge->finish : edge->start;
                std::string v_name = neighbor_ptr->data;
                int weight = edge->data;

                if (distances[u_name] + weight < distances[v_name]) {
                    distances[v_name] = distances[u_name] + weight;
                    previous[v_name] = u_name;
                    pq.push({distances[v_name], v_name});
                }
            }
        }

        Path result;
        if (distances[finish] == std::numeric_limits<int>::max()) {
            return result;
        }

        result.sum_distance = distances[finish];
        
        std::string current = finish;
        while (current != start) {
            result.verts.insert(result.verts.begin(), current);
            int travel_time = distances[current] - distances[previous[current]];
            result.distance.insert(result.distance.begin(), travel_time);
            current = previous[current];
        }
        result.verts.insert(result.verts.begin(), start);

        return result;
    }

    void GenerateRandomEdges() {
        auto all_vertices_map = Map->GetAllVertices();
        std::vector<Vertex<std::string, int>*> vertex_names_list;
        
        for (auto const& [name, vertex_ptr] : all_vertices_map) {
            vertex_names_list.push_back(vertex_ptr);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> weight_dist(1, 20);
        std::uniform_int_distribution<> chance_dist(1, 100);

        for (size_t i = 0; i < vertex_names_list.size(); ++i) {
            for (size_t j = i + 1; j < vertex_names_list.size(); ++j) {
                if (chance_dist(gen) <= 30) {
                    int random_weight = weight_dist(gen);
                    Map->AddEdge(random_weight, vertex_names_list[i], vertex_names_list[j]);
                }
            }
        }
    }

    const std::unordered_map<std::string, Vertex<std::string, int>*> GetAllVertices() const {
        return Map->GetAllVertices();
    }

    const std::unordered_map<std::string, Edge<std::string, int>*> GetAllEdges() const {
        return Map->GetAllEdges();
    }

    void Clear() {
        Map->ClearAll();
    }
    
    void ClearEdges() {
        auto all_vertices = Map->GetAllVertices();
        for (auto const& [name, ptr] : all_vertices) {
            ptr->related_edges.clear();
        }
        Map->ClearEdges();
    }
};