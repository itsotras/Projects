#include <iostream>
#include <sstream>
#include "graphiso.h"

using namespace std;

// ================= Complete - Begin Graph class implementation ===================
Graph::Graph(std::istream& istr)
{
    string aline;
    while(getline(istr,aline))
    {
        istringstream iss(aline);
        string u, v;
        if(iss >> u){
            VERTEX_SET_T neighbors;
            while(iss >> v)
            {
                neighbors.insert(v);
            }
            adj_.insert(std::make_pair(u,neighbors));
        }
    }
}


bool Graph::edgeExists(const VERTEX_T& u, const VERTEX_T& v) const
{
    if((adj_.find(u) != adj_.end()) && (adj_.at(u).find(v) != adj_.at(u).end()))
    {
        return true;
    }
    return false;
}
const VERTEX_SET_T& Graph::neighbors(const VERTEX_T& v) const
{
    if(adj_.find(v) == adj_.end()){
        throw std::invalid_argument("Neighbors: invalid vertex - not in  map");
    }
    return adj_.at(v);
}
VERTEX_LIST_T Graph::vertices() const
{
    VERTEX_LIST_T verts;
    for(const auto& p : adj_)
    {
        verts.push_back(p.first);
    }
    return verts;
}
// ================= Complete - End Graph class implementation ===================

// Prototype and helper functions here
bool isoHelper(const Graph& g1, const Graph& g2, const VERTEX_LIST_T &g1verts, const VERTEX_LIST_T &g2verts, VERTEX_ID_MAP_T& mapping, int i, VERTEX_SET_T& used);
bool checkNeighbors(VERTEX_T g1u, VERTEX_T g2u, VERTEX_SET_T g1u_neighbors, VERTEX_SET_T g2u_neighbors, VERTEX_ID_MAP_T& mapping, int full);

// To be completed
bool isConsistent(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping)
{   
    // Feel free to change or remove this starter code
    VERTEX_LIST_T g1verts = g1.vertices();
    VERTEX_LIST_T g2verts = g2.vertices();

    for(int i = 0; i < (int)g1verts.size(); i++)
    {
        // Check mappings for necessary vertices to see if there is any violation
        // and return false
        VERTEX_T g1u = g1verts[i];
        if(mapping.find(g1u) == nullptr){// the graph hasn't been mapped here yet
            
        }
        else{
            VERTEX_T g2u = mapping[g1u];//get the corresponding g2u

            VERTEX_SET_T g1u_neighbors = g1.neighbors(g1u);//get the neigbors of g1u
            VERTEX_SET_T g2u_neighbors = g2.neighbors(g2u);//get the neigbors of g2u

            if(mapping.size() == 1){

                if(g1u_neighbors.size() == g2u_neighbors.size()){
                    return true;
                }
                
                return false;

            }

            else if(g1u_neighbors.size() == g2u_neighbors.size()){//check if the degree is equal
            
                //check if there exists an edge between u1 and v1, as well as u2 and v2
                if(checkNeighbors(g1u, g2u, g1u_neighbors, g2u_neighbors, mapping, g1.vertices().size())){

                }
                else{
                    return false;
                }
                
            }
            else{//degree is not equal

                return false;

            }

        }
        
        
        
    }
    //only if we get here is the graph consistent
    return true;
}

// Add any helper functions you deem useful

// To be completed
bool graphIso(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping)
{
    VERTEX_LIST_T g1verts = g1.vertices();
    VERTEX_LIST_T g2verts = g2.vertices();
    if(g1verts.size() != g2.vertices().size())
    {
        return false;
    }
    // Add code 
    VERTEX_SET_T used;
    bool status = isoHelper(g1, g2, g1verts, g2verts, mapping, 0, used);
    // Delete this and return the correct value...
    // This is just placeholder to allow compilation
    return status;
}

bool isoHelper(const Graph& g1, const Graph& g2, const VERTEX_LIST_T &g1verts, const VERTEX_LIST_T &g2verts, VERTEX_ID_MAP_T& mapping, int i, VERTEX_SET_T& used){

    if(mapping.size() == g1verts.size()){//if we have the specific number of vertices we need, then we can end

        return true;

    }
    else{
        VERTEX_T v1 = g1verts[i];

        for(int j = 0; j < (int) g2verts.size(); j++){

            VERTEX_T v2 = g2verts[j];//get the vertex

            mapping.insert({v1,v2});
            std::cout<<"Mapping "<<v1<<":"<<v2<<endl;

            if(used.find(v2) != used.end()){//if it already exists in the used set

                continue;

            }
            used.insert(v2);//if it doesn't, include it in the set

            if(isConsistent(g1, g2, mapping)){

                bool status = isoHelper(g1, g2, g1verts, g2verts, mapping, i+1, used);//go to the next vertex in g1

                if(status){

                    return true;

                }

            }
            used.erase(v2);
            
                
        }

        mapping.remove(v1);
        return false;
    }
    



}

bool checkNeighbors(VERTEX_T g1u, VERTEX_T g2u, VERTEX_SET_T g1u_neighbors, VERTEX_SET_T g2u_neighbors, VERTEX_ID_MAP_T& mapping, int full){

    bool checker = false;
    for(VERTEX_SET_T::iterator it = g1u_neighbors.begin(); it!= g1u_neighbors.end(); ++it){//loop through my neighbors
        checker = false;
        for(VERTEX_SET_T::iterator jt = g2u_neighbors.begin(); jt!= g2u_neighbors.end(); ++jt){//loop through my neighbors
            VERTEX_T g1v = *it;
            VERTEX_T g2v = *jt;


                if(mapping.find(g1v) == nullptr){//this neighbor hasn't been mapped yet
                    //don't do anything
                    checker = true;
                }
                else if(mapping[g1v] == g2v){//if they are mapped
                    //numIsoNeighbors++;//add to the number of neighbors who have an edge with us and map to the same place
                    checker = true;
                }
                else if(mapping[g1v] != g2v){//they map to a node that isn't a neighbor of my mapped node
                    //do nothing
                    if(mapping.size() == full){
                        //return false;
                    }
                }

            

        }
        if(!checker){
            return false;
        }

    }
    return true;

}