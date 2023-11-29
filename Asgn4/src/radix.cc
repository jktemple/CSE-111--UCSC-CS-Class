/*
 * Copyright (C) 2018-2022 David C. Harrison. All rights reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include "../include/radix.h"
#include <algorithm>
#include <thread>
#include <iostream>

#include <string>
#define bucket_type std::vector<std::vector<std::vector<unsigned int>>>

bucket_type sortIntoBuckets(std::vector<unsigned int> &vec){
    bucket_type returnVec(10, std::vector<std::vector<unsigned int>>(10));
    for(auto x : vec){
        unsigned int first = x;
        unsigned int second = 0;
        while(first >= 10){
            second = first%10;
            first = first/10;
        }
        //std::cout << "pushing_back x = " << x << std::endl;
        returnVec[first][second].push_back(x);
    }
    return returnVec;
}

//using costom comparison with sort() leraned from the c++ documentation
 struct {
        bool operator()(unsigned int a, unsigned int b) const {  
            std::string tmp1 = std::to_string(a);
            std::string tmp2 = std::to_string(b);
            return std::lexicographical_compare(tmp1.begin(),tmp1.end(),tmp2.begin(),tmp2.end()); }
} lex_compare;

void sortVec(std::vector<unsigned int> &vec){
    std::sort(vec.begin(), vec.end(), lex_compare);
    
}

std::vector<unsigned int> bucketsToList(bucket_type buckets){
    std::vector<unsigned int> returnVec;
    for(int row = 0; row < 10; row++){
        for(int col = 0; col < 10; col++){
            for(size_t i = 0; i < buckets[row][col].size(); i++){
                //std::cout<< "pushing " << buckets[row][col][i-1] << " onto return vec \n";
                returnVec.push_back(buckets[row][col][i]);
            }
        }
    }
    return returnVec;
}

void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, unsigned int cores) { 
   for(auto &list : lists){
       std::vector<std::thread> threads; 
       //sort list into 100 buckets
       bucket_type buckets = sortIntoBuckets(list.get());
       //printBuckets(buckets);
       //divide those 100 buckets between cores # of threads
       int sorts = 0;
       while(sorts < 100){
           for(unsigned int i = 0; i < cores && sorts < 100; i++){
               //std::cout << "sorts = " << sorts << " creating new thread" << std::endl;
               threads.push_back(std::thread(sortVec, std::ref(buckets[sorts/10][sorts%10])));
               sorts++;
           }
           //std::cout << " going to join threads" << std::endl;
           for(unsigned int i = 0; i < threads.size(); i++){
               //std::cout << "trying to join thread #" << i << std::endl;
               threads[i].join();
           }
           threads.clear();
       }
       //checkSorted(buckets);
       //std::cout << "printing buckets post sort" << std::endl;
       //printBuckets(buckets);
       //assemble a sorted vector from all the sorted buckets
       std::vector<unsigned int> vec = bucketsToList(buckets);
        list.get().clear();
        list.get().insert(list.get().begin(),vec.begin(), vec.end());
    }
}
