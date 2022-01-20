//
//  AStarVisualize.cpp
//  Map Visualization
//
//  Created by William Lee on 11/21/21.
//

#include "GridVisual.hpp"
#include "Map.hpp"
#include "fssimplewindow.h"
#include "astar.hpp"
#include<list>
#include<stack>
#include<set>
#include "yssimplesound.h"
#include "yspng.h"
#include "yspngenc.h"
#include "ysglfontdata.h"
#include <stdio.h>
#include <time.h>
#define R 20
#define C 20
using namespace std;

typedef pair <int, int> points;


GridVisual::GridVisual(int wx,int wy){
    windowx = wx;
    windowy = wy;
    pixelw = windowx/C;
    pixelh = windowy/R;
}
void GridVisual::SetWeight(double w){
    weight = w;
}

void GridVisual::visualize(){
    
    Map bmp;
    bmp.Create(C,R);
    
    int map[R][C] = {0};
    
    points start,end;
    
    
    //FsOpenWindow(0,0,windowx,windowy,1);
    YsSoundPlayer player;
    YsSoundPlayer::SoundData wav1;
    if (YSOK != wav1.LoadWav("bensound-creativeminds.wav"))
    {
        printf("Failed to read %s\n", "bensound-creativeminds.wav");

    }
    player.Start();
    player.PlayOneShot(wav1);
    bool s = false, e = false;
    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();

        int lb,mb,rb,mx,my;
        auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);

        if(FSKEY_ESC==key)
        {
            break;
        }
        
        if(FSKEY_S==key && !s){
            int px=mx/pixelw;
            int py=my/pixelh;
            s = true;
            bmp.SetPixel(px,py,1);
            start = std::make_pair(px, py);
            
            printf("%d %d\n",px,py);
        }
        else if (FSKEY_S==key && s){
            bmp.SetPixel(start.first,start.second,7);
            int px=mx/pixelw;
            int py=my/pixelh;
            bmp.SetPixel(px,py,1);
            start = std::make_pair(px, py);
            
            printf("%d %d\n",px,py);
            
        }
        if(FSKEY_E==key&& !e){
            int px=mx/pixelw;
            int py=my/pixelh;
            e = true;
            bmp.SetPixel(px,py,2);
            end = std::make_pair(px, py);
            printf("%d %d\n",px,py);
        }
        else if (FSKEY_E==key && e){
            bmp.SetPixel(end.first,end.second,7);
    
            int px=mx/pixelw;
            int py=my/pixelh;
            bmp.SetPixel(px,py,2);
            end = std::make_pair(px, py);
            
            printf("%d %d\n",px,py);
        }
        
        
        if(FSKEY_ENTER ==key && s && e){
            break;
        }
        if(FSKEY_O==key){
            int px=mx/pixelw;
            int py=my/pixelh;
            if(bmp.GetPixel(px,py) == 7){
                bmp.SetPixel(px, py, 0);
                map[px][py] = 1;
            }
            else if (bmp.GetPixel(px,py)==0){
                bmp.SetPixel(px, py, 7);
                map[px][py] = 0;
            }
            
        }
        
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        bmp.Draw(pixelw,pixelh);

        FsSwapBuffers();

        FsSleep(25);
    }
    
    astar(map, start, end, bmp,weight);
    bmp.SetPixel(start.first,start.second,1);
    bmp.SetPixel(end.first,end.second,2);
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
    time_t start_time = time(NULL);
    // *****************************************
    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();

        if(FSKEY_ESC==key)
        {
            break;
        }


        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        
        // Goal reached Img **********************
        if (int(time(NULL) - start_time) > 2)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glRasterPos2i(0, 799);
            glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);
            glColor3f(0, 1, 0);
            glRasterPos2i(275, 675);
            YsGlDrawFontBitmap20x28("Goal Reached");
            //FsSwapBuffers();
            //FsSleep(7000);
            //break;
        }
        else
        {
            bmp.Draw(pixelw,pixelh);
        }
        // ******************************************

        FsSwapBuffers();

        FsSleep(25);
    }
    player.Stop(wav1);
    
}
