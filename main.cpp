
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <math.h>
#include "csr.h"

// for compile: g++ -o b.out main.cpp -g
// gdb b.out --> run --> quit


template <typename T>
void metricCalculator(CSR<T> *new_csr,std::string path, bool option)
{
    std::string line = "";
    std::ifstream input(path);
    int sizemax = 0;
    if (input.is_open())
    {       
        while (getline(input, line))
        {
            sizemax++;
        }       
    }
    float *result_array = new float[int(sizemax)*6];
    for (int i = 0; i < sizemax; i++)
    {
        result_array[i] = 0;
    }
    int array_tracker = 0;
    
    std::ifstream input2(path);
    if(input2.is_open())
    {
        line = "";
        
        while (getline(input2,line))
        {
            
            std::istringstream iss (line);
            int firstnode,secondnode;
            iss >> firstnode;
            iss >> secondnode;
            int degree_firstnode = new_csr->V[firstnode + 1]-new_csr->V[firstnode];
            int degree_secondnode = new_csr->V[secondnode + 1] - new_csr->V[secondnode];
            if (!option) // looplari dahil ettigimiz opsiyon
            {                
                int union_degree_nodes = degree_firstnode + degree_secondnode + 2;
                int intersection = 2;
                float pa_score = (degree_firstnode + 1) * (degree_secondnode + 1);               
                float sum_rai = float(1)/degree_firstnode + float(1)/degree_secondnode ;
                float sum_aai = float(1)/log(degree_firstnode) + float(1)/log(degree_secondnode);
                for (int m = new_csr->V[int(firstnode)]; m < new_csr->V[int(firstnode+1)]; m++)
                {
                    for (int n = new_csr->V[int(secondnode)]; n < new_csr->V[int(secondnode + 1)]; n++)
                    {

                       // std::cout<<"nodes are "<< new_csr->E[m] <<" and " << new_csr->E[n]<< std::endl;

                        if (new_csr->E[m] == new_csr->E[n])
                        {
                            //std::cout<<"intersected nodes are "<< new_csr->E[m] <<" and " << new_csr->E[n]<< std::endl;
                            intersection++;
                            float temp_rai = float(1) / ((new_csr->V[new_csr->E[m]+1]) - (new_csr->V[new_csr->E[m]]));
                            float temp_aai = float(1) / log((new_csr->V[new_csr->E[m]+1]) - (new_csr->V[new_csr->E[m]]));                            

                            sum_rai += temp_rai;
                            sum_aai += temp_aai;
                        }
                        
                    }
                }
                float jackard = float(float(intersection) / float(union_degree_nodes - intersection)); 
                // res array = [firstnode seconnode jackard rai aai pa]
                result_array[array_tracker] = firstnode;
                array_tracker++;
                result_array[array_tracker] = secondnode;
                array_tracker++;
                result_array[array_tracker] = jackard;
                array_tracker++;
                result_array[array_tracker] = sum_rai;
                array_tracker++;
                result_array[array_tracker] = sum_aai;
                array_tracker++;
                result_array[array_tracker] = pa_score;
                array_tracker++;
            }
            else
            {
                int union_degree_nodes = degree_firstnode + degree_secondnode;
                int intersection = 0;
                float pa_score = (degree_firstnode) * (degree_secondnode);               
                float sum_rai = 0;
                float sum_aai = 0;
                for (int m = new_csr->V[int(firstnode)]; m < new_csr->V[int(firstnode+1)]; m++)
                {
                    for (int n = new_csr->V[int(secondnode)]; n < new_csr->V[int(secondnode + 1)]; n++)
                    {

                       // std::cout<<"nodes are "<< new_csr->E[m] <<" and " << new_csr->E[n]<< std::endl;

                        if (new_csr->E[m] == new_csr->E[n])
                        {
                            //std::cout<<"intersected nodes are "<< new_csr->E[m] <<" and " << new_csr->E[n]<< std::endl;
                            intersection++;
                            float temp_rai = float(1) / ((new_csr->V[new_csr->E[m]+1]) - (new_csr->V[new_csr->E[m]]));
                            float temp_aai = float(1) / log((new_csr->V[new_csr->E[m]+1]) - (new_csr->V[new_csr->E[m]]));                            

                            sum_rai += temp_rai;
                            sum_aai += temp_aai;
                        }
                        
                    }
                }
                float jackard = float(float(intersection) / float(union_degree_nodes - intersection)); 
                // res array = [firstnode seconnode jackard rai aai pa]
                result_array[array_tracker] = firstnode;
                array_tracker++;
                result_array[array_tracker] = secondnode;
                array_tracker++;
                result_array[array_tracker] = jackard;
                array_tracker++;
                result_array[array_tracker] = sum_rai;
                array_tracker++;
                result_array[array_tracker] = sum_aai;
                array_tracker++;
                result_array[array_tracker] = pa_score;
                array_tracker++;
            }
        }
        
    }
    // int k =1;
    // std::cout << path << std::endl;
    // for (int t = 0; t < array_tracker; t+=6)
    // {
    //     //pa << fin_array_pa[t] << " " << fin_array_pa[t+1] << " " << fin_array_pa[t+2] << std::endl;
    //     std::cout<< k << "th line " << result_array[t] << " " << result_array[t+1] << " " << result_array[t+2]<< " " << result_array[t+3]<< " " << result_array[t+4]<< " " << result_array[t+5] << std::endl;
    //     k++;
    // }
    std::string opt = "";
    if (option)
    {
        opt = "option2";
    }
    else
    {
        opt = "option1";
    }
    
    path = path.substr(0,path.rfind('.')) +"_"+opt+ "_feats.bin";
    std::ofstream output(path);
    for (int t = 0; t < array_tracker; t+=6)
    {
        //pa << fin_array_pa[t] << " " << fin_array_pa[t+1] << " " << fin_array_pa[t+2] << std::endl;
        output << result_array[t] << " " << result_array[t+1] << " " << result_array[t+2]<< " " << result_array[t+3]<< " " << result_array[t+4]<< " " << result_array[t+5];
        if (t != array_tracker - 6)
        {
            output << "\n";
        }
        
    }
}

int main()
{
    


    std::cout << "Welcome to our similarity metric calculator...\n" << std::endl;
    
    // Notes from the meetings

    // take graph generate 2 way embedding,print out two way embedding edges, positive ones. print 
    // calculate the metrics between 2 edges. output a |E| * 2 x |M| contains two way of (1,2), (3,4), (5,6)
    // times 2 because there are non existing connections such as (1,6), (3,2), (1,5) 

    string path = "/home/mutku/Desktop/gosh/graphs/train_test_split/10graph.graph0.8/";
    CSR<unsigned int> *new_csr = new CSR<unsigned int>(path+"ptrain.graph", false,false);
    
    std::cout << "Total number of vertices in this graph: " << new_csr->num_vertices << std::endl;

    auto start = std::chrono::steady_clock::now();

    metricCalculator(new_csr,path+"ptrain.graph",true);
    metricCalculator(new_csr,path+"ptrain.graph",false);
    metricCalculator(new_csr,path+"ntrain.graph",true);
    metricCalculator(new_csr,path+"ntrain.graph",false);
    metricCalculator(new_csr,path+"ptest.graph",true);
    metricCalculator(new_csr,path+"ptest.graph",false);
    metricCalculator(new_csr,path+"ntest.graph",true);
    metricCalculator(new_csr,path+"ntest.graph",false);


    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;

    //std::cout <<"Total not connected pair size: "<< (newsize + 1)/3 <<std::endl;
    std::cout <<"Time elapsed to calculate the metrics: "<< elapsed_seconds.count() << "s\n\n";

    // printing out edges and vertices array from csr (to check if it is working properly)
    //
    // for (unsigned int i = 0; i < new_csr->num_vertices; i++)
    // {
    //     //std::cout << new_csr->E[i] << std::endl;
    //     std::cout << "right now i = " << i << std::endl;
    //     for (unsigned int j = new_csr->V[i]; j < new_csr->V[i+1]; j++)
    //     {
    //         std::cout << new_csr->E[j]<<std::endl;
            
    //     }
    // }

    

    // rai = Resource Allocation Index
    // aai = Adamic-Agar Index
    // jackard = Jackard Coefficient
    // pa = Preferential Attachments

    // metric result array (fin_array_***) initializations
    // this array contains (vertex1 (n), vertex2 (n+1), similarity score (n+2))
    // since the maximum number of two pair of vertices = num_vertices * num_vertices 
    // and the third index contains the similarity score
    // so we assumed that max size for the array will be num_vertices * num_vertices * 3 it is donated below as sizemax

    //int sizemax = new_csr->num_vertices * new_csr->num_vertices * 3; // unsigned long long
    //float *fin_array_rai = new float[int(sizemax)];

    // int sizemax = new_csr->num_edges*2;
    // float *result_array = new float[int(sizemax)];
    // for (int i = 0; i < sizemax; i++)
    // {
    //     result_array[i] = 0;
    // }
    /*float *fin_array_aai = new float[int(sizemax)];

    for (int i = 0; i < sizemax; i++)
    {
        fin_array_aai[i] = 0;
    }
    float *fin_array_jackard = new float[int(sizemax)];

    for (int i = 0; i < sizemax; i++)
    {
        fin_array_jackard[i] = 0;
    }

    float *fin_array_pa= new float[int(sizemax)];

    for (int i = 0; i < sizemax; i++)
    {
        fin_array_pa[i] = 0;
    }*/
    
    // newsize tracks down the last modified indexes of result arrays 

    //int newsize = -1; // can be started from 0 

    // // std::string path_ptrain = "";
    // // std::string path_ntrain = "";
    // // bool option = false;
    // // int array_tracker = 0;
    
    // // std::ifstream inputp(path_ptrain);
    // // if(inputp.is_open())
    // // {
    // //     std::string line = "";
        
    // //     while (getline(inputp,line))
    // //     {
            
    // //         std::istringstream iss (line);
    // //         int firstnode,secondnode;
    // //         iss >> firstnode;
    // //         iss >> secondnode;
    // //         int degree_firstnode = new_csr->V[firstnode + 1]-new_csr->V[firstnode];
    // //         int degree_secondnode = new_csr->V[secondnode + 1] - new_csr->V[secondnode];
    // //         if (!option) // looplari dahil ettigimiz opsiyon
    // //         {                
    // //             int union_degree_nodes = degree_firstnode + degree_secondnode + 2;
    // //             int intersection = 2;
    // //             float pa_score = (degree_firstnode + 1) * (degree_secondnode + 1);               
    // //             float sum_rai = float(1)/degree_firstnode + float(1)/degree_secondnode ;
    // //             float sum_aai = float(1)/log(degree_firstnode) + float(1)/log(degree_secondnode);
    // //             for (int m = new_csr->V[int(firstnode)]; m < new_csr->V[int(firstnode+1)]; m++)
    // //             {
    // //                 for (int n = new_csr->V[int(secondnode)]; n < new_csr->V[int(secondnode + 1)]; n++)
    // //                 {

    // //                    std::cout<<"nodes are "<< new_csr->E[m] <<" and " << new_csr->E[n]<< std::endl;

    // //                     if (new_csr->E[m] == new_csr->E[n])
    // //                     {
    // //                         std::cout<<"intersected nodes are "<< new_csr->E[m] <<" and " << new_csr->E[n]<< std::endl;
    // //                         intersection++;
    // //                         float temp_rai = float(1) / ((new_csr->V[new_csr->E[m]+1]) - (new_csr->V[new_csr->E[m]]));
    // //                         float temp_aai = float(1) / log((new_csr->V[new_csr->E[m]+1]) - (new_csr->V[new_csr->E[m]]));                            

    // //                         sum_rai += temp_rai;
    // //                         sum_aai += temp_aai;
    // //                     }
                        
    // //                 }
    // //             }
    // //             float jackard = float(float(intersection) / float(union_degree_nodes - intersection)); 
    // //             res array = [firstnode seconnode jackard rai aai pa]
    // //             result_array[array_tracker] = firstnode;
    // //             array_tracker++;
    // //             result_array[array_tracker] = secondnode;
    // //             array_tracker++;
    // //             result_array[array_tracker] = jackard;
    // //             array_tracker++;
    // //             result_array[array_tracker] = sum_rai;
    // //             array_tracker++;
    // //             result_array[array_tracker] = sum_aai;
    // //             array_tracker++;
    // //             result_array[array_tracker] = pa_score;
    // //             array_tracker++;
    // //         }
    // //         else
    // //         {
    // //             int union_degree_nodes = degree_firstnode + degree_secondnode;
    // //             int intersection = 0;
    // //             float pa_score = (degree_firstnode) * (degree_secondnode);               
    // //             float sum_rai = 0;
    // //             float sum_aai = 0;
    // //             for (int m = new_csr->V[int(firstnode)]; m < new_csr->V[int(firstnode+1)]; m++)
    // //             {
    // //                 for (int n = new_csr->V[int(secondnode)]; n < new_csr->V[int(secondnode + 1)]; n++)
    // //                 {

    // //                    std::cout<<"nodes are "<< new_csr->E[m] <<" and " << new_csr->E[n]<< std::endl;

    // //                     if (new_csr->E[m] == new_csr->E[n])
    // //                     {
    // //                         std::cout<<"intersected nodes are "<< new_csr->E[m] <<" and " << new_csr->E[n]<< std::endl;
    // //                         intersection++;
    // //                         float temp_rai = float(1) / ((new_csr->V[new_csr->E[m]+1]) - (new_csr->V[new_csr->E[m]]));
    // //                         float temp_aai = float(1) / log((new_csr->V[new_csr->E[m]+1]) - (new_csr->V[new_csr->E[m]]));                            

    // //                         sum_rai += temp_rai;
    // //                         sum_aai += temp_aai;
    // //                     }
                        
    // //                 }
    // //             }
    // //             float jackard = float(float(intersection) / float(union_degree_nodes - intersection)); 
    // //             res array = [firstnode seconnode jackard rai aai pa]
    // //             result_array[array_tracker] = firstnode;
    // //             array_tracker++;
    // //             result_array[array_tracker] = secondnode;
    // //             array_tracker++;
    // //             result_array[array_tracker] = jackard;
    // //             array_tracker++;
    // //             result_array[array_tracker] = sum_rai;
    // //             array_tracker++;
    // //             result_array[array_tracker] = sum_aai;
    // //             array_tracker++;
    // //             result_array[array_tracker] = pa_score;
    // //             array_tracker++;
    // //         }
            

            
                
    // //     }
        
    // // }
    // // for(int i= 0; i<new_csr->num_edges; i++)
    // // {
    // //     int intersection = 0;
    // //     for (int k = 0; k < new_csr->num_edges; k++)
    // //     {
    // //         int unionsize = new_csr->V[i+1]-new_csr->V[i] + new_csr->V[k+1] - new_csr->V[k];
    // //     }
        
        
    // // }

    // // starts with the first node starting from 0 
    // // for (int i = 0; i < new_csr->num_vertices; i++)
    // // {
    // //     initializing the not connected array of one vertex
    // //     for ex if num_vertices = 7, temparr = [0,1,2,3,4,5,6]

    // //     int temparr[new_csr->num_vertices];
    // //     for (int i = 0; i < new_csr->num_vertices; i++)
    // //     {
    // //         temparr[i] = i;
    // //         E = [1,2,3,0,3,4,0,3,6,0,1,2,4,5,1,3,3,6,2,5]
    // //         V = [0,3,6,9,14,16,18,20]
    // //     }
        
    // //     here we modify temparr according to vertex's neighbours and marking their indexes as -1
    // //     for ex, neighbours of index 0 = (1,2,3), temparr becomes [-1,-1,-1,-1,4,5,6]
    // //     we use this to find its non-neighbours.
    // //     temparr[i] = -1; // also it cant be a neighbour of itself
    // //     for (int j = new_csr->V[i]; j < new_csr->V[i+1]; j++)
    // //     {
    // //         temparr[int(new_csr->E[j])] = -1;          
    // //     }

    // //     printing temparr to debug
    // //     for (int y = 0; y < new_csr->num_vertices; y++)
    // //     {
    // //         std::cout<< temparr[y] << ", ";
    // //     }
    // //     std::cout<< std::endl;

    // //     here we will be computing each non-neighbour pairs
    // //     for (int k = 0; k < new_csr->num_vertices; k++)
    // //     {
    // //         nonequal check for -1 is to find non connected pairs
    // //         also we are checking if a pair repeats itself or not
    // //         since this is an undirected graph each edge exist twice in edge array
    // //         for ex: we take (0,4), but when it comes to (4,0) if condition fails since i < k (4 < 0) is not true 
    // //         if (temparr[k] != -1 && i < k) // not connected and not repeated
    // //         {
    // //             add the non-connected pair to result array 
    // //             before adding them to the array increment newsize by 1
    // //             newsize++;
    // //             fin_array_rai[newsize] = i;
    // //             /*fin_array_aai[newsize] = i;
    // //             fin_array_jackard[newsize] = i;
    // //             fin_array_pa[newsize] = i;*/
            
    // //             newsize++;
    // //             fin_array_rai[newsize] = k;
    // //             /*fin_array_aai[newsize] = k;
    // //             fin_array_jackard[newsize] = k;
    // //             fin_array_pa[newsize] = k;*/

    // //             needed variables for metric calculations
    // //             /*int intersection = 0;
    // //             int union_nodes = 0;*/
    // //             float sum_rai = 0;
    // //             union_nodes = new_csr->V[i+1]-new_csr->V[i] + new_csr->V[k+1] - new_csr->V[k];
    // //             float sum_aai = 0;
                

    // //             here we check the common neighbours of non-connected pairs
    // //             we are finding the connections of vertices using infos from V and E array
    // //             for (int m = new_csr->V[int(fin_array_rai[newsize-1])]; m < new_csr->V[int(fin_array_rai[newsize-1]+1)]; m++)
    // //             {
    // //                 for (int n = new_csr->V[int(fin_array_rai[newsize])]; n < new_csr->V[int(fin_array_rai[newsize] + 1)]; n++)
    // //                 {

    // //                    std::cout<<"nodes are "<< new_csr->E[m] <<" and " << new_csr->E[n]<< std::endl;

    // //                    If there is a common neighbor, we do the following calculations for 
    // //                    jackard, adamic agar and resource allocation
    // //                    preferential attachment not related with the common neighbors
    // //                     if (new_csr->E[m] == new_csr->E[n])
    // //                     {
    // //                         std::cout<<"intersected nodes are "<< new_csr->E[m] <<" and " << new_csr->E[n]<< std::endl;
    // //                         intersection++;
    // //                         float temp_rai = float(1) / ((new_csr->V[new_csr->E[m]+1]) - (new_csr->V[new_csr->E[m]]));
    // //                         float temp_aai = float(1) / log((new_csr->V[new_csr->E[m]+1]) - (new_csr->V[new_csr->E[m]]));
                            

    // //                         sum_rai += temp_rai;
    // //                         sum_aai += temp_aai;
    // //                     }
                        
    // //                 }
    // //                 here we add our results to the result arrays
    // //                 fin_array_rai[newsize+1] = sum_rai;
    // //                 /*fin_array_aai[newsize+1] = sum_aai;
                    
    // //                 fin_array_jackard[newsize+1] = float(float(intersection) / float(union_nodes-intersection)); 
    // //                 float pa_score = (new_csr->V[i+1]-new_csr->V[i]) * (new_csr->V[k+1]-new_csr->V[k]);
    // //                 fin_array_pa[newsize+1] = pa_score;*/
    // //             }
    // //             incrementing this to add the next pair of non-connected vertices to our result array
    // //             newsize++;               
    // //         }
            
    // //     }
        
    // // }
    
    //ofstream rai, aai, jackard, pa;
    //rai.open("rai.txt");
    //aai.open("aai.txt");
    //jackard.open("jackard.txt");
    //pa.open("pa.txt");

    
    /*int k = 1;

    for (int t = 0; t < newsize+1; t+=3)
    {
        //rai << fin_array_rai[t] << " " << fin_array_rai[t+1] << " " << fin_array_rai[t+2] << std::endl;
        std::cout<< k << "th rai = " << fin_array_rai[t] << " " << fin_array_rai[t+1] << " " << fin_array_rai[t+2] << std::endl;
        k++;
    }
    std::cout<< "-----------------------------" << std::endl;
    k = 1;
    for (int t = 0; t < newsize+1; t+=3)
    {
        //aai << fin_array_aai[t] << " " << fin_array_aai[t+1] << " " << fin_array_aai[t+2] << std::endl;
        std::cout<< k << "th aai = " << fin_array_aai[t] << " " << fin_array_aai[t+1] << " " << fin_array_aai[t+2] << std::endl;
        k++;
    }
    std::cout<< "-----------------------------" << std::endl;
    k = 1;
    for (int t = 0; t < newsize+1; t+=3)
    {
        //jackard << fin_array_jackard[t] << " " << fin_array_jackard[t+1] << " " << fin_array_jackard[t+2] << std::endl;
        std::cout<< k << "th jackard = " << fin_array_jackard[t] << " " << fin_array_jackard[t+1] << " " << fin_array_jackard[t+2] << std::endl;
        k++;
    }
    std::cout<< "-----------------------------" << std::endl;
    k = 1;
    for (int t = 0; t < newsize+1; t+=3)
    {
        //pa << fin_array_pa[t] << " " << fin_array_pa[t+1] << " " << fin_array_pa[t+2] << std::endl;
        std::cout<< k << "th pa = " << fin_array_pa[t] << " " << fin_array_pa[t+1] << " " << fin_array_pa[t+2] << std::endl;
        k++;
    }*/

    
    
    
    
    return 0;

}
