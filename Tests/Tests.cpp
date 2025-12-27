#include "Tests.hpp"

void TestGraph(){ //GROUP 1 тесты методов графа
    ResetTestNumber();
    int result_of_test = 1;

    result_of_test = 1; //TEST 1.1 Проверка пустого графа
    Graph<std::string, int> graph1_1;
    try {
        if (graph1_1.Empty()) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 1.2 Проверка количества рёбер
    try {
        if (graph1_1.EdgeCount() == 0) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 1.3 Проверка количества вершин
    try {
        if (graph1_1.VertexCount() == 0) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 1.4 Добавление 2 вершин
    try {
        graph1_1.AddVertex("test1_4_1");
        graph1_1.AddVertex("test1_4_2");
        if (graph1_1.ContainVertex("test1_4_1") && graph1_1.ContainVertex("test1_4_2")) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 1.5 Добавление ребра
    try {
        auto vertices = graph1_1.GetAllVertices();
        graph1_1.AddEdge(15, vertices["test1_4_1"], vertices["test1_4_2"]);
        if (graph1_1.ContainsEdge("test1_4_1", "test1_4_2")) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 1.6 Удаление ребра
    try {
        auto edges = graph1_1.GetAllEdges();
        graph1_1.DeleteEdge(edges["test1_4_1test1_4_2"]);
        if (!graph1_1.ContainsEdge("test1_4_1", "test1_4_2")) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 1.7 Удаление вершины
    try {
        auto vertices = graph1_1.GetAllVertices();
        graph1_1.DeleteVertex(vertices["test1_4_2"]);
        if (!graph1_1.ContainVertex("test1_4_2")) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 1.8 Очищение всего
    try {
        graph1_1.ClearAll();
        if (graph1_1.Empty()) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();
}

void TestTransportMap(){ //GROUP 2 тесты методов TransportMap
    NextGroup();
    ResetTestNumber();
    int result_of_test = 1;

    result_of_test = 1; //TEST 2.1 Проверка пустого
    TransportMap map1_1({"test2_1"});
    try {
        if (map1_1.HasVertex("idk") == 0) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 2.2 Добавление 2 вершин
    try {
        map1_1.AddVertex("test2_2_1");
        map1_1.AddVertex("test2_2_2");
        if (map1_1.HasVertex("test2_2_1") && map1_1.HasVertex("test2_2_2")) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 0; //TEST 2.3 Добавление ребра
    try {
        map1_1.AddEdge(10, "test2_2_1", "test2_2_2");
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 2.4 Удаление вершины
    try {
        map1_1.DeleteVertex("test2_2_2");
        if (map1_1.HasVertex("test2_2_2") == 0) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();
}

void TestAll() {
    TestGraph();
    TestTransportMap();
}