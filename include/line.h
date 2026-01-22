#ifndef LINE_H
#define LINE_H

#include "raylib.h"
#include "simulation.h"

typedef struct Line {
    bool active;
    int id;
    Vector3 start;
    Vector3 end;
    Vector3 direction;
    Line* parent;
    Line* children[MAX_LINES];
    Line* attractors[MAX_LINES];
} Line;

Line* CreateLine(Vector3 start, Vector3 end, Vector3 direction, Line* parent);

#endif // LINE_H