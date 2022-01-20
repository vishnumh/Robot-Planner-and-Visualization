// rrtconnect.h : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef RRTCONNECT_H_IS_INCLUDED
#define RRTCONNECT_H_IS_INCLUDED
#include "visualize_rrt.h"
//#include "fssimplewindow.h"
//#include <math.h>
//#include <iostream>
//#include <vector>
//#include <map>
//#include <random>
//#include <limits>
//#include <queue>
//#include <set>
//#include <cmath>
//#include <chrono>
//#include <stdlib.h>

class rrtconnect
{
public:

    // ************************************* RRT *********************************************************************

    bool IsValidConfiguration(int size_x, int size_y, double* map, CSpaceVertex q, std::vector <double> obst);

    CSpaceVertex sampleRandomVertex(double* map, int x_size, int y_size, std::vector <double> obst);

    float VertexDistance(CSpaceVertex v1, CSpaceVertex v2);

    void printCSpaceVertex(CSpaceVertex v);

    int RRTNearestNeighbour(Tree& tree, CSpaceVertex q_rand);

    bool LocalPlanner(double* map, int x_size, int y_size, CSpaceVertex v1, CSpaceVertex v2, int num_steps, std::vector <double> obst);

    CSpaceVertex AltRRTGenerateNewVertex(Tree& tree, CSpaceVertex q_rand, int q_near_index, double max_step, int numofDOFs, double* map,
        int x_size, int y_size, int num_steps, std::vector <double> obst);

    void RRTAddVertex(Tree& tree, CSpaceVertex q_new, int nn_index, int& count);

    // ************************************* RRT *********************************************************************
    
    std::vector<int> RRTConnectExtend(Tree& tree, CSpaceVertex q, int q_near_index, double max_step, int numofDOFs, double* map,
        int x_size, int y_size, int num_steps, int& count, std::vector <double> obst);
    
    std::vector<CSpaceVertex>plannerRRTConnect(
        double* map,
        int x_size,
        int y_size,
        CSpaceVertex start,
        CSpaceVertex goal,
        int numofDOFs,
        std::vector <double> obst
    );

};
#endif