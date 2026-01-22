#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "list.h"
#include "line.h"

// Space Colonization Settings
int initialNodeAmount = 1000;
int nodesLeft = 0;
float passageLength = 0.1f;
float attractionRange = 1;
float killRange = 0.6f;
float randomGrowth = 0.2f;

List* nodes; // needs to be removable
List* activeNodes;
List* lines;
List* extremities; // needs to be removable

void InitializeSimulation()
{
    // create lists
    list_init(nodes, sizeof(Vector3));
    list_init(activeNodes, sizeof(int));
    list_init(lines, sizeof(Line));
    list_init(extremities, sizeof(Line));
    
    // init random seed
    srand(0);

    // generate nodes
    GenerateNodes(initialNodeAmount, 5);

    // create entrance
    Vector3 first_start = {0};
    Vector3 first_end = { 0, passageLength, 0 };
    Line* firstLine = CreateLine(first_start, first_end, (Vector3){0,1,0}, NULL);

    lines.Add(firstLine);
    extremities.Add(firstLine);
}

void IterateSpaceColonization()
{
    // cleanup leftover nodes and return
    if (nodes.Count > 0 && nodes.Count <= nodesLeft) nodes.Clear();
    if (nodes.Count == 0) return;

    // remove nodes in killrange
    List<Vector3> toRemove = new List<Vector3>();
    for (int i = 0; i < nodes.Count; i++)
    {
        for (int j = 0; j < lines.Count; j++)
        {
            float distance = Vector3.Distance(nodes[i], lines[j].end);
            if (distance < killRange) toRemove.Add(nodes[i]);
        }
    }
    foreach (var node in toRemove) nodes.Remove(node);

    // reset attractors and active nodes
    for (int i = 0; i < lines.Count; i++) lines[i].attractors.Clear();
    activeNodesAmount.Clear();

    // calculate active nodes and attractors
    for (int i = 0; i < nodes.Count; i++)
    {
        float lastDist = 10000;
        Passage closest = null;

        for (int j = 0; j < lines.Count; j++) 
        {
            float distance = Vector3.Distance(lines[j].end, nodes[i]);
            if (distance < attractionRange && distance < lastDist) 
            {
                closest = lines[j];
                lastDist = distance;
            }
        }

        if (closest != null)
        {
            closest.attractors.Add(nodes[i]);
            activeNodesAmount.Add(i);
        }
    }

    // if there are nodes in attraction range
    if (activeNodesAmount.Count != 0)
    {
        extremities.Clear();
        List<Passage> newPassages = new List<Passage>();

        for (int i = 0; i < lines.Count; i++)
        {
            if (lines[i].attractors.Count > 0)
            {
                Vector3 dir = new Vector3(0, 0, 0);
                for (int j = 0; j < lines[i].attractors.Count; j++)
                {
                    var diff = passages[i].attractors[j] - passages[i].end;
                    dir += Vector3.Normalize(diff);
                }
                dir /= lines[i].attractors.Count;
                dir += RandomGrowthVector();
                dir = Vector3.Normalize(dir);
                Passage passage = new Passage(passages[i].end, passages[i].end + dir * passageLength, dir, passages[i]);
                lines[i].children.Add(passage);
                newPassages.Add(passage);
                extremities.Add(passage);
            } 
            else if (lines[i].children.Count == 0) extremities.Add(lines[i]);
        }

        lines.AddRange(newPassages);
    }

    // if no active nodes
    if (activeNodesAmount.Count == 0)
    {
        for (int i = 0; i < extremities.Count; i++)
        {
            Passage current = extremities[i];
            bool beginning = lines.Count < 20;

            if (beginning)
            {
                Vector3 raw = current.direction + RandomGrowthVector();
                Vector3 dir = Vector3.Normalize(raw);
                Passage next = new Passage(current.end, current.end + dir * passageLength, dir, current);
                current.children.Add(next);
                lines.Add(next);
                extremities.Remove(current);
                extremities.Add(next);
            }
        }
    }
}

void GenerateNodes(int number, int radius)
{
    for (int i = 0; i < number; i++)
    {
        float alpha = RandomValue() * PI;
        float theta = RandomValue() * PI * 2;
        float offset = pow(RandomValue(), 1.0f / 3.0f) / 2;

        Vector3 dir = {
            cos(theta) * sin(alpha),
            sin(theta) * sin(alpha),
            cos(alpha)
        };

        Vector3 node = {
            dir.x * offset * radius,
            dir.y * offset * radius,
            dir.z * offset * radius,
        };
        
        list_add(nodes, (void*)&node);
    }
}

float RandomValue()
{
    return (float)rand() / (float)RAND_MAX;
}

Vector3 RandomGrowthVector()
{
    float alpha = RandomValue() * PI;
    float theta = RandomValue() * PI * 2;
    Vector3 offset = { cos(theta) * sin(alpha), sin(theta) * sin(alpha), cos(alpha) };
    return offset = (Vector3){ offset.x * randomGrowth, offset.y * randomGrowth, offset.z * randomGrowth };
}

#endif // SIMULATION_H