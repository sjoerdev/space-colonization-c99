#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"
#include "list.h"
#include "line.h"

int initialNodeAmount;
int nodesLeft;
float lineLength;
float attractionRange;
float killRange;
float randomGrowth;

List nodes;
List activeNodes;
List lines;
List extremities;

float RandomValue();
void GenerateNodes(int number, int radius);
Vector3 RandomGrowthVector();
void InitializeSimulation();
void IterateSpaceColonization();

#endif // SIMULATION_H