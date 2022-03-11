#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <bit>
#include <ctime>
#include <fstream>
#include <string>
#include <chrono>
#include "compact_vector.hpp"
#include "bitvector.hpp"
//#include "rank_support.hpp"
#include "select_support.hpp"
//#include "sparse_array.hpp"
#include "sparse_array_faster.hpp"

using namespace std;
using namespace std::chrono;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//************************Function to get sorted random indices to set 1 in the sparse array***********// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<uint64_t> get_vector_index_ones(uint64_t size, uint64_t sparsity)
{
    uint64_t number_of_ones_vec = size - ceil(size*sparsity/100);
    vector<uint64_t> ones_index_vec(number_of_ones_vec);
    srand(time(0));
    for (uint64_t i = 0; i < number_of_ones_vec; i++)
    {
        ones_index_vec[i] = rand()%size;
        for (size_t j = 0; j < i; j++)
        {
            if (i>0 && (ones_index_vec[i]==ones_index_vec[j]))
            {
                i--;
                break;
            }
           
        }
       
    }

    sort(ones_index_vec.begin(),ones_index_vec.end());
    return ones_index_vec;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************Main Function****************************************//
//////////////////////////////////////////////////////////////////////////////////////////////////

int main(){

    vector<uint64_t> size={100,1000,10000,100000};
    vector<uint64_t> sparsity={1,5,10,15};
    
    //storing the results in files
    ofstream outfile1("overhead_rank.txt");   
    ofstream outfile2("overhead_select.txt");
    ofstream outfile3("time_required_rank.txt");   
    ofstream outfile4("time_required_select.txt");
    ofstream outfile5("time_required_sparse_append.txt");   
    ofstream outfile6("time_required_sparse_get_at_rank.txt");
    ofstream outfile7("time_required_sparse_get_at_index.txt");   
    ofstream outfile8("time_required_sparse_num_elem_at.txt");
    //**************************************************************************************************

    //overhead for rank and select operations
    for (auto i : size)
    {
        
        bitvector A(i, sparsity[0]);
        rank_support B(A.b, i);
        outfile1 << B.overhead()<<endl;

        select_support C(A.b, i);
        outfile2<<C.overhead()<<endl;
  
    }

    outfile1.close();
    outfile2.close();
    //*************************************************************************************************

    //computing the time required for some fixed number of rank and select operations
    uint64_t n = 0;
    for (auto i : size)
    {
        bitvector A(i, sparsity[0]);
        rank_support B(A.b, i);

        //start time for rank operation
        auto start = high_resolution_clock::now();

        for (size_t i = 0; i < 1000; i++)
        {
            B.rank1(99*(10^n));            
        }
        auto stop = high_resolution_clock::now();
        duration<double> elapsedTime = (stop - start);
        
        outfile3 << elapsedTime.count() * 1e6<<endl;
        //end time for rank operation

        //select function
        select_support C(A.b, i);

        //start time of select operation
        start = high_resolution_clock::now();

        for (size_t i = 0; i < 1000; i++)
        {
            C.select1(1);            
        }
        stop = high_resolution_clock::now();
        elapsedTime = (stop - start);
        
        outfile4 << elapsedTime.count() * 1e6<<endl;
        //end time of select operation
        cout<<"\n i is :"<<i;
        n+=1;
    }

    outfile3.close();
    outfile4.close();
    //**************************************************************************************************

    //time required with different size and sparsity for different functions of sparse array
    vector<uint64_t> vec_index;
    string mystring;
    for (auto i : size)
    {  

        for (auto j : sparsity)
        {
            vec_index = get_vector_index_ones(i,j);
            sparse_array D;
            D.create(i);
            //start time for append function
            auto start1 = high_resolution_clock::now();

            for (size_t k = 0; k < vec_index.size(); k++)
            {
                D.append("A",vec_index[k]);
            }

            auto stop1 = high_resolution_clock::now();
            duration<double> elapsedTime1 = (stop1 - start1);
        
            outfile5 << elapsedTime1.count() * 1e6<<endl;
            //end time for append funtion
            
            //start time for get_at_rank function
            start1 = high_resolution_clock::now();

            for (size_t k = 0; k < 10; k++)
            {
                D.get_at_rank(1, mystring);
            }

            stop1 = high_resolution_clock::now();
            elapsedTime1 = (stop1 - start1);
        
            outfile6 << elapsedTime1.count() * 1e6<<endl;
            //end time for get_at_rank function


            //start time for get_at_index function
            start1 = high_resolution_clock::now();

            for (size_t k = 0; k < 100; k++)
            {
                D.get_at_index(vec_index[0], mystring);
            }

            stop1 = high_resolution_clock::now();
            elapsedTime1 = (stop1 - start1);
        
            outfile7 << elapsedTime1.count() * 1e6<<endl;
            //end time for get_at_index function

            //start time for num_elem_at function
            start1 = high_resolution_clock::now();

            for (size_t k = 0; k < 100; k++)
            {
                D.num_elem_at(vec_index[0]);
            }

            stop1 = high_resolution_clock::now();
            elapsedTime1 = (stop1 - start1);
        
            outfile8 << elapsedTime1.count() * 1e6<<endl;
            //end time for num_elem_at function


        }
        
    }

    outfile5.close();
    outfile6.close();
    outfile7.close();
    outfile8.close();

  




    return 0;
};