#ifndef LINE_H
#define LINE_H

#include <vector>
#include "raylib.h"

struct Line
{
    bool active;
    int id;
    Vector3 start;
    Vector3 end;
    Vector3 direction;
    Line* parent;
    std::vector<Line*> children;
    std::vector<Vector3*> attractors;
};

Line* CreateLine(Vector3 start, Vector3 end, Vector3 direction, Line* parent);

#endif // LINE_H