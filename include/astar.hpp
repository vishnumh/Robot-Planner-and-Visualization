//
//  astar.hpp
//  Map Visualization
//
//  Created by William Lee on 11/19/21.
//

#ifndef astar_hpp
#define astar_hpp

#include <stdio.h>
#include "Map.hpp"
#include<list>
#include<stack>
#include<set>
#define R 20
#define C 20

struct Node
{
    int parentX;
    int parentY;
    double f, g, h;

};

typedef std::pair <int, int> points;
typedef std::pair<double, std::pair<int, int> > pointss;


static bool checkifvalid(int r, int c);
bool checkobstacle(int map[][C], int r, int c);
bool checkdest(int r, int c, points fin);
bool checkdest(int r, int c, points fin);
double calcHval(int r, int c, points fin);
void calculateroute(Node mapgrid[][C], points targetnode, Map &bmap);
void astar (int grid[][C], points start, points fin, Map &bmap,double weight);

#endif /* astar_hpp */
