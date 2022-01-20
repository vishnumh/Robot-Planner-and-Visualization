// rrt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "fssimplewindow.h"
#include "rrt.h"




    bool rrt::IsValidConfiguration(int size_x, int size_y, double* map, CSpaceVertex q, std::vector <double> obst)
        // May make class map with obstacles and and map size
    {
        int size = obst.size() / 4;

        if (q[0] > size_x || q[1] > size_y)
        {
            return false;
        }

        for (int i = 0; i < size; i++)
        {
            int start = 4 * i;
            double obstaclex = obst[start];
            double obstacley = obst[start + 1];
            double obstaclew = obst[start + 2];
            double obstacleh = obst[start + 3];

            if(obstaclew < 0)
            {
                obstaclex = obstaclex + obstaclew;
                obstaclew = fabs(obstaclew);
            }
            if(obstacleh < 0)
            {
                obstacley = obstacley + obstacleh;
                obstacleh = fabs(obstacleh);
            }

            double relativeX, relativeY;
            relativeX = q[0] - obstaclex;
            relativeY = q[1] - obstacley;
            if (0 <= relativeX && relativeX < obstaclew && 0 <= relativeY && relativeY < obstacleh)
            {
                return false;
            }
        }
        return true;
    }

    CSpaceVertex rrt::sampleRandomVertex(double* map, int x_size, int y_size, std::vector <double> obst)
    {

        while (1)
        {
            std::random_device rd;  // Will be used to obtain a seed for the random number engine
            std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
            // std::nextafter(2.0*M_PI, std::numeric_limits<RealType>::max()
            std::uniform_real_distribution<> dis(0, std::max(x_size, y_size));
            CSpaceVertex sampleVertex;
            for (int n = 0; n < 2; n++)
            {
                // Use dis to transform the random unsigned int generated by gen into a 
                // double in [1, 2). Each call to dis(gen) generates a new random double

                const double a = dis(gen);
                sampleVertex.push_back(a);

            }

            if (IsValidConfiguration(x_size, y_size, map, sampleVertex, obst))
            {

                return sampleVertex;
            }
            else
            {
                //std::cout << "Sample generated is not valid. Generate new sample" << std::endl;
            }
        }

    }

    float rrt::VertexDistance(CSpaceVertex v1, CSpaceVertex v2)
    {
        float sum = 0;
        for (int i = 0; i < v1.size(); i++)
        {
            sum += abs((v1[i] - v2[i]) * (v1[i] - v2[i]));


        }
        return sqrt(sum);
    }

    void rrt::printCSpaceVertex(CSpaceVertex v)
    {
        for (int i = 0; i < v.size(); i++)
        {
            std::cout << v[i] << ",";
        }
        std::cout << std::endl;
    }

    int rrt::RRTNearestNeighbour(Tree& tree, CSpaceVertex q_rand) //, int numofDOFs, double*	map,int x_size, int y_size
    {

        int size = tree.size();
        double min_dist = 1000000; // Initialize very high value
        double dist;
        int nn_index; // Index of nearest neighbour
        for (int i = 0; i < size; i++)
        {
            dist = VertexDistance(q_rand, tree[i]->vertex);
            if (dist < min_dist)
            {
                min_dist = dist;
                nn_index = i;

            }
        }

        return nn_index;

    }

    bool rrt::LocalPlanner(double* map, int x_size, int y_size, CSpaceVertex v1, CSpaceVertex v2, int num_steps, std::vector <double> obst)
    {
        // From v1 to v2
        int size = v1.size();
        CSpaceVertex increment;
        for (int i = 0; i < size; i++)
        {
            increment.push_back((v2[i] - v1[i]) / num_steps);
        }

        for (int i = 0; i < num_steps; i++)
        {
            // CSpaceVertex temp;
            for (int j = 0; j < size; j++)
            {
                v1[j] = v1[j] + increment[j];
            }

            if (IsValidConfiguration(x_size, y_size, map, v1, obst)) continue;
            else return false;
        }

        return true;
    }

    CSpaceVertex rrt::AltRRTGenerateNewVertex(Tree& tree, CSpaceVertex q_rand, int q_near_index, double max_step, int numofDOFs, double* map,
        int x_size, int y_size, int num_steps, std::vector <double> obst)
    {

        CSpaceVertex q_new; // Initialize q_new


        double dist = VertexDistance(q_rand, tree[q_near_index]->vertex);

        if (dist < max_step)
        {
            return q_rand; // The random vertex is itself the new vertex
        }



        else
        {
            double resolution = 0.001;
            dist = 0;

            CSpaceVertex q_near = tree[q_near_index]->vertex;
            CSpaceVertex q_new_int = q_near;
            CSpaceVertex increment;

            for (int i = 0; i < q_near.size(); i++)
            {
                if (q_rand[i] > q_near[i]) increment.push_back(1);
                else increment.push_back(-1);
            }

            while (dist < max_step)
            {

                for (int i = 0; i < q_rand.size(); i++) // Increase each joint config linearly by value ofresolution
                {
                    if (increment[i] == 1)
                    {
                        q_new_int[i] = q_new_int[i] + increment[i] * resolution;
                        if (q_new_int[i] > q_rand[i]) q_new_int[i] = q_rand[i];
                    }

                    if (increment[i] == -1)
                    {
                        q_new_int[i] = q_new_int[i] + increment[i] * resolution;
                        if (q_new_int[i] < q_rand[i]) q_new_int[i] = q_rand[i];
                    }
                }

                //   double arr[5];

                //   for(int i = 0; i < q_rand.size();i++) 
                //   {
                //     arr[i] = q_new_int[i];
                //   }

                if (IsValidConfiguration(x_size, y_size, map, q_new_int, obst) && VertexDistance(q_near, q_new_int) < max_step)
                {
                    //std::cout << "Potential new node is valid and within max_step" << std::endl;
                    if (LocalPlanner(map, x_size, y_size, q_near, q_new_int, num_steps, obst))
                    {
                        //std::cout << "Local Planner can find valid path " << std::endl;
                        dist = VertexDistance(q_near, q_new_int);
                        q_new = q_new_int;
                    }

                    else break;


                }
                else break;


            }

        }

        return q_new; // If q_new returned is empty, then no new node was created, generate random node again 

    }


    void rrt::RRTAddVertex(Tree& tree, CSpaceVertex q_new, int nn_index, int& count)
    {

        RRTNode* temp = new RRTNode;
        temp->index = count;
        temp->parent_index = nn_index;
        temp->vertex = q_new;
        temp->cost = tree[nn_index]->cost + VertexDistance(q_new, tree[nn_index]->vertex); // Is it correct?

        tree.push_back(temp);
        count++;

    }

    std::vector<CSpaceVertex> rrt::plannerRRT(
        double* map,
        int x_size,
        int y_size,
        CSpaceVertex start,
        CSpaceVertex goal,
        int numofDOFs,
        std::vector <double> obst)
    {
        //no plan by default

        // auto start = std::chrono::high_resolution_clock::now();
        // *plan = NULL;
        // *planlength = 0;
        // double* map;


        Tree tree; // Initialize tree
        int count = 0; // Index in the tree

        int max_iter = 80000; // Number of itrations to run
        std::cout << " Initializing tree. Running for max_iter =  " << max_iter << std::endl;

        double max_step = 10; // Define epsilon for max_step distance
        int num_steps = 20; // Define num_steps for local planner
        int goal_index = 0;
        std::vector<CSpaceVertex> path;

        CSpaceVertex start_pos;
        CSpaceVertex goal_pos;
        CSpaceVertex q_rand;
        CSpaceVertex q_near;
        CSpaceVertex q_new;
        int nn_index; // Nearest neighbour index in tree


        // for(int i = 0; i < numofDOFs; i++)
        // {
        //   start_pos.push_back(armstart_anglesV_rad[i]);
        // }

        start_pos = start;
        goal_pos = goal;


        // for(int i = 0; i < numofDOFs; i++)
        // {
        //   goal_pos.push_back(armgoal_anglesV_rad[i]);
        // }

        RRTNode* start_node = new RRTNode;

        start_node->index = count;
        start_node->parent_index = -1; // -1 as it is root of the tree
        start_node->vertex = start_pos;
        start_node->cost = 0;

        tree.push_back(start_node); // Add root node to tree
        count++;

        /*MAIN FOR LOOP - RUNNING FOR MAX_ITER ITERATIONS */

        for (int i = 0; i < max_iter; i++)
        {

            /* Generate Random Configuration */
            if (i % 10000 == 0 && i != 0)
            {
                std::cout << "Number of iterations is " << i << std::endl;
                std::cout << "Size of tree is " << tree.size() << std::endl;
            }

            if (i % 20 == 0 && i != 0)
            {
                q_rand = goal_pos; // Goal Bias - P = 0.05 --> Selects goal once every 20 times ( as suggested by Max)
            }
            else
            {
                q_rand = sampleRandomVertex(map, x_size, y_size, obst);
            }


            /* Get nearest neighbour */

            nn_index = RRTNearestNeighbour(tree, q_rand);
            q_new = AltRRTGenerateNewVertex(tree, q_rand, nn_index, max_step, 2, map, x_size, y_size, num_steps, obst);
            if (q_new.empty())
            {
                std::cout << "New vertex wasnt able to be Generated. Break" << std::endl;
                continue; // Go to next iterations
            }

            RRTAddVertex(tree, q_new, nn_index, count);

            if (q_new == goal_pos)
            {
                goal_index = count - 1;
                break; // Break outta for loopx
            }



            // while(FSKEY_ESC!=FsInkey())
            //   {

            Draw(tree, obst, start, goal);
            // auto DT=std::chrono::high_resolution_clock::now()-t0;
            // auto millisec=std::chrono::duration_cast <std::chrono::milliseconds> (DT).count();



        }
        printf("DRAW DONEEEEEEEEEE");

        /* BAcktracking to get path */

        std::cout << "Final size of tree is " << tree.size() << std::endl;


        std::cout << "Starting Backtracking ***********" << std::endl;
        if (goal_index == 0) std::cout << "Path wasnt found !!!!!!!!!!!!!!!!!" << std::endl;
        int parent_index = goal_index;
        while (parent_index != -1)
        {
            CSpaceVertex temp = tree[parent_index]->vertex;
            path.push_back(temp);
            parent_index = tree[parent_index]->parent_index;
        }


        // int numofsamples = path.size();
        // if(numofsamples < 2){
        //     printf("the arm is already at the goal\n");
        //     return;
        // }
        // *plan = (double**) malloc(numofsamples*sizeof(double*));
        // int firstinvalidconf = 1;
        // for (int i = 0; i < numofsamples; i++){
        //     (*plan)[i] = (double*) malloc(numofDOFs*sizeof(double)); 
        //     }

        // for(int j =0; j < numofsamples; j++)
        // {
        //   for(int i = 0; i < numofDOFs; i ++)
        //   {
        //     (*plan)[j][i] = path[numofsamples - 1 - j][i];
        //   }
        // }

        // *planlength = numofsamples;

        std::cout << "PLan length is " << path.size() << std::endl;

        // Delete dynamic memory allocated to tree
        std::cout << "Cost of Goal Vertex is " << tree[goal_index]->cost << std::endl;

        for (int i = 0; i < tree.size(); i++)
        {

            delete tree[i];
            tree[i] = nullptr;
        }


        auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> elapsed_seconds = end-start;
        // std::cout << "Time taken for planning is " << elapsed_seconds.count() << std::endl;

        // Print Path


        //for(int i = 0; i < 5; i++)
        //{
        DrawPath(path, obst, start, goal);
        Moverobot(path, obst, start, goal);
        //}
        //FsSleep(3000);


        for (int i = 0; i < path.size(); i++)
        {
            printCSpaceVertex(path[i]);
        }



        return path;
    }
