#include <windows.h>
#include <commdlg.h>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "TransportMap/TransportMap.hpp" 

#define ID_BTN_LOAD_FILE 101
#define ID_BTN_CALC_PATH 102
#define ID_BTN_GEN_RANDOM 103
#define ID_BTN_CLEAR_EDGES 104
#define ID_BTN_CLEAR_ALL 105
#define ID_BTN_ADD_VERTEX 106
#define ID_BTN_ADD_EDGE 107

#define ID_EDIT_VERTEX_NAME 201
#define ID_EDIT_EDGE_WEIGHT 202

TransportMap* g_TransportMap = nullptr;
std::string g_SelectedV1 = "";
std::string g_SelectedV2 = "";
HWND hEditVName, hEditEWeight;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawGraph(HWND hwnd, HDC hdc);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "TransportMapWindowClass";
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowExA(0, CLASS_NAME, "Transport Map Dijkstra", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 750, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, nCmdShow);

    g_TransportMap = new TransportMap({});

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        int startX = 820;
        CreateWindowA("BUTTON", "Загрузить файл", WS_VISIBLE | WS_CHILD, startX, 20, 150, 30, hwnd, (HMENU)ID_BTN_LOAD_FILE, NULL, NULL);
        CreateWindowA("BUTTON", "Найти путь", WS_VISIBLE | WS_CHILD, startX, 60, 150, 30, hwnd, (HMENU)ID_BTN_CALC_PATH, NULL, NULL);
        
        CreateWindowA("STATIC", "Имя новой вершины:", WS_VISIBLE | WS_CHILD, startX, 110, 150, 20, hwnd, NULL, NULL, NULL);
        hEditVName = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "Street", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, startX, 130, 150, 25, hwnd, (HMENU)ID_EDIT_VERTEX_NAME, NULL, NULL);
        CreateWindowA("BUTTON", "Добавить точку", WS_VISIBLE | WS_CHILD, startX, 160, 150, 30, hwnd, (HMENU)ID_BTN_ADD_VERTEX, NULL, NULL);

        CreateWindowA("STATIC", "Длина ребра:", WS_VISIBLE | WS_CHILD, startX, 210, 150, 20, hwnd, NULL, NULL, NULL);
        hEditEWeight = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "10", WS_VISIBLE | WS_CHILD | ES_NUMBER, startX, 230, 150, 25, hwnd, (HMENU)ID_EDIT_EDGE_WEIGHT, NULL, NULL);
        CreateWindowA("BUTTON", "Добавить ребро", WS_VISIBLE | WS_CHILD, startX, 260, 150, 30, hwnd, (HMENU)ID_BTN_ADD_EDGE, NULL, NULL);

        CreateWindowA("BUTTON", "Случ. ребра", WS_VISIBLE | WS_CHILD, startX, 320, 150, 30, hwnd, (HMENU)ID_BTN_GEN_RANDOM, NULL, NULL);
        CreateWindowA("BUTTON", "Очистить ребра", WS_VISIBLE | WS_CHILD, startX, 360, 150, 30, hwnd, (HMENU)ID_BTN_CLEAR_EDGES, NULL, NULL);
        CreateWindowA("BUTTON", "Очистить всё", WS_VISIBLE | WS_CHILD, startX, 400, 150, 30, hwnd, (HMENU)ID_BTN_CLEAR_ALL, NULL, NULL);
        
        return 0;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_BTN_LOAD_FILE: {
            OPENFILENAMEA ofn;
            char szFile[260] = { 0 };
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
            if (GetOpenFileNameA(&ofn)) {
                std::ifstream file(szFile);
                std::string line;
                while (std::getline(file, line)) {
                    if (!line.empty()) g_TransportMap->AddVertex(line);
                }
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }

        case ID_BTN_ADD_VERTEX: {
            char buffer[128];
            GetWindowTextA(hEditVName, buffer, 128);
            std::string name(buffer);
            if (!name.empty()) {
                g_TransportMap->AddVertex(name);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }

        case ID_BTN_ADD_EDGE: {
            if (!g_SelectedV1.empty() && !g_SelectedV2.empty()) {
                char buffer[32];
                GetWindowTextA(hEditEWeight, buffer, 32);
                int weight = atoi(buffer);
                if (weight <= 0) weight = 1;

                try {
                    g_TransportMap->AddEdge(weight, g_SelectedV1, g_SelectedV2);
                    InvalidateRect(hwnd, NULL, TRUE);
                } catch(...) { MessageBoxA(hwnd, "Ошибка добавления ребра", "Error", MB_OK); }
            } else {
                MessageBoxA(hwnd, "Выберите две точки на графе (ЛКМ и ПКМ)", "Инфо", MB_OK);
            }
            break;
        }

        case ID_BTN_CALC_PATH: {
            if (!g_SelectedV1.empty() && !g_SelectedV2.empty()) {
                try {
                    Path p = g_TransportMap->DijkstraAlgorithm(g_SelectedV1, g_SelectedV2);
                    if (p.verts.empty()) {
                        MessageBoxA(hwnd, "Путь не найден", "Результат", MB_OK);
                    } else {
                        std::string res = "Путь: ";
                        for (size_t i = 0; i < p.verts.size(); ++i) {
                            res += p.verts[i] + (i == p.verts.size()-1 ? "" : " -> ");
                        }
                        res += "\nИтоговая дистанция: " + std::to_string(p.sum_distance);
                        MessageBoxA(hwnd, res.c_str(), "Результат", MB_OK);
                    }
                } catch (...) { MessageBoxA(hwnd, "Ошибка алгоритма", "Error", MB_ICONERROR); }
            }
            break;
        }

        case ID_BTN_GEN_RANDOM:
            g_TransportMap->GenerateRandomEdges();
            InvalidateRect(hwnd, NULL, TRUE);
            break;

        case ID_BTN_CLEAR_EDGES:
            g_TransportMap->ClearEdges();
            InvalidateRect(hwnd, NULL, TRUE);
            break;

        case ID_BTN_CLEAR_ALL:
            g_TransportMap->Clear();
            g_SelectedV1 = ""; g_SelectedV2 = "";
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        return 0;
    }

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        auto vertices = g_TransportMap->GetAllVertices();
        int i = 0, count = vertices.size();
        for (auto const& [name, ptr] : vertices) {
            double angle = 2 * 3.1415 * i / count;
            int vx = 400 + (int)(300 * cos(angle));
            int vy = 300 + (int)(300 * sin(angle));
            if (sqrt(pow(x - vx, 2) + pow(y - vy, 2)) < 25) {
                if (uMsg == WM_LBUTTONDOWN) g_SelectedV1 = name;
                else g_SelectedV2 = name;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            }
            i++;
        }
        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawGraph(hwnd, hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

void DrawGraph(HWND hwnd, HDC hdc) {
    auto vertices = g_TransportMap->GetAllVertices();
    auto edges = g_TransportMap->GetAllEdges();
    int count = vertices.size();
    if (count == 0) return;

    std::unordered_map<std::string, POINT> coords;
    int i = 0;
    for (auto const& [name, ptr] : vertices) {
        double angle = 2 * 3.1415 * i / count;
        coords[name] = { 400 + (int)(300 * cos(angle)), 300 + (int)(300 * sin(angle)) };
        i++;
    }

    HPEN hEdgePen = CreatePen(PS_SOLID, 1, RGB(180, 180, 180));
    SelectObject(hdc, hEdgePen);
    for (auto const& [key, edge] : edges) {
        POINT p1 = coords[edge->start->data];
        POINT p2 = coords[edge->finish->data];
        MoveToEx(hdc, p1.x, p1.y, NULL);
        LineTo(hdc, p2.x, p2.y);
        std::string dist = std::to_string(edge->data);
        TextOutA(hdc, (p1.x + p2.x) / 2, (p1.y + p2.y) / 2, dist.c_str(), dist.length());
    }

    for (auto const& [name, ptr] : vertices) {
        POINT p = coords[name];
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        if (name == g_SelectedV1) hBrush = CreateSolidBrush(RGB(150, 255, 150));
        if (name == g_SelectedV2) hBrush = CreateSolidBrush(RGB(255, 150, 150));
        
        SelectObject(hdc, hBrush);
        Ellipse(hdc, p.x - 25, p.y - 25, p.x + 25, p.y + 25);
        SetBkMode(hdc, TRANSPARENT);
        TextOutA(hdc, p.x - 20, p.y - 8, name.c_str(), name.length());
        DeleteObject(hBrush);
    }
    DeleteObject(hEdgePen);
}