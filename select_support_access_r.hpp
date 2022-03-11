#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <bit>
#include <ctime>
#include <fstream>
#include <string>
#include <chrono>
#include "rank_support.hpp"
#include "compact_vector.hpp"

using namespace std;
using namespace std::chrono;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************************Select_support_class*****************************************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class select_support
{

    public:
    
        select_support(rank_support &d,uint64_t size2);
        uint64_t select1( uint64_t i);
        uint64_t overhead();
        void save(string& fname);
        void load(string& fname);
        rank_support* r = nullptr;
        uint64_t size;
 
    
};

select_support::select_support(rank_support &d, uint64_t size2)
{
    size = size2;
    (*r) = d;
}



uint64_t select_support::select1(uint64_t i)
{
    uint64_t index_value=0;
    uint64_t first_index = 0;
    uint64_t last_index = size;
    uint64_t middle_index;
    uint64_t comparing_value;
    
    

    for (size_t j = 0; j <= ceil(log2(size)); j++)
    {
        middle_index = (first_index + last_index)/2;
        comparing_value = (*r).rank1(middle_index);
        
        if (comparing_value == i)
        {
            index_value = middle_index;
            break;
        }
        else if (comparing_value < i)
        {
            first_index = middle_index + 1;
        }
        else
        {
            last_index = middle_index;
        }
        
        
    }

    
    
    return index_value;
    
}


uint64_t select_support::overhead(){

    return (*r).overhead();
}

void select_support::save(string& fname){
   
   (*r).save(fname);
}


void select_support::load(string& fname){
    
    (*r).load(fname);


}