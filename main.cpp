
#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <math.h>
#include "csr.h"

// for compile: g++ -o b.out main.cpp -g
// gdb b.out --> run --> quit
// 

int main()
{
    


    std::cout << "Welcome to our similarity metric calculator...\n" << std::endl;
    
    // take graph generate 2 way embedding,print out two way embedding edges, positive ones. print 
    // calculate the metrics between 2 edges. output a |E| * 2 x |M| contains two way of (1,2), (3,4), (5,6)
    // times 2 because there are non existing connections such as (1,6), (3,2), (1,5) 

    string path = "facebook_combined_c.graph";
    CSR<unsigned int> *new_csr = new CSR<unsigned int>(path, false,false);
    
    // for (unsigned int i = 0; i < new_csr->num_vertices; i++)
    // {
    //     //std::cout << new_csr->E[i] << std::endl;
    //     std::cout << "right now i = " << i << std::endl;
    //     for (unsigned int j = new_csr->V[i]; j < new_csr->V[i+1]; j++)
    //     {
    //         std::cout << new_csr->E[j]<<std::endl;
            
    //     }
    // }

    std::cout << "Total number of vertices in this graph: " << new_csr->num_vertices << std::endl;

    auto start = std::chrono::steady_clock::now();
    int sizemax = new_csr->num_vertices * new_csr->num_vertices * 3;
    float *fin_array_rai = new float[int(sizemax)];

    for (int i = 0; i < sizemax; i++)
    {
        fin_array_rai[i] = 0;
    }
    float *fin_array_aai = new float[int(sizemax)];

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
    }
    

    int newsize = -1;
    for (int i = 0; i < new_csr->num_vertices; i++)
    {
        int temparr[new_csr->num_vertices];
        for (int i = 0; i < new_csr->num_vertices; i++)
        {
            temparr[i] = i;
        }
        

        temparr[i] = -1;
        for (int j = new_csr->V[i]; j < new_csr->V[i+1]; j++)
        {
            temparr[int(new_csr->E[j])] = -1;          
        }

        // not neighbours arrays are working fine
        // for (int y = 0; y < 7; y++)
        // {
        //     std::cout<< temparr[y] << ", ";
        // }
        // std::cout<< std::endl;

        for (int k = 0; k < new_csr->num_vertices; k++)
        {
            if (temparr[k] != -1 && i < k) // not connected and not repeated
            {
                
                newsize++;
                fin_array_rai[newsize] = i;
                fin_array_aai[newsize] = i;
                fin_array_jackard[newsize] = i;
                fin_array_pa[newsize] = i;
            
                newsize++;
                fin_array_rai[newsize] = k;
                fin_array_aai[newsize] = k;
                fin_array_jackard[newsize] = k;
                fin_array_pa[newsize] = k;
                int intersection = 0;
                int union_nodes = 0;
                float sum_rai = 0;
                float sum_aai = 0;
                
                for (int m = new_csr->V[int(fin_array_rai[newsize-1])]; m < new_csr->V[int(fin_array_rai[newsize-1]+1)]; m++)
                {
                    for (int n = new_csr->V[int(fin_array_rai[newsize])]; n < new_csr->V[int(fin_array_rai[newsize] + 1)]; n++)
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
                        
                    }// calculations
                    fin_array_rai[newsize+1] = sum_rai;
                    fin_array_aai[newsize+1] = sum_aai;
                    union_nodes = new_csr->V[i+1]-new_csr->V[i] + new_csr->V[k+1] - new_csr->V[k] - intersection;
                    fin_array_jackard[newsize+1] = float(float(intersection) / float(union_nodes)); 
                    float pa_score = (new_csr->V[i+1]-new_csr->V[i]) * (new_csr->V[k+1]-new_csr->V[k]);
                    fin_array_pa[newsize+1] = pa_score;
                }
                newsize++;               
            }
            
        }
        
    }
    auto end = std::chrono::steady_clock::now();
    ofstream rai, aai, jackard, pa;
    rai.open("rai.txt");
    aai.open("aai.txt");
    jackard.open("jackard.txt");
    pa.open("pa.txt");

    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout <<"Total not connected pair size: "<< (newsize + 1)/3 <<std::endl;
    std::cout <<"Time elapsed to calculate the metrics: "<< elapsed_seconds.count() << "s\n\n";
    /*int k = 1;
    for (int t = 0; t < newsize+1; t+=3)
    {
        rai << fin_array_rai[t] << " " << fin_array_rai[t+1] << " " << fin_array_rai[t+2] << std::endl;
        //std::cout<< k << "th rai = " << fin_array_rai[t] << " " << fin_array_rai[t+1] << " " << fin_array_rai[t+2] << std::endl;
        k++;
    }
    //std::cout<< "-----------------------------" << std::endl;
    k = 1;
    for (int t = 0; t < newsize+1; t+=3)
    {
        aai << fin_array_aai[t] << " " << fin_array_aai[t+1] << " " << fin_array_aai[t+2] << std::endl;
        //std::cout<< k << "th aai = " << fin_array_aai[t] << " " << fin_array_aai[t+1] << " " << fin_array_aai[t+2] << std::endl;
        k++;
    }
    //std::cout<< "-----------------------------" << std::endl;
    k = 1;
    for (int t = 0; t < newsize+1; t+=3)
    {
        jackard << fin_array_jackard[t] << " " << fin_array_jackard[t+1] << " " << fin_array_jackard[t+2] << std::endl;
        //std::cout<< k << "th jackard = " << fin_array_jackard[t] << " " << fin_array_jackard[t+1] << " " << fin_array_jackard[t+2] << std::endl;
        k++;
    }
    //std::cout<< "-----------------------------" << std::endl;
    k = 1;
    for (int t = 0; t < newsize+1; t+=3)
    {
        pa << fin_array_pa[t] << " " << fin_array_pa[t+1] << " " << fin_array_pa[t+2] << std::endl;
        //std::cout<< k << "th pa = " << fin_array_pa[t] << " " << fin_array_pa[t+1] << " " << fin_array_pa[t+2] << std::endl;
        k++;
    }*/
    


    
    return 0;

}
