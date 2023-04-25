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
#include <cstring>

using namespace std;

struct mywork_struct {
    int with_sock;
    struct sockaddr_in *from_cli;
};

// Accumulator variable
int accumulator;

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

    accumulator = 0;
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
      
      struct mywork_struct *params = new mywork_struct;
      params -> with_sock = connected_sock;
      params -> from_cli = &from;
      
      pthread_create(new pthread_t, NULL, do_work, (void *) params);
    }
}

void* do_work(void *param) {
  struct mywork_struct *mwsp = (mywork_struct *) param;
  int conn_sock = mwsp -> with_sock;
  struct sockaddr_in *client_addr = mwsp -> from_cli;

  // suggested idea: delete now unused structure:
  delete mwsp;

  // Read from Client
  char bufferRead[81];
  int charsRead;

  charsRead = read(conn_sock, bufferRead, 80);

  string buffer;

  // Read input from client
  if (charsRead > 0) {
    // Grab token of input
    // Used because we use getline to grab the add command specifically
    char *tok = strtok((char *) bufferRead, " ");

    // get-clear-add, probably should add an else that says command not valid
    if (strcmp(tok, "get") == 0) {
      buffer = to_string(accumulator);
    }
    else if (strcmp(tok, "clear") == 0) {
      accumulator = 0;
    }
    else if (strcmp(tok, "add") == 0) {
      tok = strtok(NULL, " ");
      accumulator += atoi(tok);
      buffer += to_string(accumulator);
    }
    else {
      buffer = "Command not valid!";
    }
  }

  buffer += '\n';

  // build the response string to send to the client

  char *cbuff = (char *) buffer.c_str(); 

  int needed = buffer.length();

  // need to put in a loop ...
  while(needed > 0) {
    int num_written= write(conn_sock, cbuff, needed);

    needed -= num_written;
    cbuff += num_written;
  }

  close(conn_sock);  // closed connected socket - we are done with this client!

  return nullptr;
}