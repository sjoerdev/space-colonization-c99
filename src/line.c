#include "line.h"

#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"

Line* CreateLineHeap(Vector3 start, Vector3 end, Vector3 direction, Line* parent)
{
    Line* line = (Line*)malloc(sizeof(line));
    list_init(&line->children, sizeof(Line*));
    list_init(&line->attractors, sizeof(Vector3*));
    line->start = start;
    line->end = end;
    line->direction = direction;
    line->parent = parent;
    return line;
}