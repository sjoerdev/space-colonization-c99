#include "line.h"

#include <stdlib.h>
#include "raylib.h"

Line* CreateLine(Vector3 start, Vector3 end, Vector3 direction, Line* parent)
{
    Line* line = (Line*)malloc(sizeof(line));
    line->children = (List){0};
    line->attractors = (List){0};
    list_init(&line->children, sizeof(Line*));
    list_init(&line->attractors, sizeof(Vector3*));
    line->start = start;
    line->end = end;
    line->direction = direction;
    line->parent = parent;
    return line;
}