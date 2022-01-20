//
//  astar.cpp
//  Map Visualization
//
//  Created by William Lee on 11/19/21.
//
#include "fssimplewindow.h"
#include "astar.hpp"
#include <iostream>
#include <stdio.h>
#include<list>
#include<stack>
#include<set>
#include<math.h>
#include <limits>
#include <float.h>

#include "Map.hpp"

using namespace std;


#define R 20
#define C 20

typedef pair <int, int> points;
typedef pair<double, pair<int, int> > pointss;



static bool checkifvalid(int r, int c) //function to check if cell is a valid cell
{
    if (r >= 0 && r < R)
    {
        if (c >= 0 && c < C)
        {
            return true; //checking if row and column is in range
        }
    }
    else
    {
        return false;
    }
    return false;
}

bool checkobstacle(int map[][C], int r, int c) //checking if a cell is an obstacle
{

    if (map[r][c] == 1) //if grid is 1= its an obstacle
    {
        return true;
    }
    else
    {
        return false; // if grid cell=0 its not an obstacle
    }
}

bool checkdest(int r, int c, points fin) //to check if a cell is the target/destination/final cell
{
    if (r == fin.first && c == fin.second)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double calcHval(int r, int c, points fin) //calculates H value and uses euclidean distance formula
{
    double euc_dist;
    euc_dist = (sqrt((r - fin.first) * (r - fin.first) + (c - fin.second) * (c - fin.second)));
    return euc_dist;
}

void calculateroute(Node mapgrid[][C], points targetnode, Map& bmap) //will calculate the path points from source to target point
{
    int r, c;
    r = targetnode.first;
    c = targetnode.second;
    stack<points> Calcroute;

    int wid = C, hei = R;
    int windowx = 800, windowy = 800;
    int pixelw = windowx / wid, pixelh = windowy / hei;


    while (!(mapgrid[r][c].parentX == r && mapgrid[r][c].parentY == c))
    {
        Calcroute.push(make_pair(r, c));
        int temprow, tempcol;
        temprow = mapgrid[r][c].parentX;
        tempcol = mapgrid[r][c].parentY;
        r = temprow;
        c = tempcol;
    }

    Calcroute.push(make_pair(r, c));

    while (!Calcroute.empty())
    {
        FsPollDevice();
        auto key = FsInkey();

        if (key == FSKEY_ESC) {
            break;
        }

        pair< int, int> x = Calcroute.top();

        bmap.SetPixel(x.first, x.second, 6);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bmap.Draw(pixelw, pixelh);

        glColor3ub(0, 0, 0);
        glBegin(GL_LINES);

        for (int x = 0; x <= wid; ++x)
        {
            glVertex2i(x * pixelw, 0);
            glVertex2i(x * pixelw, hei * pixelh);
        }
        for (int y = 0; y <= hei; ++y)
        {
            glVertex2i(0, y * pixelh);
            glVertex2i(wid * pixelw, y * pixelh);
        }
        glEnd();

        FsSwapBuffers();

        FsSleep(100);


        Calcroute.pop();
    }

}


void astar(int grid[][C], points start, points fin, Map& bmap, double weight)
{
    int wid = C, hei = R;
    int windowx = 800, windowy = 800;
    int pixelw = windowx / wid, pixelh = windowy / hei;

    bool closed_list[R][C];
    memset(closed_list, false, sizeof(closed_list));
    Node Nodepts[R][C];
    int i, j;
    printf("%f\n", weight);

    if (checkifvalid(start.first, start.second) == false) //check if start is valid
    {
        cout << "Not a valid cell";
    }

    if (checkifvalid(fin.first, fin.second) == false) //check if end is valid
    {
        cout << "Not a valid cell";
    }


    if (checkobstacle(grid, start.first, start.second) || checkobstacle(grid, fin.first, fin.second))
    {
        cout << "Obstacle is in start or end point";
    }

    if (checkdest(start.first, start.second, fin) == true)
    {
        cout << "Destination reached";
    }


    for (i = 0; i < R; i++)
    {
        for (j = 0; j < C; j++)
        {
            Nodepts[i][j].g = FLT_MAX;
            Nodepts[i][j].h = FLT_MAX;
            Nodepts[i][j].f = FLT_MAX;
            Nodepts[i][j].parentX = -1;
            Nodepts[i][j].parentY = -1;
        }
    }


    i = start.first;
    j = start.second;
    Nodepts[i][j].g = 0;
    Nodepts[i][j].h = 0;
    Nodepts[i][j].f = 0;
    Nodepts[i][j].parentX = i;
    Nodepts[i][j].parentY = j;

    set<pointss> open_list;
    open_list.insert(make_pair(0.0, make_pair(i, j)));

    bool final_node = false;

    while (!open_list.empty())
    {
        FsPollDevice();
        auto key = FsInkey();

        if (key == FSKEY_ESC) {
            break; 
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bmap.Draw(pixelw, pixelh);

        FsSwapBuffers();

        FsSleep(25);

        pointss p = *open_list.begin();
        open_list.erase(open_list.begin());
        i = p.second.first;
        j = p.second.second;
        closed_list[i][j] = true;
        bmap.SetPixel(i, j, 5);
        bmap.SetPixel(start.first, start.second, 1);
        bmap.SetPixel(fin.first, fin.second, 2);

        double g_updated, h_updated, f_updated;

        //checking north dirxn

        if (checkifvalid(i - 1, j) == true)
        {
            if (checkdest(i - 1, j, fin) == true)
            {

                Nodepts[i - 1][j].parentX = i;
                Nodepts[i - 1][j].parentY = j;
                printf("The finination cell is found\n");
                calculateroute(Nodepts, fin, bmap);
                final_node = true;
                return;
            }

            else if (closed_list[i - 1][j] == false && !checkobstacle(grid, i - 1, j))
            {
                g_updated = Nodepts[i][j].g + 1.0;
                h_updated = calcHval(i - 1, j, fin);
                f_updated = g_updated + weight * h_updated;



                if (Nodepts[i - 1][j].f == FLT_MAX || Nodepts[i - 1][j].f > f_updated)
                {
                    bmap.SetPixel(i - 1, j, 4);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    bmap.Draw(pixelw, pixelh);

                    FsSwapBuffers();

                    FsSleep(25);

                    open_list.insert(make_pair(f_updated, make_pair(i - 1, j)));
                    Nodepts[i - 1][j].f = f_updated;
                    Nodepts[i - 1][j].g = g_updated;
                    Nodepts[i - 1][j].h = h_updated;
                    Nodepts[i - 1][j].parentX = i;
                    Nodepts[i - 1][j].parentY = j;


                }
            }
        }
        //checking south
        if (checkifvalid(i + 1, j) == true)
        {
            if (checkdest(i + 1, j, fin) == true)
            {
                Nodepts[i + 1][j].parentX = i;
                Nodepts[i + 1][j].parentY = j;
                calculateroute(Nodepts, fin, bmap);
                final_node = true;
                return;
            }
            else if (closed_list[i + 1][j] == false && !checkobstacle(grid, i + 1, j))
            {
                g_updated = Nodepts[i][j].g + 1.0;
                h_updated = calcHval(i + 1, j, fin);
                f_updated = g_updated + weight * h_updated;



                if (Nodepts[i + 1][j].f == FLT_MAX || Nodepts[i + 1][j].f > f_updated)
                {
                    bmap.SetPixel(i + 1, j, 4);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    bmap.Draw(pixelw, pixelh);

                    FsSwapBuffers();

                    FsSleep(25);
                    open_list.insert(make_pair(f_updated, make_pair(i + 1, j)));
                    Nodepts[i + 1][j].f = f_updated;
                    Nodepts[i + 1][j].g = g_updated;
                    Nodepts[i + 1][j].h = h_updated;
                    Nodepts[i + 1][j].parentX = i;
                    Nodepts[i + 1][j].parentY = j;
                }
            }
        }
        //check east
        if (checkifvalid(i, j + 1) == true)
        {
            if (checkdest(i, j + 1, fin) == true)
            {
                Nodepts[i][j + 1].parentX = i;
                Nodepts[i][j + 1].parentY = j;
                calculateroute(Nodepts, fin, bmap);
                final_node = true;
                return;
            }
            else if (closed_list[i][j + 1] == false && !checkobstacle(grid, i, j + 1))
            {
                g_updated = Nodepts[i][j].g + 1.0;
                h_updated = calcHval(i, j + 1, fin);
                f_updated = g_updated + weight * h_updated;



                if (Nodepts[i][j + 1].f == FLT_MAX || Nodepts[i][j + 1].f > f_updated)
                {
                    bmap.SetPixel(i, j + 1, 4);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    bmap.Draw(pixelw, pixelh);

                    FsSwapBuffers();

                    FsSleep(25);
                    open_list.insert(make_pair(f_updated, make_pair(i, j + 1)));
                    Nodepts[i][j + 1].f = f_updated;
                    Nodepts[i][j + 1].g = g_updated;
                    Nodepts[i][j + 1].h = h_updated;
                    Nodepts[i][j + 1].parentX = i;
                    Nodepts[i][j + 1].parentY = j;
                }
            }
        }

        if (checkifvalid(i, j - 1) == true)
        {

            if (checkdest(i, j - 1, fin) == true)
            {
                Nodepts[i][j - 1].parentX = i;
                Nodepts[i][j - 1].parentY = j;
                calculateroute(Nodepts, fin, bmap);
                final_node = true;
                return;
            }
            else if (closed_list[i][j - 1] == false && !checkobstacle(grid, i, j - 1))
            {
                g_updated = Nodepts[i][j].g + 1.0;
                h_updated = calcHval(i, j - 1, fin);
                f_updated = g_updated + weight * h_updated;



                if (Nodepts[i][j - 1].f == FLT_MAX || Nodepts[i][j - 1].f > f_updated)
                {
                    bmap.SetPixel(i, j - 1, 4);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    bmap.Draw(pixelw, pixelh);

                    FsSwapBuffers();

                    FsSleep(25);
                    open_list.insert(make_pair(f_updated, make_pair(i, j - 1)));
                    Nodepts[i][j - 1].f = f_updated;
                    Nodepts[i][j - 1].g = g_updated;
                    Nodepts[i][j - 1].h = h_updated;
                    Nodepts[i][j - 1].parentX = i;
                    Nodepts[i][j - 1].parentY = j;
                }
            }
        }

        //check northeast
        if (checkifvalid(i - 1, j + 1) == true)
        {
            if (checkdest(i - 1, j + 1, fin) == true)
            {
                Nodepts[i - 1][j + 1].parentX = i;
                Nodepts[i - 1][j + 1].parentY = j;
                calculateroute(Nodepts, fin, bmap);
                final_node = true;
                return;
            }
            else if (closed_list[i - 1][j + 1] == false && !checkobstacle(grid, i - 1, j + 1)) {
                g_updated = Nodepts[i][j].g + 1.414;
                h_updated = calcHval(i - 1, j + 1, fin);
                f_updated = g_updated + weight * h_updated;



                if (Nodepts[i - 1][j + 1].f == FLT_MAX || Nodepts[i - 1][j + 1].f > f_updated)
                {
                    bmap.SetPixel(i - 1, j + 1, 4);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    bmap.Draw(pixelw, pixelh);

                    FsSwapBuffers();

                    FsSleep(25);
                    open_list.insert(make_pair(f_updated, make_pair(i - 1, j + 1)));
                    Nodepts[i - 1][j + 1].f = f_updated;
                    Nodepts[i - 1][j + 1].g = g_updated;
                    Nodepts[i - 1][j + 1].h = h_updated;
                    Nodepts[i - 1][j + 1].parentX = i;
                    Nodepts[i - 1][j + 1].parentY = j;
                }
            }
        }

        //check NW
        if (checkifvalid(i - 1, j - 1) == true)
        {
            if (checkdest(i - 1, j - 1, fin) == true)
            {
                Nodepts[i - 1][j - 1].parentX = i;
                Nodepts[i - 1][j - 1].parentY = j;
                calculateroute(Nodepts, fin, bmap);
                final_node = true;
                return;
            }
            else if (closed_list[i - 1][j - 1] == false && !checkobstacle(grid, i - 1, j - 1))
            {
                g_updated = Nodepts[i][j].g + 1.414;
                h_updated = calcHval(i - 1, j - 1, fin);
                f_updated = g_updated + weight * h_updated;



                if (Nodepts[i - 1][j - 1].f == FLT_MAX || Nodepts[i - 1][j - 1].f > f_updated)
                {
                    bmap.SetPixel(i - 1, j - 1, 4);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    bmap.Draw(pixelw, pixelh);

                    FsSwapBuffers();

                    FsSleep(25);
                    open_list.insert(make_pair(f_updated, make_pair(i - 1, j - 1)));
                    Nodepts[i - 1][j - 1].f = f_updated;
                    Nodepts[i - 1][j - 1].g = g_updated;
                    Nodepts[i - 1][j - 1].h = h_updated;
                    Nodepts[i - 1][j - 1].parentX = i;
                    Nodepts[i - 1][j - 1].parentY = j;
                }
            }
        }
        //checkSE
        if (checkifvalid(i + 1, j + 1) == true)
        {
            if (checkdest(i + 1, j + 1, fin) == true)
            {
                Nodepts[i + 1][j + 1].parentX = i;
                Nodepts[i + 1][j + 1].parentY = j;
                calculateroute(Nodepts, fin, bmap);
                final_node = true;
                return;
            }

            else if (closed_list[i + 1][j + 1] == false && !checkobstacle(grid, i + 1, j + 1))
            {
                g_updated = Nodepts[i][j].g + 1.414;
                h_updated = calcHval(i + 1, j + 1, fin);
                f_updated = g_updated + weight * h_updated;




                if (Nodepts[i + 1][j + 1].f == FLT_MAX || Nodepts[i + 1][j + 1].f > f_updated)
                {
                    bmap.SetPixel(i + 1, j + 1, 4);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    bmap.Draw(pixelw, pixelh);

                    FsSwapBuffers();

                    FsSleep(25);

                    open_list.insert(make_pair(f_updated, make_pair(i + 1, j + 1)));
                    Nodepts[i + 1][j + 1].f = f_updated;
                    Nodepts[i + 1][j + 1].g = g_updated;
                    Nodepts[i + 1][j + 1].h = h_updated;
                    Nodepts[i + 1][j + 1].parentX = i;
                    Nodepts[i + 1][j + 1].parentY = j;
                }
            }
        }

        //check SW
        if (checkifvalid(i + 1, j - 1) == true)
        {
            if (checkdest(i + 1, j - 1, fin) == true)
            {
                Nodepts[i + 1][j - 1].parentX = i;
                Nodepts[i + 1][j - 1].parentY = j;
                calculateroute(Nodepts, fin, bmap);
                final_node = true;
                return;
            }

            else if (closed_list[i + 1][j - 1] == false && !checkobstacle(grid, i + 1, j - 1))
            {
                g_updated = Nodepts[i][j].g + 1.414;
                h_updated = calcHval(i + 1, j - 1, fin);
                f_updated = g_updated + weight * h_updated;



                if (Nodepts[i + 1][j - 1].f == FLT_MAX || Nodepts[i + 1][j - 1].f > f_updated)
                {
                    bmap.SetPixel(i + 1, j - 1, 4);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    bmap.Draw(pixelw, pixelh);

                    FsSwapBuffers();

                    FsSleep(25);
                    open_list.insert(make_pair(f_updated, make_pair(i + 1, j - 1)));
                    Nodepts[i + 1][j - 1].f = f_updated;
                    Nodepts[i + 1][j - 1].g = g_updated;
                    Nodepts[i + 1][j - 1].h = h_updated;
                    Nodepts[i + 1][j - 1].parentX = i;
                    Nodepts[i + 1][j - 1].parentY = j;
                }
            }
        }
    }

    if (final_node == false)
    {
        cout << "target not found";
    }
}