#include <vector>
#include "simulation.hpp"

// Space Colonization Settings
int initialNodeAmount = 1000;
int nodesLeft = 0;
float lineLength = 0.1f;
float attractionRange = 1;
float killRange = 0.6f;
float randomGrowth = 0.2f;

std::vector<Vector3*> nodes = {};
std::vector<Vector3*> activeNodes = {};
std::vector<Line*> lines = {};
std::vector<Line*> extremities = {};

float RandomValue()
{
    return (float)rand() / (float)RAND_MAX;
}

void GenerateNodes(int number, float radius)
{
    for (int i = 0; i < number; i++)
    {
        float alpha = RandomValue() * PI;
        float theta = RandomValue() * PI * 2;
        float offset = (float)pow(RandomValue(), 1.0f / 3.0f) / 2;

        Vector3 dir = {
            (float)cos(theta) * (float)sin(alpha),
            (float)sin(theta) * (float)sin(alpha),
            (float)cos(alpha)
        };

        Vector3 node = dir * offset * (float)radius;

        nodes.push_back(&node);
    }
}

Vector3 RandomGrowthVector()
{
    float alpha = RandomValue() * PI;
    float theta = RandomValue() * PI * 2;
    Vector3 offset = { (float)cos(theta) * (float)sin(alpha), (float)sin(theta) * (float)sin(alpha), (float)cos(alpha) };
    return offset = offset * randomGrowth;
}

void InitializeSimulation()
{
    // init random seed
    srand((unsigned int)time(NULL));

    // generate nodes
    GenerateNodes(initialNodeAmount, 5.0f);

    // create entrance
    Vector3 first_start = {};
    Vector3 first_end = {0, lineLength, 0};
    Line* firstLine = CreateLine(first_start, first_end, {0,1,0}, NULL);

    lines.push_back(firstLine);
    extremities.push_back(firstLine);
}

void IterateSpaceColonization()
{
    // cleanup leftover nodes and return
    if (nodes.size() > 0 && nodes.size() <= nodesLeft) nodes.clear();
    if (nodes.size() == 0) return;

    // remove nodes in killrange
    std::vector<int> nodes_indexes_to_remove = {};
    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = 0; j < lines.size(); j++)
        {
            Vector3* node = nodes[i];
            Line* line = lines[j];
            float distance = Vector3Distance(*node, line->end);
            if (distance < killRange) nodes_indexes_to_remove.push_back(i);
        }
    }

    for (int i = 0; i < nodes_indexes_to_remove.size(); i++)
    {
        int index = nodes_indexes_to_remove[i];
        nodes.erase(nodes.begin() + index);
    }

    // reset attractors and active nodes
    for (int i = 0; i < lines.size(); i++)
    {
        Line* line = lines[i];
        line->attractors.clear();
        line->children.clear();
    }

    // calculate active nodes and attractors
    for (int i = 0; i < nodes.size(); i++)
    {
        Vector3* node = nodes[i];

        float lastDist = 10000;
        Line* closest = NULL;

        for (int j = 0; j < lines.size(); j++)
        {
            Line* line = lines[j];

            float distance = Vector3Distance(line->end, *node);
            if (distance < attractionRange && distance < lastDist)
            {
                closest = line;
                lastDist = distance;
            }
        }

        if (closest != NULL)
        {
            closest->attractors.push_back(node);
            activeNodes.push_back(node);
        }
    }

    // if there are nodes in attraction range
    if (activeNodes.size() != 0)
    {
        extremities.clear();

        std::vector<Line*> newLines = {};

        for (int i = 0; i < lines.size(); i++)
        {
            Line* line = lines[i];

            if (line->attractors.size() > 0)
            {
                Vector3 dir = {0};
                for (int j = 0; j < line->attractors.size(); j++)
                {
                    Vector3* attractor = line->attractors[j];
                    Vector3 diff = *attractor - line->end;
                    dir = Vector3Normalize(diff);
                }

                int attractors_amount = (int)line->attractors.size();
                dir /= (float)attractors_amount;
                dir += RandomGrowthVector();
                dir = Vector3Normalize(dir);

                Vector3 new_line_end = (line->end + dir) * lineLength;
                Line* new_line = CreateLine(line->end, new_line_end, dir, line);

                line->children.push_back(new_line);
                newLines.push_back(new_line);
                extremities.push_back(new_line);
            }
            else if (line->children.size() == 0) extremities.push_back(line);
        }

        // add all newlines to lines
        for (int i = 0; i < newLines.size(); i++) lines.push_back(newLines[i]);
    }

    // if no active nodes
    if (activeNodes.size() == 0)
    {
        // extend extremities with new extremities
        for (int i = 0; i < extremities.size(); i++)
        {
            Line* current_extremity = extremities[i];

            Vector3 new_extremity_dir = Vector3Normalize(current_extremity->direction + RandomGrowthVector());
            Vector3 next_extremity_end = (current_extremity->end + new_extremity_dir) + Vector3{lineLength, lineLength, lineLength};
            Line* next_extremity = CreateLine(current_extremity->end, next_extremity_end, new_extremity_dir, current_extremity);


            current_extremity->children.push_back(next_extremity);
            lines.push_back(next_extremity);
            extremities.erase(extremities.begin() + i);
            extremities.push_back(next_extremity);
        }
    }
}