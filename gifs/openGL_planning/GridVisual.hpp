//
//  AStarVisualize.hpp
//  Map Visualization
//
//  Created by William Lee on 11/21/21.
//

#ifndef GridVisual_hpp
#define GridVisual_hpp

#include <stdio.h>


class GridVisual{
    
public:
    int windowx;
    int windowy;
    int pixelw,pixelh;
    double weight = 1.0;
    GridVisual(int wx,int wy);
    void SetWeight(double w);
    void visualize();

};

#endif /* GridVisual_hpp */
