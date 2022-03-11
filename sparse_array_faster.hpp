#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <bit>
#include <ctime>
#include <fstream>
#include <string>
#include "rank_support.hpp"
#include "compact_vector.hpp"

using namespace std;


//////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************************Sparse Array Function****************************************
//////////////////////////////////////////////////////////////////////////////////////////////////////

class sparse_array
{

public:
    sparse_array();
    void create(uint64_t size);
    void append(string elem, uint64_t pos);
    bool get_at_rank(uint64_t r, std::string& elem);
    bool get_at_index(uint64_t r, std::string& elem);
    uint64_t num_elem_at(uint64_t r);
    uint64_t size();
    uint64_t num_elem();
    void save(string& fname);
    void load(string& fname);

private:
    uint64_t sizeofarray;
    compact::vector<unsigned int>* sparse_bitvector = nullptr;
    vector<string> string_set;
    uint64_t previou_pos;
    uint64_t position_zero;
    uint64_t numberofbits_block;
    uint64_t block_size;
    rank_support* number_of_elements = nullptr;

};

sparse_array::sparse_array()
{
    previou_pos = 0;
    position_zero = 1;
}

void sparse_array::create(uint64_t size1){
    previou_pos = 0;
    position_zero = 1;
    sizeofarray = size1;
    numberofbits_block = ceil(log2(sizeofarray)); //number of bits in each block of Rb(maximum 64bits)
    block_size =  (sizeofarray+numberofbits_block-1)/numberofbits_block; // size of the Rb vector
    sparse_bitvector = new compact::vector<unsigned int>(numberofbits_block,block_size);
    for (size_t i = 0; i < block_size; i++)
    {
        (*sparse_bitvector)[i] = 0;
    }

    cout<<"\n";
    for (size_t i = 0; i < (*sparse_bitvector).size(); i++)
    {
        cout<<(*sparse_bitvector)[i]<<"\t";
    }
    
}

void sparse_array::append(string elem, uint64_t pos)
{
    compact::vector<unsigned int> m_zero(numberofbits_block);
    m_zero[0] = 0;

    if (pos < sizeofarray)
    {
        if (position_zero > 1 && previou_pos < pos)
        {
            m_zero[0] = 1 << (numberofbits_block - 1 -(pos % numberofbits_block));
            (*sparse_bitvector)[pos/numberofbits_block] = (*sparse_bitvector)[pos/numberofbits_block] | (m_zero[0]);

            string_set.push_back(elem);
            previou_pos = pos;
                
        }
        else if(position_zero > 1 && previou_pos > pos)
        {
            cout << "\n****The position is not correct!!***\n";
        }
        else
        {
            position_zero = position_zero + 1;
            m_zero[0] = 1 << (numberofbits_block - 1 -(pos % numberofbits_block));
            (*sparse_bitvector)[pos/numberofbits_block] = (*sparse_bitvector)[pos/numberofbits_block] | (m_zero[0]);

            string_set.push_back(elem);
            previou_pos = pos;

        }
        
                   
    }
    else
    {
        cout<< "\n***The posision is not in the range!!**\n";
    }

    number_of_elements = new rank_support(sparse_bitvector, sizeofarray);
      
}


bool sparse_array::get_at_rank(uint64_t r, std::string& elem)
{
    if (r >= 0 && r<=sizeofarray)
    {
        if (r > 0 && r <= sparse_array::num_elem())
        {
            elem = string_set[r - 1];
            return 1;
        }
        else if (r == 0)
        {
            elem = "\nThere is no item with r=0\n";
            return 1;
        }
        else
        {
            elem = "\nThere is less item in the list\n";
            return 0;
        }
        
    }
    else
    {
       elem = "\nThere is less item in the list\n";
       return 0;
    }
    
    
}


bool sparse_array::get_at_index(uint64_t r, std::string& elem)
{
    
    if (r >= 0 && r < sizeofarray)
    {
        uint64_t if_get_value_index = ((*sparse_bitvector)[r/numberofbits_block] >> (numberofbits_block - 1 - (r%numberofbits_block))) & 1;
        if (if_get_value_index)
        {
            //rank_support number_of_elements1(sparse_bitvector, sizeofarray);
            elem = string_set[(*number_of_elements).rank1(r) - 1];
            return 1;
        }
        else
        {
            elem = "\nis_empty\n";
            return 0;
        }
                  
    }
    else
    {
        cout << "\nThe index value is out of range\n";
        return 0;
    }

    
}


uint64_t sparse_array::num_elem_at(uint64_t r)
{
    //rank_support number_of_elements2(sparse_bitvector, sizeofarray);

    if (r < sizeofarray)
    {
        return (*number_of_elements).rank1(r);
    }
    else
    {
        cout << "\n***The value is not in range***\n";
        return (*number_of_elements).rank1(sizeofarray - 1);
    }
    
    
}


uint64_t sparse_array::size()
{
    return sizeofarray;
}


uint64_t sparse_array::num_elem()
{
    //rank_support number_of_elements3(sparse_bitvector, sizeofarray);
    return (*number_of_elements).rank1(sizeofarray - 1);

}

void sparse_array::save(string& fname)
{
    ofstream outfile(fname);
    
    //converting the integer bit vector and its size to string
    string itos_value = to_string((*sparse_bitvector).size());
    outfile << itos_value <<endl;

    for (size_t i = 0; i < (*sparse_bitvector).size(); i++)
    {
        itos_value = to_string((*sparse_bitvector)[i]);
        outfile << itos_value <<endl;
    }

    for (size_t i = 0; i < string_set.size(); i++)
    {
        outfile << string_set[i]<<endl;
    }
    
    outfile.close();

}


void sparse_array::load(string& fname)
{
    ifstream infile(fname);
    uint64_t data_load_size, string_load_size;
    string data_aux; 
    vector<string> vec_aux;

    while (infile >> data_aux)
    {
        vec_aux.push_back(data_aux);
    }

    data_load_size = stoi(vec_aux[0]);
    string_load_size = vec_aux.size() - data_load_size - 1;

    for (size_t i = 0; i < data_load_size; i++)
    {
        (*sparse_bitvector)[i] = stoi(vec_aux[i + 1]);
    }

    for (size_t i = 0; i < string_load_size; i++)
    {
        string_set[i] = vec_aux[i + data_load_size + 1];
    }
 

    infile.close();
}

