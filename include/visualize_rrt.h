// visualize_rrt.h : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef VISUALIZE_RRT_H_IS_INCLUDED
#define VISUALIZE_RRT_H_IS_INCLUDED

#include "fssimplewindow.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <limits>
#include <queue>
#include <set>
#include <cmath>
#include <chrono>
#include <stdlib.h>

typedef std::vector<double> CSpaceVertex; // Storing n DOF dimension points of C free

struct RRTNode
{

    int index;
    int parent_index;
    double cost;
    CSpaceVertex vertex;

};

typedef std::vector<RRTNode*> Tree;
void DrawGoalandstart(CSpaceVertex start, CSpaceVertex goal);

void Drawobstacle(std::vector <double> obst);

void Draw(Tree t, std::vector <double> obst, CSpaceVertex start, CSpaceVertex goal);

void Drawtwotrees(Tree t1, Tree t2, std::vector <double> obst, CSpaceVertex start, CSpaceVertex goal);

void DrawPath(std::vector<CSpaceVertex> path, std::vector <double> obst, CSpaceVertex start, CSpaceVertex goal);

void Moverobot(std::vector<CSpaceVertex> path, std::vector <double> obst, CSpaceVertex start, CSpaceVertex goal);

void Drawrobot(double X, double Y);

#endif