// visualize_rrt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "visualize_rrt.h"
#include "yspng.h"
#include "yspngenc.h"

#include "ysglfontdata.h"
#include <stdio.h>
#include <time.h>


void DrawGoalandstart(CSpaceVertex start, CSpaceVertex goal)
{
    double gx = goal[0];
    double gy = goal[1];
    double sx = start[0];
    double sy = start[1];
    glColor3ub(255, 0, 0);
    glPointSize(9);
    glBegin(GL_POINTS);

    glVertex2i(sx, sy);
    glEnd();

    glColor3ub(0, 255, 0);
    glPointSize(9);
    glBegin(GL_POINTS);

    glVertex2i(gx, gy);
    glEnd();
}

void Drawobstacle(std::vector <double> obst)
{
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);

    int size = obst.size() / 4;

    for (int i = 0; i < size; i++)
    {
        int start = 4 * i;
        glVertex2d(obst[start], obst[start + 1]);
        glVertex2d(obst[start] + obst[start + 2], obst[start + 1]);
        glVertex2d(obst[start] + obst[start + 2], obst[start + 1] + obst[start + 3]);
        glVertex2d(obst[start], obst[start + 1] + obst[start + 3]);
    }
    glEnd();
}

void Draw(Tree t, std::vector <double> obst, CSpaceVertex start, CSpaceVertex goal)
{

    FsPollDevice();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    Drawobstacle(obst);
    DrawGoalandstart(start, goal);
    Drawrobot(start[0], start[1]);
    double x1, y1, x2, y2;

    for (int i = 0; i < t.size(); ++i)
    {
        auto temp = t[i];
        int n;
        //double x1, y1, x2, y2;
        x1 = temp->vertex[0];

        y1 = temp->vertex[1];
        n = temp->parent_index;

        if (n != -1)
        {
            x2 = t[n]->vertex[0];
            y2 = t[n]->vertex[1];
            glColor3ub(255, 0, 0);
            glBegin(GL_LINES);
            glVertex2d(x1, y1);
            glVertex2d(x2, y2);
            glEnd();
            // if(parent_index)
        
            glPointSize(3);
            glBegin(GL_POINTS);

            glVertex2i(x2, y2);
            glEnd();
        
        }

        

    }

    FsSwapBuffers();
    FsSleep(50);
}

void Drawtwotrees(Tree t1, Tree t2, std::vector <double> obst, CSpaceVertex start, CSpaceVertex goal)
{

    FsPollDevice();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    Drawobstacle(obst);
    DrawGoalandstart(start, goal);
    double x1, y1, x2, y2;

    for (int i = 0; i < t1.size(); ++i)
    {
        auto temp = t1[i];
        int n;
        //double x1, y1, x2, y2;
        x1 = temp->vertex[0];

        y1 = temp->vertex[1];
        n = temp->parent_index;

        if (n != -1)
        {
            x2 = t1[n]->vertex[0];
            y2 = t1[n]->vertex[1];
            glColor3ub(255, 0, 0);
            glBegin(GL_LINES);
            glVertex2d(x1, y1);
            glVertex2d(x2, y2);
            glEnd();
            // if(parent_index)
        
            glPointSize(3);
            glBegin(GL_POINTS);

            glVertex2i(x2, y2);
            glEnd();
        }

        

    }

    for (int i = 0; i < t2.size(); ++i)
    {
        auto temp = t2[i];
        int n;
        //double x1, y1, x2, y2;
        x1 = temp->vertex[0];

        y1 = temp->vertex[1];
        n = temp->parent_index;
        //double x1, y1, x2, y2;

        if (n != -1)
        {
            x2 = t2[n]->vertex[0];
            y2 = t2[n]->vertex[1];
            glColor3ub(255, 0, 0);
            glBegin(GL_LINES);
            glVertex2d(x1, y1);
            glVertex2d(x2, y2);
            glEnd();
            // if(parent_index)
        
            glPointSize(3);
            glBegin(GL_POINTS);

            glVertex2i(x2, y2);
            glEnd();
        
        }

        

    }

    FsSwapBuffers();
    FsSleep(5);
}

void DrawPath(std::vector<CSpaceVertex> path, std::vector <double> obst, CSpaceVertex start, CSpaceVertex goal)
{
    printf("DRAW PATH");
    double x1, y1, x2, y2;
    for (int i = 0; i < path.size() - 1; ++i)
    {
        FsPollDevice();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        

        // double x1, y1, x2, y2;

        for (int j = 0; j <= i; ++j)
        {
            x1 = path[j][0];
            y1 = path[j][1];
            x2 = path[j + 1][0];
            y2 = path[j + 1][1];
            Drawobstacle(obst);
            DrawGoalandstart(start, goal);
            glColor3ub(0, 0, 255);
            glBegin(GL_LINES);
            glVertex2d(x1, y1);
            glVertex2d(x2, y2);
            glEnd();
        }
        FsSwapBuffers();
        FsSleep(5);

    }


}

void Drawrobot(double X, double Y)
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    YsRawPngDecoder png;
    if(YSOK==png.Decode("robot_final.png"))
    {
        png.Flip();
        printf("%d x %d\n",png.wid,png.hei);
    }
    glRasterPos2d(X - 27.5, Y + 33);
    glDrawPixels(png.wid,png.hei,GL_RGBA,GL_UNSIGNED_BYTE,png.rgba);
}

void Moverobot(std::vector<CSpaceVertex> path, std::vector <double> obst, CSpaceVertex start, CSpaceVertex goal)
{
    // Goal Image ****************************
    YsRawPngDecoder png2;
    if (YSOK == png2.Decode("Win.png"))
    {
        png2.Flip();
        printf("%d x %d\n", png2.wid, png2.hei);
    }
    else
    {
        printf("Could not read the image.\n");
    }
  
    // *****************************************
    double x1, y1, x2, y2;
    for (int i = 0; i < path.size() - 1; ++i)
    {
        FsPollDevice();
        
        int k = path.size() - 2 - i;
        // double x1, y1, x2, y2;

        for (int j = path.size() - 2; j >= k; --j)
        {
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            Drawobstacle(obst);
            DrawGoalandstart(start, goal);
            x1 = path[j][0];
            y1 = path[j][1];
            x2 = path[j + 1][0];
            y2 = path[j + 1][1];
            for (int l = path.size() - 2; l >= k; --l)
            {
            

                x1 = path[l][0];
                y1 = path[l][1];
                x2 = path[l + 1][0];
                y2 = path[l + 1][1];
                glColor3ub(0, 0, 255);
                glBegin(GL_LINES);
                glVertex2d(x1, y1);
                glVertex2d(x2, y2);
                glEnd();
            }
            Drawrobot(x2, y2);
            glColor3ub(0, 0, 255);
            glBegin(GL_LINES);
            glVertex2d(x1, y1);
            glVertex2d(x2, y2);
            glEnd();
        }
        FsSwapBuffers();
        FsSleep(5);

    }
    // Goal reached Img **********************
    FsSleep(500);


        for(;;)
        {
            FsPollDevice();
            auto key=FsInkey();
            if(FSKEY_ESC==key)
            {
                break;
            }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glRasterPos2i(0, 799);
            glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);
            glColor3f(0, 1, 0);
            glRasterPos2i(275, 675);
            YsGlDrawFontBitmap20x28("Goal Reached");
            FsSwapBuffers();

        }
        //FsSleep(7000);
        
    
    // ******************************************



}


