#include "simulation.h"

// Space Colonization Settings
int initialNodeAmount = 1000;
int nodesLeft = 0;
float lineLength = 0.1f;
float attractionRange = 1;
float killRange = 0.6f;
float randomGrowth = 0.2f;

List nodes = {0};
List activeNodes = {0};
List lines = {0};
List extremities = {0};

float RandomValue()
{
    return (float)rand() / (float)RAND_MAX;
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

        list_add(&nodes, &node);
    }
}

Vector3 RandomGrowthVector()
{
    float alpha = RandomValue() * PI;
    float theta = RandomValue() * PI * 2;
    Vector3 offset = { cos(theta) * sin(alpha), sin(theta) * sin(alpha), cos(alpha) };
    return offset = (Vector3){ offset.x * randomGrowth, offset.y * randomGrowth, offset.z * randomGrowth };
}

void InitializeSimulation()
{
    // create lists
    list_init(&nodes, sizeof(Vector3*));
    list_init(&lines, sizeof(Line*));
    list_init(&extremities, sizeof(Line*));

    // init random seed
    srand((unsigned int)time(NULL));

    // generate nodes
    GenerateNodes(initialNodeAmount, 5);

    // create entrance
    Vector3 first_start = {0};
    Vector3 first_end = {0, lineLength, 0};
    Line* firstLine = CreateLine(first_start, first_end, (Vector3){0,1,0}, NULL);

    list_add(&lines, firstLine);
    list_add(&extremities, firstLine);
}

void IterateSpaceColonization()
{
    // cleanup leftover nodes and return
    if (list_size(&nodes) > 0 && list_size(&nodes) <= nodesLeft) list_clear(&nodes);
    if (list_size(&nodes) == 0) return;

    // remove nodes in killrange
    List nodes_to_remove = {0};
    list_init(&nodes_to_remove, sizeof(Vector3*));
    for (int i = 0; i < list_size(&nodes); i++)
    {
        for (int j = 0; j < list_size(&lines); j++)
        {
            Vector3* node = list_get(&nodes, i);
            Line* line = list_get(&lines, j);
            float distance = Vector3Distance(*node, line->end);
            if (distance < killRange) list_add(&nodes_to_remove, list_get(&nodes, i));
        }
    }
    list_remove_list(&nodes, &nodes_to_remove);

    /*
        // reset attractors and active nodes
        for (int i = 0; i < passages.Count; i++) passages[i].attractors.Clear();
        activeNodes.Clear();
    */

    // reset attractors and active nodes
    for (int i = 0; i < list_size(&lines); i++)
    {
        Line* line = list_get(&lines, i);
        list_clear(&line->attractors);
        list_clear(&activeNodes);
    }

    // calculate active nodes and attractors
    for (int i = 0; i < list_size(&nodes); i++)
    {
        Vector3* node = list_get(&nodes, i);

        float lastDist = 10000;
        Line* closest = NULL;

        for (int j = 0; j < list_size(&lines); j++)
        {
            Line* line = list_get(&lines, j);

            float distance = Vector3Distance(line->end, *node);
            if (distance < attractionRange && distance < lastDist)
            {
                closest = line;
                lastDist = distance;
            }
        }

        if (closest != NULL)
        {
            list_add(&closest->attractors, list_get(&nodes, 1));
            list_add(&activeNodes, node);
        }
    }

    // if there are nodes in attraction range
    if (list_size(&activeNodes) != 0)
    {
        list_clear(&extremities);

        List newLines = {0};
        list_init(&newLines, sizeof(Line*));

        for (int i = 0; i < list_size(&lines); i++)
        {
            Line* line = list_get(&lines, i);

            if (list_size(&line->attractors) > 0)
            {
                Vector3 dir = {0};
                for (int j = 0; j < list_size(&line->attractors); j++)
                {
                    Vector3* attractor = list_get(&line->attractors, j);
                    Vector3 diff = Vector3Subtract(*attractor, line->end);
                    dir = Vector3Normalize(diff);
                }

                int attractors_amount = list_size(&line->attractors);
                dir = Vector3Divide(dir, (Vector3){attractors_amount, attractors_amount, attractors_amount});

                Vector3 rgv = RandomGrowthVector();
                dir = Vector3Add(dir, rgv);

                dir = Vector3Normalize(dir);

                Vector3 new_line_end = Vector3Multiply(Vector3Add(line->end, dir), (Vector3){lineLength, lineLength, lineLength});
                Line* new_line = CreateLine(line->end, new_line_end, dir, line);

                list_add(&line->children, new_line);
                list_add(&newLines, new_line);
                list_add(&extremities, new_line);
            }
            else if (list_size(&line->children) == 0) list_add(&extremities, line);
        }

        // add all newlines to lines
        for (int i = 0; i < list_size(&newLines); i++)
        {
            Line* new_line = list_get(&newLines, i);
            list_add(&lines, new_line);
        }
    }

    // if no active nodes
    if (list_size(&activeNodes) == 0)
    {
        // extend extremities with new extremities
        for (int i = 0; i < list_size(&extremities); i++)
        {
            Line* current_extremity = list_get(&extremities, i);

            Vector3 new_extremity_dir = Vector3Normalize(Vector3Add(current_extremity->direction, RandomGrowthVector()));
            Vector3 next_extremity_end = Vector3Add(Vector3Add(current_extremity->end, new_extremity_dir), (Vector3){lineLength, lineLength, lineLength});
            Line* next_extremity = CreateLine(current_extremity->end, next_extremity_end, new_extremity_dir, current_extremity);

            list_add(&current_extremity->children, next_extremity);
            list_add(&lines, next_extremity);
            list_remove(&extremities, i);
            list_add(&extremities, next_extremity);
        }
    }
}