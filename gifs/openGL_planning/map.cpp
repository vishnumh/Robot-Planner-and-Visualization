//
//  map.cpp
//  Map Visualization
//
//  Created by William Lee on 11/14/21.
//

#include <stdio.h>
#include "fssimplewindow.h"
#include <iostream>
#include<list>
#include<stack>
#include<set>
#include<math.h>
#include <limits>
#include <float.h>
#include "Map.hpp"
using namespace std;





/*
class Map{
private:
    int wid;
    int hei;
    int *pix = nullptr;
public:
    Map();
    ~Map();

    void CleanUp(void);
    int GetHeight(void) const;
    int GetWidth(void) const;
    void Create(int w, int h);
    void SetPixel(int x,int y,int p);
    int GetPixel(int x,int y) const;
    void Draw(int pixelw,int pixelh) const;
};
 */


Map::Map()
{
    wid=0;
    hei=0;
    pix=nullptr;
}


void Map::CleanUp(void)
{
    wid=0;
    hei=0;
    if(nullptr!=pix)
    {
        delete [] pix;
    }
    pix=nullptr;
}

Map::~Map(){
    CleanUp();
}

int Map::GetHeight(void) const{
    return hei;
}

int Map::GetWidth(void) const{
    return wid;
}

void Map::Create(int w,int h)
{
    CleanUp();

    pix=new int [h*w];
    wid=w;
    hei=h;

    for(int i=0; i<wid*hei; ++i)
    {
        pix[i] = 7;
    }
}

void Map::SetPixel(int x,int y,int p)
{
    if(0<=x && x<wid && 0<=y && y<hei)
    {
        pix[y*wid+x]=p;
    }
}


int Map::GetPixel(int x,int y) const
{
    if(0<=x && x<wid && 0<=y && y<hei)
    {
        return pix[y*wid+x];
    }
    return 0;
}




void Map::Draw(int pixelw, int pixelh) const
{
    glBegin(GL_QUADS);

    for(int y=0; y<GetHeight(); ++y)
    {
        for(int x=0; x<GetWidth(); ++x)
        {
            auto pix=GetPixel(x,y);
            int b=255*(pix%2);
            int r=255*((pix/2)%2);
            int g=255*((pix/4)%2);

            glColor3ub(r,g,b);
            glVertex2i(x*pixelw   ,y*pixelh);
            glVertex2i(x*pixelw+pixelw,y*pixelh);
            glVertex2i(x*pixelw+pixelw,y*pixelh+pixelh);
            glVertex2i(x*pixelw   ,y*pixelh+pixelh);
        }
    }
    
    glEnd();
    
    glColor3ub(0,0,0);
    glBegin(GL_LINES);

    for(int x=0; x<=GetWidth(); ++x)
    {
        glVertex2i(x*pixelw,0);
        glVertex2i(x*pixelw,hei*pixelh);
    }
    for(int y=0; y<=GetHeight(); ++y)
    {
        glVertex2i(0,y*pixelh);
        glVertex2i(wid*pixelw,y*pixelh);
    }
    glEnd();

    
}
