#pragma once
#include <iostream>

enum class ErrorCode {
    OK,
    NO_VERTEX,
    NO_EDGE,
    EDGE_DATA_CANT_BE_PATHS_LENGHT
};

void ErrorsOutput(ErrorCode error);