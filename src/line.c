#include "line.h"

#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

Line* CreateLine(Vector3 start, Vector3 end, Vector3 direction, Line* parent) // also make parent by possible to be NULL
{
    Line* line = (Line*)malloc(sizeof(line));
    line->start = start;
    line->end = end;
    line->direction = direction;
    line->parent = parent;
    line->childrenAmount = 0;
    line->attractorsAmount = 0;
    return line;
}