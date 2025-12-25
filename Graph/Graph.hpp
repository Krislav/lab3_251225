#include <vector>
#include <algorithm>
#include <unordered_map>
#include "..\Errors\Errors.hpp"

template <class TVertexData, class TEdgeData>
struct Edge;

template <class TVertexData, class TEdgeData>
struct Vertex {
    TVertexData data;
    std::vector<Edge<TVertexData, TEdgeData>*> related_edges;

    Vertex(const TVertexData& value) : data(value) {}
};

template <class TVertexData, class TEdgeData>
struct Edge {
    TEdgeData data;
    Vertex<TVertexData, TEdgeData>* start;
    Vertex<TVertexData, TEdgeData>* finish;

    Edge(const TEdgeData& value, Vertex<TVertexData, TEdgeData>* first, Vertex<TVertexData, TEdgeData>* second)
        : data(value), start(first), finish(second) {}
};

template <class TVertexData, class TEdgeData>
class Graph {
private:
    std::unordered_map<TVertexData, Vertex<TVertexData, TEdgeData>*> vertices;
    std::unordered_map<TVertexData, Edge<TVertexData, TEdgeData>*> edges;

public:

    Graph() = default;
    Graph(const std::vector<TVertexData>& vertices_data) {
        for (const auto& data : vertices_data) {
            AddVertex(data);
        }
    }
    ~Graph() {
        for (auto& pair : edges) delete pair.second;
        for (auto& pair : vertices) delete pair.second;
    }

    void AddVertex(const TVertexData& value) {
        if (vertices.find(value) != vertices.end()) return;

        Vertex<TVertexData, TEdgeData>* new_vertex = new Vertex<TVertexData, TEdgeData>(value);
        vertices[value] = new_vertex;
    }

    void AddEdge(const TEdgeData& value, Vertex<TVertexData, TEdgeData>* first_v, Vertex<TVertexData, TEdgeData>* second_v) {
        if (!first_v || !second_v) throw ErrorCode::NO_VERTEX;

        TVertexData edge_key = first_v->data + second_v->data;
        
        Edge<TVertexData, TEdgeData>* new_edge = new Edge<TVertexData, TEdgeData>(value, first_v, second_v);
        edges[edge_key] = new_edge;
        
        first_v->related_edges.push_back(new_edge);
        second_v->related_edges.push_back(new_edge);
    }

    void DeleteEdge(Edge<TVertexData, TEdgeData>* edge_to_delete) {
        if (!edge_to_delete) throw ErrorCode::NO_EDGE;

        auto& ve_start = edge_to_delete->start->related_edges;
        ve_start.erase(std::remove(ve_start.begin(), ve_start.end(), edge_to_delete), ve_start.end());
        auto& ve_finish = edge_to_delete->finish->related_edges;
        ve_finish.erase(std::remove(ve_finish.begin(), ve_finish.end(), edge_to_delete), ve_finish.end());

        TVertexData edge_key = edge_to_delete->start->data + edge_to_delete->finish->data;
        edges.erase(edge_key);

        delete edge_to_delete;
    }

    void DeleteVertex(Vertex<TVertexData, TEdgeData>* vertex_to_delete) {
        if (!vertex_to_delete) throw ErrorCode::NO_VERTEX;

        std::vector<Edge<TVertexData, TEdgeData>*> to_remove = vertex_to_delete->related_edges;
        for (auto* edge : to_remove) {
            DeleteEdge(edge);
        }

        vertices.erase(vertex_to_delete->data);

        delete vertex_to_delete;
    }

    bool ContainVertex(const TVertexData& vertex_data) const {
        return vertices.find(vertex_data) != vertices.end();
    }

    bool ContainsEdge(const TVertexData& first_vertex_data, const TEdgeData& second_vertex_data) const {
        TVertexData edge_key_1 = first_vertex_data + second_vertex_data;
        TVertexData edge_key_2 = second_vertex_data + first_vertex_data;

        return edges.find(edge_key_1) != edges.end() || edges.find(edge_key_2) != edges.end();
    }

    int VertexCount() const {
        return vertices.size();
    }

    int EdgeCount() const {
        return edges.size();
    }

    bool Empty() const {
        return vertices.empty();
    }

    const std::unordered_map<TVertexData, Vertex<TVertexData, TEdgeData>*> GetAllVertices() const {
        return vertices;
    }

    const std::unordered_map<TVertexData, Edge<TVertexData, TEdgeData>*> GetAllEdges() const {
        return edges;
    }

    void ClearEdges() {
        edges.clear();
    }

    void ClearAll() {
        edges.clear();
        vertices.clear();
    }
};