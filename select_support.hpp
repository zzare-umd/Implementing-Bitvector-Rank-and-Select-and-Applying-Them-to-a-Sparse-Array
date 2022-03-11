#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <bit>
#include <ctime>
#include <fstream>
#include <string>
#include <chrono>
#include "compact_vector.hpp"

using namespace std;
using namespace std::chrono;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************************Select_support_class*****************************************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class select_support
{

    public:
    
        select_support(compact::vector<unsigned int>* d, uint64_t size2);
        uint64_t rank1( uint64_t i);
        uint64_t select1( uint64_t i);
        uint64_t overhead();
        void save(string& fname);
        void load(string& fname);
    
    private:
        //uint64_t binarysearching( uint64_t i);
        uint64_t size;
        vector<uint16_t> Rb;
        vector<uint64_t> Rs;
        uint64_t block_size;
        uint16_t numberofbits_blocks;
        uint64_t numberofbits_superblocks;
        uint64_t superblock_size;
        compact::vector<unsigned int>* data;
    
};

select_support::select_support(compact::vector<unsigned int>* d, uint64_t size2)
{
    data = d;
    size = size2;
    block_size = (*d).size(); //size of the Rb vector
    //cout<< "\n new block size\n" << block_size<< "\n end of new\n";
    numberofbits_blocks = ceil(log2(size));
    numberofbits_superblocks = pow(ceil(log2(size)),2);
    //block_size = (size+numberofbits_blocks-1)/numberofbits_blocks;
    superblock_size = (size + numberofbits_superblocks -1)/numberofbits_superblocks; //size of the Rs vector
    Rb.resize(block_size);
    Rs.resize(superblock_size);
    uint64_t counting_value = 0;

    for (size_t i = 1; i < block_size; i++)
    {
        counting_value = counting_value + __builtin_popcount((*d)[i-1]);
        if (i % numberofbits_blocks == 0)
        {
            Rs[i / numberofbits_blocks] = Rs[(i / numberofbits_blocks)-1] + counting_value; 
            counting_value = 0;
        }
        Rb[i] = counting_value;
    }

}

uint64_t select_support::rank1(uint64_t i)
{

    uint64_t number_of_ones = Rs[i / numberofbits_superblocks]+ Rb[i / numberofbits_blocks] ;
    number_of_ones = number_of_ones + __builtin_popcount((*data)[(i / numberofbits_blocks)]>>(numberofbits_blocks-1-(i%numberofbits_blocks)));
 
    return number_of_ones;
}

uint64_t select_support::select1(uint64_t i)
{
    uint64_t index_value=0;
    uint64_t first_index = 0;
    uint64_t last_index = size;
    uint64_t middle_index;
    uint64_t comparing_value;
    
    auto start = steady_clock::now();

    for (size_t j = 0; j <= ceil(log2(size)); j++)
    {
        middle_index = (first_index + last_index)/2;
        comparing_value = select_support::rank1(middle_index);
        
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

    auto stop = steady_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start).count();
    cout<<"\n****"<< ((double)duration); //duration.count();
    
    return index_value;
    
}


uint64_t select_support::overhead(){

    return ((Rb.size() * 16) + (Rs.size() * 64));
}

void select_support::save(string& fname){
    ofstream outfile(fname);
    
    //wrting the size of the input, Rs, and Rb into file
    outfile << Rs.size() <<endl;
    outfile << Rb.size() <<endl;

    //writing Rs into file
    for (size_t i = 0; i < Rs.size(); i++)
    {
        outfile << Rs[i] <<endl;
    }

    //writing Rb into file
    for (size_t i = 0; i < Rb.size(); i++)
    {
        outfile << Rb[i] <<endl;
    }

    //writing data into file
    for (size_t i = 0; i < (*data).size(); i++)
    {
        outfile << (*data)[i] <<endl;
    }
    
    outfile.close();
}


void select_support::load(string& fname){
    ifstream infile(fname);
    uint64_t Rs_load_size, Rb_load_size, data_load_size, data_aux; 
    vector<uint64_t> vec_aux;
    while (infile >> data_aux)
    {
        vec_aux.push_back(data_aux);
    }

    Rs_load_size = vec_aux[0] ;
    
    Rb_load_size = vec_aux[1]  ;

    data_load_size = vec_aux.size() - Rs_load_size - Rb_load_size - 2;

    
    for (size_t i = 0; i < Rs_load_size; i++)
    {
        Rs[i] = vec_aux[2 + i];
                
    }
    
    
    for (size_t i = 0; i < Rb_load_size; i++)
    {
        Rb[i] = vec_aux[2 + Rs_load_size + i];

    }

    
    for (size_t i = 0; i < data_load_size; i++)
    {
        (*data)[i] = vec_aux[2 + Rs_load_size + Rb_load_size +i];
    }
    infile.close();


}