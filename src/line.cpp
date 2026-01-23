#include "line.hpp"

#include <vector>
#include <stdlib.h>
#include "raylib.h"

Line* CreateLine(Vector3 start, Vector3 end, Vector3 direction, Line* parent)
{
    Line* line = (Line*)malloc(sizeof(line));
    line->children;
    line->attractors;
    line->start = start;
    line->end = end;
    line->direction = direction;
    line->parent = parent;
    return line;
}