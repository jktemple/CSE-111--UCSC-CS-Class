/*
 * Copyright (C) 2018-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */
#include <algorithm>
#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include "../include/cracker.h"
#include <cstring>
#include <crypt.h>


void threadableCrypt(int index, const char *a, const char *hash, char *passwd){
    char salt[3] = {hash[0], hash[1], 0};
    for(size_t x = 0; x < strlen(a); x++){
        for(size_t y = 0; y< strlen(a); y++){
            for(size_t z = 0; z <strlen(a); z++){
                if(strcmp(passwd, "!!!!!") != 0){
                    return;
                }
                char guess[5] = {a[index],a[x],a[y],a[z], 0};
                struct crypt_data data;
                data.initialized = 0; 
                char* cmp = crypt_r(guess, salt, &data);
                if(strcmp(hash, cmp) == 0){
                   strcpy(passwd, guess);
                  // std::cout<< "copying and returning, guess = " << guess << std::endl;
                   //std::cout<<"cmp = " << crypt(guess, salt) << " hash = " << hash << std::endl;
                   return;
                }
            }
        }
    }
}

void parrallelCrack(const char *alphabet, const char *hash, char *passwd){
    size_t tests = 0;
    std::vector<std::thread> threads;
    while(tests < strlen(alphabet)){
        for(unsigned int i = 0; i < 25 && tests < strlen(alphabet); i++){
            threads.push_back(std::thread(threadableCrypt, tests, alphabet, hash, passwd));
            tests++;
        }
        for(size_t i = 0; i < threads.size(); i++){
            threads[i].join();
        }
        threads.clear();
    }
}

#define OLAF 0
#define NOGGIN 1
#define THOR 2
#define NOGBAD 3

int main(void) {
    char name[100];
    int hostName = OLAF;
    gethostname(name, 100);
    if(strcmp(name, "olaf") == 0){
        hostName = OLAF;
    } else if(strcmp(name, "thor") == 0){
        hostName = THOR;
    } else if(strcmp(name, "noggin") == 0){
        hostName = NOGGIN;
    } else if(strcmp(name, "nogbad") == 0){
        hostName = NOGBAD;
    } 

    int teachingSock = socket(AF_INET, SOCK_DGRAM, 0);
        if(teachingSock < 0) exit(-1);
    struct sockaddr_in fromOthers;
    bzero((char *) &fromOthers, sizeof(fromOthers));
    fromOthers.sin_family = AF_INET;
    fromOthers.sin_port = htons(get_unicast_port());
    if (bind(teachingSock, (struct sockaddr *) &fromOthers, sizeof(fromOthers)) < 0) {
        std::cout<<"failed to bind for other teaching servers" << std::endl;
        exit(-1);
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) exit(-1);
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(get_multicast_port());
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        std::cout<<"failed to bind" << std::endl;
        exit(-1);
    }
    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = get_multicast_address();
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
    (void *) &multicastRequest, sizeof(multicastRequest)) < 0){
        std::cout<<"failed to setsockopt()\n";
        exit(-1);
    }
    struct sockaddr_in remote_addr;
    socklen_t len = sizeof(remote_addr);
    Message messageFromGrader;
    bzero(&messageFromGrader,sizeof(messageFromGrader));
    int n = recvfrom(sockfd, &messageFromGrader, sizeof(messageFromGrader), 0, (struct sockaddr *)&remote_addr, &len);
    messageFromGrader.num_passwds = ntohl(messageFromGrader.num_passwds);
    if(n<0) exit(-1);
    char passArray[MAX_HASHES][HASH_LENGTH+1];
    for(unsigned int i = (hostName*messageFromGrader.num_passwds)/4; i < ((hostName+1)*messageFromGrader.num_passwds)/4; i++){
        //std::cout<<"starting parrellel crack #" << i<< "\n";
        strcpy(passArray[i], "!!!!!");
        parrallelCrack(messageFromGrader.alphabet, messageFromGrader.passwds[i], passArray[i]);
    }
    close(sockfd);
    if(hostName != OLAF){
        //create a new message struct all the same details except cracked passwords instead of hashes
        Message toOlaf;
        bzero(&toOlaf,sizeof(toOlaf));
        if(hostName == NOGGIN){
            strcpy(toOlaf.cruzid, "noggin");
            //toOlaf.cruzid = "Noggin";
        } else if(hostName == THOR){
            strcpy(toOlaf.cruzid, "thor");
            //toOlaf.cruzid = "Thor";
        } else {
            strcpy(toOlaf.cruzid, "nogbad");
            //toOlaf.cruzid == "Nogbad";
        }
        toOlaf.num_passwds = htonl(messageFromGrader.num_passwds/4);
        for(unsigned int i = 0; i < messageFromGrader.num_passwds; i++){
            strcpy(toOlaf.passwds[i], passArray[i]);
            //std::cout<<"password #" << i <<": " << toOlaf.passwds[i] << std::endl;
        }
        //send Message via UDP To olaf
        int olafSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if(olafSocket<0){
            std::cout<<"Failed to created olafSocket\n";
            exit(-1);
        }
        struct sockaddr_in olaf_addr;
        struct hostent *olafServer;
        bzero((char *) &olaf_addr, sizeof(olaf_addr));
        olaf_addr.sin_family = AF_INET;
        olaf_addr.sin_port = htons(get_unicast_port());
        olafServer = gethostbyname("olaf");
        memcpy(&olaf_addr.sin_addr.s_addr, olafServer->h_addr, olafServer->h_length);
        sendto(olafSocket, &toOlaf, sizeof(toOlaf), 
            0, (struct sockaddr *) &olaf_addr,sizeof(olaf_addr));
        //std::cout<<"Sent to Olaf count = " << count << "\n";
        close(olafSocket);
    } else if(hostName == OLAF){
        struct sockaddr_in teaching_addr;
        socklen_t teachLen = sizeof(teaching_addr);
        Message teachingMessages[3];
        bzero(&teachingMessages[0],sizeof(teachingMessages[0]));
        bzero(&teachingMessages[1],sizeof(teachingMessages[1]));
        bzero(&teachingMessages[2],sizeof(teachingMessages[2]));
        //std::cout<<"about to recvfrom teaching server\n";
        for(int i = 0; i < 3; i++){
            int n = recvfrom(teachingSock, &teachingMessages[i], sizeof(teachingMessages[i]), 0, (struct sockaddr *)&teaching_addr, &teachLen);
            if(n < 0){
                std::cout << "recvfrom teaching server failed\n";
                exit(-1);
            }
             //std::cout<<"Message recived from name = " << teachingMessages[i].cruzid << "\n";
            int loc;
            if(strcmp(teachingMessages[i].cruzid, "thor") == 0){
                loc = THOR;
            } else if(strcmp(teachingMessages[i].cruzid, "noggin") == 0){
                loc = NOGGIN;
            } else if(strcmp(teachingMessages[i].cruzid, "nogbad") == 0){
                loc = NOGBAD;
            } 
            //std::cout<<"Loc = " << loc << std::endl;
            for(unsigned int x = (loc*messageFromGrader.num_passwds)/4; x < ((loc+1)*messageFromGrader.num_passwds)/4; x++){
                //std::cout<<"teachingMessages[" << i << "].passwds[" << x <<"] = " << teachingMessages[i].passwds[x] <<"\n";
                strcpy(passArray[x], teachingMessages[i].passwds[x]);
            }
        }
        //std::cout<<"Message recived from name = " << teachingMessage1.cruzid << "\n";
        int returnSock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in returnServerAddress;
        struct hostent *returnServer;
        returnServer = gethostbyname(messageFromGrader.hostname);
        //std::cout<<"messageFromGrader.hostname = " << messageFromGrader.hostname << std::endl;
        //std::bzero((char *) &serverAddress, sizeof(serverAddress));
        memset(&returnServerAddress, 0, sizeof(returnServerAddress));
        returnServerAddress.sin_family = AF_INET;
        //std::bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
        memcpy(&returnServerAddress.sin_addr.s_addr, returnServer->h_addr, returnServer->h_length);
        returnServerAddress.sin_port = messageFromGrader.port;
        //std::cout<<"messageFromGrader.port = " << messageFromGrader.port << std::endl;
        //std::cout<<"Connecting to grader\n";
        int test = connect(returnSock,(struct sockaddr *) &returnServerAddress,sizeof(returnServerAddress));
        if(test < 0){
            std::cout << "failed to connect to grader\n";
            exit(-1);
        }
        for(unsigned int i = 0; i < messageFromGrader.num_passwds; i++){
            strcpy( messageFromGrader.passwds[i], passArray[i]);
        }
        send(returnSock, &messageFromGrader, sizeof(messageFromGrader),0);
        close(returnSock);
    }
    close(teachingSock);
}
