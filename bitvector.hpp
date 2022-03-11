#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <bit>
#include <ctime>
#include <fstream>
#include <string>
//#include "rank_support.hpp"
#include "compact_vector.hpp"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************************Bit-Vector-Class*****************************************
//////////////////////////////////////////////////////////////////////////////////////////////////////


class bitvector
{

public:
    //vector<uint64_t> b1;
    compact::vector<unsigned int>* b = nullptr;//(16,8);
    uint64_t size;
    uint64_t sparsity;
    bitvector(uint64_t size,uint64_t sparsity);
    ~bitvector();
        
};

bitvector::~bitvector() {
    delete b;
}

bitvector::bitvector(uint64_t size,uint64_t sparsity)
{

    uint64_t numberofbits_block = ceil(log2(size)); //number of bits in each block of Rb(maximum 64bits)
    uint64_t block_size =  (size+numberofbits_block-1)/numberofbits_block; // size of the Rb vector
    b = new compact::vector<unsigned int>(numberofbits_block,block_size);

    for (size_t i = 0; i < block_size; i++)
    {
        (*b)[i] = pow(2,numberofbits_block)-1;
    }

    uint64_t number_of_zeroes = ceil(size*sparsity/100);
    vector<int64_t> zeroes_index_vec(number_of_zeroes);
    srand(time(0));
    for (uint64_t i = 0; i < number_of_zeroes; i++)
    {
        zeroes_index_vec[i] = rand()%size;
        for (size_t j = 0; j < i; j++)
        {
            if (i>0 && (zeroes_index_vec[i]==zeroes_index_vec[j]))
            {
                i--;
                break;
            }
           
        }
       
    }

    sort(zeroes_index_vec.begin(),zeroes_index_vec.end());


    compact::vector<unsigned int> m_zero(numberofbits_block);
    m_zero[0] = 0;
    for (size_t l = 0; l < number_of_zeroes; l++)
    {
        m_zero[0] = 1 << (numberofbits_block - 1 -(zeroes_index_vec[l] % numberofbits_block));
        (*b)[zeroes_index_vec[l]/numberofbits_block] = (*b)[zeroes_index_vec[l]/numberofbits_block] & (~m_zero[0]);
        m_zero[0] = 0;
    }

    for (int i=0; i<block_size; i++)
    {
        cout<<(*b)[i]<<"\t";
    }
    
    
}
