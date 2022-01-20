#include "Main.hpp"
#include "GridVisual.hpp"
#include "rrt.h"
#include "rrtconnect.h"
#include "rrtstar.h"
#include "visualize_rrt.h"
#include <stdio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#include "yssimplesound.h"
#include "ysglfontdata.h"
#include "yspng.h"
#include "yspngenc.h"

int main(void)
{
    
    FsOpenWindow(32, 32, 800, 800, 1);
    int curs = 0;
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    YsRawPngDecoder png1;
    if (YSOK == png1.Decode("background.png"))
    {
        png1.Flip();
        printf("%d x %d\n", png1.wid, png1.hei);
    }
    else
    {
        printf("Could not read the image.\n");
    }

    YsRawPngDecoder png2;
    if (YSOK == png2.Decode("cursor.png"))
    {
        png2.Flip();
        printf("%d x %d\n", png2.wid, png2.hei);
    }
    else
    {
        printf("Could not read the image.\n");
    }
    for (;;)
    {
        FsPollDevice();
        auto  key = FsInkey();
        if (key == FSKEY_ESC)
            return 0;
        if (key == FSKEY_DOWN)
        {
            curs += 1;
            if (curs > 6)
                curs = 0;
        }
        if (key == FSKEY_UP)
        {
            curs -= 1;
            if (curs < 0)
                curs = 6;
        }
        if (key == FSKEY_ENTER)
        {
            
            break;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glRasterPos2i(0, 799);
        glDrawPixels(png1.wid, png1.hei, GL_RGBA, GL_UNSIGNED_BYTE, png1.rgba);
        //std::cout << curs;
        switch (curs)
        {
        case 0:
            glRasterPos2i(380, 220);
            glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);

            break;
        case 1: 
            glRasterPos2i(370, 420);
            glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);
            break;
        case 2:
            glRasterPos2i(350, 620);
            glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);
            break;
        case 3: 
            glRasterPos2i(720, 220);
            glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);

            break;
        case 4:
            glRasterPos2i(700, 420);
            glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);

            break;
        case 5:
            glRasterPos2i(730, 620);
            glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);

            break;
        default:
            glRasterPos2i(380, 220);
            glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);

            break;
        }
        


        glColor3f(49, 53, 83);
        glRasterPos2i(180, 200);
        YsGlDrawFontBitmap20x28("A star");

        glRasterPos2i(100, 400);
        YsGlDrawFontBitmap20x28("Weighted A star");

        glRasterPos2i(150, 600);
        YsGlDrawFontBitmap20x28("Dijkstra");
        
        glColor3f(0, 0, 0);
        
        glRasterPos2i(570, 200);
        YsGlDrawFontBitmap20x28("RRT");

        glRasterPos2i(500, 400);
        YsGlDrawFontBitmap20x28("RRT Connect");

        glRasterPos2i(530, 600);
        YsGlDrawFontBitmap20x28("RRT Star");
        
        FsSwapBuffers();

    }
    
    
    //int PLA = 3;
    
    //std::cout << "Enter PLA, 0 for grid based and 1 for tree based planner: "; // no flush needed
    //std::cin >> PLA;
    if (curs < 3)
    {
        GridVisual a = GridVisual(800, 800);// set weight to 1 for A* set weight to >1 for weight A* 0 for Dijkstra
        
        
        //int planner = 2;
        if (curs == 2) { // Dijkstra
            a.SetWeight(0.0);
        }
        else if (curs == 0) { // A*
            a.SetWeight(1.0);
        }
        else if (curs == 1){ // Weighted A*
            a.SetWeight(3.0);
        }

        a.visualize();
        
        
    }
    else
    {
        //FsOpenWindow(32, 32, 800, 600, 1);
        YsSoundPlayer player;
        YsSoundPlayer::SoundData wav1;
        if (YSOK != wav1.LoadWav("bensound-scifi.wav"))
        {
            printf("Failed to read %s\n", "bensound-scifi.wav");

        }
        player.Start();
        player.PlayOneShot(wav1);
        rrt r1;
        rrtconnect r2;
        rrtstar r3;
        double* map{ 0 };
        int numofDOFs = 2;
        int x_size = 800;
        int y_size = 800;
        CSpaceVertex start = { 5,5 };
        CSpaceVertex goal = { 600,500 };
        std::vector<CSpaceVertex> pathRRT;
        std::vector<CSpaceVertex> pathRRTConnect;
        std::vector<CSpaceVertex> pathRRTStar;
        std::vector <double> obst;
        int s = 0;
        int e = 0;
        int o = 0;
        int t = 0;
        int ot = 0;

        
        for (;;)
        {
            FsPollDevice();
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            auto key = FsInkey();

            int lb, mb, rb, mx, my;
            auto evt = FsGetMouseEvent(lb, mb, rb, mx, my);


            if (FSKEY_ESC == key)
            {
                break;
            }

            if (FSKEY_S == key)
            {
                double px = mx;
                double py = my;
                start[0] = px;
                start[1] = py;
                s = 1;

                //printf("%d %d\n",px,py);
            }

            if (FSKEY_E == key)
            {
                double px = mx;
                double py = my;
                goal[0] = px;
                goal[1] = py;
                e = 1;
                //printf("%d %d\n",px,py);
            }
            if (FSKEY_O == key)
            {
                double px = mx;
                double py = my;
                obst.push_back(px);
                obst.push_back(py);
                //std::cout << px, py << std::endl;
                o = 1;
            }
            if (FSKEY_T == key)
            {
                double px = mx;
                double py = my;
                if (o == 1)
                {
                    int size = obst.size();
                    obst.push_back(px - obst[size - 2]);
                    obst.push_back(py - obst[size - 1]);
                    //o = 0;
                }

                t = 1;
            }
            if (FSKEY_ENTER == key)
            {
                break;
            }
            if (s == 1 && e == 1)
            {
                DrawGoalandstart(start, goal);
            }
            if (o == 1 && t == 1)
            {
                Drawobstacle(obst);
            }
            FsSwapBuffers();
        }
        
        if (curs == 3)
        pathRRT = r1.plannerRRT(map, x_size, y_size, start, goal, numofDOFs, obst);
        else if (curs == 4)
        pathRRTConnect = r2.plannerRRTConnect(map, x_size, y_size, start, goal, numofDOFs, obst);
        else if (curs == 5)
        pathRRTStar = r3.plannerRRTStar(map, x_size, y_size, start, goal, numofDOFs, obst);
        else
            {
            std::cout <<"Invalid";
            return 0;
            }
        player.Stop(wav1);
    }
    return 0;
}

