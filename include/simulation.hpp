#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdlib.h>
#include <vector>
#include <time.h>
#include "raylib.h"
#include "raymath.h"
#include "line.hpp"

extern int initialNodeAmount;
extern int nodesLeft;
extern float lineLength;
extern float attractionRange;
extern float killRange;
extern float randomGrowth;

extern std::vector<Vector3*> nodes;
extern std::vector<Vector3*> activeNodes;
extern std::vector<Line*> lines;
extern std::vector<Line*> extremities;

float RandomValue();
void GenerateNodes(int number, float radius);
Vector3 RandomGrowthVector();
void InitializeSimulation();
void IterateSpaceColonization();

#endif // SIMULATION_H