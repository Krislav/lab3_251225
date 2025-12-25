#include <vector>
#include <algorithm>
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
public:
    std::vector<Vertex<TVertexData, TEdgeData>*> vertices;
    std::vector<Edge<TVertexData, TEdgeData>*> edges;

    Graph() = default;
    Graph(const std::vector<TVertexData>* vertices_data){
        for (const auto& data : vertices_data) {
            AddVertex(data);
        }
    }
    ~Graph() = default;

    void AddVertex(const TVertexData& value) {
        Vertex<TVertexData, TEdgeData>* new_vertex = new Vertex<TVertexData, TEdgeData>(value);
        vertices.push_back(new_vertex);
    }

    void AddEdge(const TEdgeData& value, Vertex<TVertexData, TEdgeData>* first_v, Vertex<TVertexData, TEdgeData>* second_v) {
        if (!first_v || !second_v) throw ErrorCode::NO_VERTEX;

        Edge<TVertexData, TEdgeData>* new_edge = new Edge<TVertexData, TEdgeData>(value, first_v, second_v);
        edges.push_back(new_edge);
        
        first_v->related_edges.push_back(new_edge);
        second_v->related_edges.push_back(new_edge);
    }

    void DeleteEdge(Edge<TVertexData, TEdgeData>* edge_to_delete) {
        if (!edge_to_delete) throw ErrorCode::NO_EDGE;

        auto& ve = edge_to_delete->start->related_edges;
        ve.erase(std::remove(ve.begin(), ve.end(), edge_to_delete), ve.end());
        auto& ve = edge_to_delete->finish->related_edges;
        ve.erase(std::remove(ve.begin(), ve.end(), edge_to_delete), ve.end());

        edges.erase(std::remove(edges.begin(), edges.end(), edge_to_delete), edges.end());

        delete edge_to_delete;
    }

    void DeleteVertex(Vertex<TVertexData, TEdgeData>* vertex_to_delete) {
        if (!vertex_to_delete) throw ErrorCode::NO_VERTEX;

        std::vector<Edge<TVertexData, TEdgeData>*> to_remove = vertex_to_delete->related_edges;
        for (auto* edge : to_remove) {
            DeleteEdge(edge);
        }

        vertices.erase(std::remove(vertices.begin(), vertices.end(), vertex_to_delete), vertices.end());

        delete vertex_to_delete;
    }
};