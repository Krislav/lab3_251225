#pragma once
#include <iostream>

enum class ErrorCode {
    OK,
    NO_VERTEX,
    NO_EDGE,
    VERTEX_DOESNT_EXIST
};

void ErrorsOutput(ErrorCode error);