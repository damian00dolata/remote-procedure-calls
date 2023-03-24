/*
    To compile the server:
    g++ *.cpp -o <server name>
    g++ -Wall -std=c++17 -pthread *.cpp -o <server name>

    To launch server:
    ./<server name>
*/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <pthread.h>
#include <syscall.h>
#include "procedures.hpp"

#define N 2048

struct cln {
    int cfd;
    struct sockaddr_in caddr;
};

void* cthread(void* arg) {
    struct cln* c = (struct cln*) arg;
    char buf[N];
    bool isListSent = false;
    memset(buf, 0, sizeof(buf));
    printf("[SERVER] New connection: %s (Client %ld)\n",
            inet_ntoa((struct in_addr)c->caddr.sin_addr), syscall(SYS_gettid));

    // Server - client loop, until client disconnects
    while(1) {
        // Checking, if procedure list has been sent - otherwise, it skips the condition
        if(!isListSent) {
            std::string temp = exec("./proc/showProcedures.sh");
            const int length = temp.length();
            char* arr = new char[length +1];
            strcpy(arr, temp.c_str());
            for(int i = 0; i < length; i++) {
                buf[i] = arr[i];
            }
            write(c->cfd, buf, sizeof(buf));
            isListSent = true;
        }
        memset(buf, 0, sizeof(buf)); // Clearing buffer
        read(c->cfd, buf, sizeof(buf));

        // Checking if clients wants to disconnect - otherwise, a procedure is called
        if(strncmp(buf, "disconnect", sizeof(buf)) == 0) {
            printf("[SERVER] Client %ld has disconnected\n", syscall(SYS_gettid));
            isListSent = false;
            break;
        } else {
            // Concatenating char arrays, that will be sent to the executive function (exec() - executing given command)
            char sBegin[] = "./proc/";
            char sSpace[] = " ";
            char sEnd[] = ".sh";
            
            char** tokens;
            tokens = str_split(buf, '$');
            char* procedure = (char*)malloc(sizeof(tokens));
            char* parameters = (char*)malloc(sizeof(tokens));
            strcpy(parameters, "");
            int isProcedureRead = 0;

            if (tokens)
            {
                int i;
                for (i = 0; *(tokens + i); i++)
                {
                    if(!isProcedureRead) {
                        strcpy(procedure, *(tokens + i));
                        isProcedureRead = true;
                    } else strcpy(parameters, *(tokens + i));
                    free(*(tokens + i));
                }
                free(tokens);
            }

            char* conString = new char[strlen(sBegin) + strlen(sEnd) + strlen(sSpace) + sizeof(procedure) + sizeof(parameters) + 1];
            strcpy(conString, sBegin);
            strcat(conString, procedure);
            strcat(conString, sEnd);
            strcat(conString, sSpace);
            strcat(conString, parameters);
            printf("[CLIENT %ld] Request procedure '%s'\n", syscall(SYS_gettid), conString);
            std::string temp = exec(conString);
            const int length = temp.length();
            char* arr = new char[length +1];
            strcpy(arr, temp.c_str());
            for(int i = 0; i < length; i++) {
                buf[i] = arr[i];
            }
            write(c->cfd, buf, sizeof(buf));
        }
    }
    close(c->cfd);
    free(c);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    // Initialization
    pthread_t tid;
    socklen_t slt;
    int sfd, on = 1;
    struct sockaddr_in saddr;

    // Setting up server socket
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(1234);
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
    listen(sfd, 10);

    // Server infinite loop
    while(1) {
        // Listening for clients
        printf("[SERVER] Waiting for connection...\n");
        struct cln* c = (struct cln*)malloc(sizeof(struct cln));
        slt = sizeof(c->caddr);
        c->cfd = accept(sfd, (struct sockaddr*)&c->caddr, &slt);
        pthread_create(&tid, NULL, cthread, c);
        pthread_detach(tid);
    }
    close(sfd);
    
    return EXIT_SUCCESS;   
}