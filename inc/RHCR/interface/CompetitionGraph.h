#pragma once
#include "RHCR/main/BasicGraph.h"
#include "Grid.h"

namespace RHCR {

class CompetitionGraph: public BasicGraph {
// for simplicity, just make everything public. but attributes or functions start with _ are supposed to use privately in general cases.
public:
    CompetitionGraph(const Grid & grid);
    
    // a dummy function.
    bool load_map(string fname);
    // preprocessing the map, e.g., computing heuristics for later planning.
    void preprocessing(bool consider_rotation=true);
    // NOTE: in this competition, we only need to deal with grid-like graphs, so we just use BFS to compute shortest paths to all other locations. 
    vector<double> compute_heuristics(int root_location);
};
}