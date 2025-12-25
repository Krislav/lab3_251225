#include "Errors.hpp"

void ErrorsOutput(ErrorCode error) {
    switch (error) {
    case ErrorCode::OK:
        std::cout << "Successful completion\n";
        break;
    case ErrorCode::NO_VERTEX:
        std::cout << "No vertex in input\n";
        break;
    case ErrorCode::NO_EDGE:
        std::cout << "No edge in input\n";
        break;
    case ErrorCode::VERTEX_DOESNT_EXIST:
        std::cout << "Vertex doesnt exist\n";
        break;
    
    default:
        std::cout << "Unknown error\n";
        break;
    }
}