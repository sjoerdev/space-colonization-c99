#ifndef PASSAGE_H
#define PASSAGE_H

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
    Vector3* attractors[MAX_NODES];
    int childrenAmount;
    int attractorsAmount;
} Line;

Line* CreateLine(Vector3 start, Vector3 end, Vector3 direction, Line* parent);

#endif // PASSAGE_H