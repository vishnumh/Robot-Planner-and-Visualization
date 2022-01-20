#include "Main.hpp"
#include "GridVisual.hpp"
#include "rrt.h"
#include "rrtconnect.h"
#include "rrtstar.h"
#include "visualize_rrt.h"


int main(void)
{
    int PLA = 3;
    std::cout << "Enter PLA, 0 for grid based and 1 for tree based planner: "; // no flush needed
    std::cin >> PLA;
    if(PLA == 0)
    {
        GridVisual a = GridVisual(800,800);// set weight to 1 for A* set weight to >1 for weight A* 0 for Dijkstra
        int planner = 2;
        if(planner == 0){ // Dijkstra
            a.SetWeight(0.0);
        }
        else if (planner == 1){ // A*
            a.SetWeight(1.0);
        }
        else{ // Weighted A*
            a.SetWeight(3.0);
        }
        
        a.visualize();
    }
    else
    {
        rrt r1;
        rrtconnect r2;
        rrtstar r3;
        double* map{ 0 };
        int numofDOFs = 2;
        int x_size = 800;
        int y_size = 600;
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

        FsOpenWindow(32, 32, 800, 600, 1);
        for(;;)
        {
            FsPollDevice();
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            auto key=FsInkey();

            int lb,mb,rb,mx,my;
            auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);


            if(FSKEY_ESC==key)
            {
                break;
            }
            
            if(FSKEY_S==key)
            {
                double px=mx;
                double py=my;
                start[0] = px;
                start[1] = py;
                s = 1;
                
                //printf("%d %d\n",px,py);
            }

            if(FSKEY_E==key)
            {
                double px=mx;
                double py=my;
                goal[0] = px;
                goal[1] = py;
                e = 1;
                //printf("%d %d\n",px,py);
            }
            if(FSKEY_O==key)
            {
                double px=mx;
                double py=my;
                obst.push_back(px);
                obst.push_back(py);
                //std::cout << px, py << std::endl;
                o = 1;
            }
            if(FSKEY_T==key)
            {
                double px=mx;
                double py=my;
                if(o == 1)
                {
                    int size = obst.size();
                    obst.push_back(px - obst[size - 2]);
                    obst.push_back(py - obst[size - 1]);
                    //o = 0;
                }

                t = 1;
            }
            if(FSKEY_ENTER ==key)
            {
                break;
            }
            if(s == 1 && e == 1)
            {
                DrawGoalandstart(start,  goal);
            }
            if(o == 1 && t == 1)
            {
                Drawobstacle(obst);   
            }
        FsSwapBuffers(); 
        }

        //pathRRT = r1.plannerRRT(map, x_size, y_size, start, goal, numofDOFs, obst);
        pathRRTConnect = r2.plannerRRTConnect(map, x_size, y_size, start, goal, numofDOFs, obst);
        //pathRRTStar = r3.plannerRRTStar(map, x_size, y_size, start, goal, numofDOFs, obst);
    }
}

