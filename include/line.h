#ifndef LINE_H
#define LINE_H

#include "raylib.h"
#include "list.h"

typedef struct Line {
    bool active;
    int id;
    Vector3 start;
    Vector3 end;
    Vector3 direction;
    struct Line* parent;
    List children;
    List attractors;
} Line;

Line* CreateLine(Vector3 start, Vector3 end, Vector3 direction, Line* parent);

#endif // LINE_H