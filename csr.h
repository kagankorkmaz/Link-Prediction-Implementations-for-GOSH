#ifndef _CSR_H
#define _CSR_H
//#define _VERBOSE
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
// class definition of CSR                                                                                                                                                  // This class encapsulates all the data members required for a single weighted or unweighted graph stored in a Compressed Sparse Row matrix format
// It is a templated class where the data type of the vertices IDs is dependant on initialization (for storage optimization purposes)
typedef long long ll;
template <class T>
class CSR
{
private:
    //static constexpr bool weighted = false;
    //static constexpr bool directed = false;
    int read_bcsr(string fname);
    //int mtx2csr(char *fname);
    int mtx2csr(string fname, bool directed, bool weighted);
    int bmtx2csr(string fname, bool directed, bool weighted);
    size_t sizeOfUnit;

    void skip_mtx_comments(ifstream &file);


    //T get_random_matching(T v, T size, T * match);
    //
    //rai
    float *get_ra_indexes();
    //matchings
    T get_ra_matching(T v, T size, float *ra_indexes, T *match);

    //aai
    float *get_aa_indexes();
    //matchings
    T get_aa_matching(T v, T size, float *aa_indexes, T *match);

    //bool is_merged_neighbor_copied(T v, unsigned int copied_edge_amount, T * n_V, T * n_E, T old_edge_map);
    unsigned long long copy_coarsed_vertex_edges(T v, T i, unsigned long long copied_edge_amount, T local_new_e_count, T *n_E, T *map, T *marker);

public:
    T *V; // the array of vertices
    T *E; // the array of edges
    T *W;
    T *map;
    ll num_vertices = 0;
    ll num_edges = 0;
    CSR(ll nv, ll ne, T *V, T *E, T *W = NULL, T *map = NULL);
    CSR(CSR &);
    CSR(string fname);
    CSR(string fname, bool directed, bool weighted, bool binary = false);
    ~CSR();
    bool is_weighted() const;
    T get_correct_edge_index(T v, T *edge_place);
    int write_bcsr(string fname);
    CSR<T> *coarsen_with_multi_edge_collapse(const char *heuristic, unsigned long long level);
        //jaccard
    float *get_jaccard_edges();
    //float get_jaccard(T v, T u, unsigned int v_edge_amount);
    //

    //matchings
    T get_jaccard_matching(T v, T size, float *jaccard_edges, T *match);
};
#include "csr.cpp"
#endif