/*
    Name: CJ Hess
    Date: 4/17/2023
    File: proj4Server.cpp
*/

#include <arpa/inet.h>
#include <unistd.h>

#include <pthread.h>

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

struct mywork_struct {
    int with_sock;
    struct sockaddr_in *from_cli;
};

#define USAGE_ERROR 1
#define SOCK_ERROR 2
#define BIND_ERROR 3
#define LISTEN_ERROR 4

#define MAX_WAITERS 25

int do_server(int portNum);
void* do_work(void *pthreadParam);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <port-number>" << endl;
        return USAGE_ERROR;
    }

    return do_server(atoi(argv[1]));
}

int do_server(int portNum) {
    int listen_sock;

    struct sockaddr_in local_addr;

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_sock < 0) {
        cerr << "Could not create listening socket" << endl;
        return SOCK_ERROR;
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(portNum);

    int status = bind(listen_sock, (sockaddr*) &local_addr, sizeof(local_addr));
    if (status!=0) {
        cerr << "bind() error" << endl;
        return BIND_ERROR;
    }

    status = listen(listen_sock, MAX_WAITERS);
    if (status!=0) {
        cerr << "listen() error" << endl;
        return LISTEN_ERROR;
    }
    
    while( true ) {
      int connected_sock;        // socket to build for incoming client
      struct sockaddr_in from;   // holds client address data
      unsigned int from_len;     // holds size of client address
      
      from_len = sizeof(from);
      
      connected_sock = accept(listen_sock, (sockaddr*) &from, &from_len);
      // do_work(connected_sock, &from); 
      struct mywork_struct *params = new mywork_struct;
      params -> with_sock = connected_sock;
      params -> from_cli = &from;
      
      pthread_create(new pthread_t, NULL, do_work, (void *) params);
    }
}

//void do_work(int conn_sock, struct sockaddr_in *client_addr)
void* do_work(void *param) {
  struct mywork_struct *mwsp = (mywork_struct *) param;
  int conn_sock = mwsp -> with_sock;
  struct sockaddr_in *client_addr = mwsp -> from_cli;

  // suggested idea: delete now unused structure:
  delete mwsp;
  
  string buffer;

  // build the response string to send to the client
  buffer =  "Your IP address is: ";
  buffer += inet_ntoa(client_addr->sin_addr);
  buffer += '\n';

  char *cbuff = (char *) buffer.c_str(); 

  int needed = buffer.length();

  // need to put in a loop ...
  while(needed>0)
    {
      int num_written= write(conn_sock, cbuff, needed);

      needed -= num_written;
      cbuff += num_written;
    }

  
  cout << "LOG: processed a connection from "
       <<  inet_ntoa(client_addr->sin_addr) << endl;

  close(conn_sock);  // closed connected socket - we are done with this client!

  return nullptr;
}