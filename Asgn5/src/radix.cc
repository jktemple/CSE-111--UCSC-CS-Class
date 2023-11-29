/*
 * Copyright (C) 2018-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include "../include/radix.h"
#include <algorithm>
#include <thread>
#include <iostream>
#include <string>
#include <cstring>
#include <bitset>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>


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

void server_msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, unsigned int cores) { 
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

RadixServer::RadixServer(const int port, const unsigned int cores) {
    // your server implementation goes here :)
    //Create and empty List of Lists
    //CLient server socket creation and connection adpated from
    //https://www.linuxhowtos.org/C_C++/socket.htm
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);
     bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    
     std::vector<std::reference_wrapper<std::vector<unsigned int>>> lists;
     unsigned int buffer;
     do{
        std::vector<unsigned int> vec;
        while(recv(newsockfd, &buffer, sizeof(buffer),0) > 0){
            //std::cout<<"top of while\n";
            unsigned int num = ntohs(buffer);
            if(num == 0){
                break;
            } else{
                //std::cout<<"number = " << num <<"\n";
                vec.push_back(num);
            }
        }
        std::vector<unsigned int> t;
        lists.push_back(std::ref(t));
        lists[0].get().insert(lists[0].get().begin(),vec.begin(), vec.end());
        vec.clear();
        server_msd(lists, cores);
        //std::cout<<"Sending back\n";
        for(auto list : lists){
            //std::cout<<"top of outer for loop\n";
            for(auto element : list.get()){
                //std::cout<<"top of inner for loop\n";
                //std::cout<< element << "\n";
                unsigned int temp = htonl(element);
                //std::cout << "after conversion\n";
                send(newsockfd, &temp, sizeof(temp),0);
            }
            unsigned int temp = 0;
            send(newsockfd, &temp, sizeof(temp),0);
        }
     } while(recv(newsockfd, &buffer, sizeof(buffer), MSG_PEEK) > 0);
    close(sockfd);
    close(newsockfd);
}

void RadixClient::msd(
    const char *hostname,
    const int port,
    std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
    // your client implementation goes here :)
    //open a socket to the server
    //socket connection code adapted from 
    //https://www.linuxhowtos.org/C_C++/socket.htm
    int sockfd;
    struct sockaddr_in serverAddress;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
    server = gethostbyname(hostname);
    //std::bzero((char *) &serverAddress, sizeof(serverAddress));
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    //std::bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    memcpy(&serverAddress.sin_addr.s_addr, server->h_addr, server->h_length);
    serverAddress.sin_port = htons(port);
    connect(sockfd,(struct sockaddr *) &serverAddress,sizeof(serverAddress));
    //Now server is connected
    //send each List to the server
  
    for(auto &list : lists){
        std::string tempString;
        for(auto element : list.get()){
            //std::cout << "sending element = " << element << std::endl;
            unsigned int temp = htonl(element);
            send(sockfd, &temp, sizeof(temp),0);
        }
        unsigned int temp = 0;
        send(sockfd, &temp, sizeof(temp),0);
    }
    //Server sorts lists
    //Server sends lists in the same way that the client did
    for(auto &list : lists){
        //Create empty vector
         std::vector<unsigned int> vec;
         for(;;){
            //read 32 bytes from the server
            unsigned int buffer = 0;
            //std::cout << "recv\n";
            recv(sockfd, &buffer, sizeof(unsigned int),0);
           // std::cout << "after recv\n";
            //covert to an unsigned int
            unsigned int number = ntohl(buffer);
            //std::cout << "buffer = " << buffer << "\n";
           // number = std::stoull(buffer, nullptr, 2);
            //if the int is 0 break
            if(number == 0){
                //std::cout << "breaking from for(;;) loop\n";
                break;
            } else { //else push the unsigned int onto vec
                //myfile << "number = " << number << "\n";
                vec.push_back(number);
            }
            //memset(buffer, 0, 33);
        }
        //replace the contents of lits with the contents of vec
        list.get().clear();
        list.get().insert(list.get().begin(),vec.begin(), vec.end());
    }
    close(sockfd);
    //done
    //close socket
}
