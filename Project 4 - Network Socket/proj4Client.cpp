/*
    Name: CJ Hess
    Date: 4/17/2023
    File: proj4Client.cpp
*/

#include <netdb.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>

#define SUCCESS 0
#define USAGE_ERROR 1
#define HOST_ERROR 2
#define SOCKET_ERROR 3
#define CONNECT_ERROR 4
#define READ_ERROR 5

using namespace std;

int do_client(char *serverName, int portNum);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Error: Usage: " << argv[0] << " <IP-Address> <Port Num>" << endl;
        return USAGE_ERROR;
    }
    return do_client(argv[1], atoi(argv[2]));
}

int do_client(char *serverName, int portNum)
{
  struct hostent *serverEntry;
  serverEntry = gethostbyname(serverName);
  if (!serverEntry)
    {
      cerr << "gethostbyname() failed on: " << serverName << endl;
      return HOST_ERROR;
    }

  struct sockaddr_in serverInfo;
  serverInfo.sin_family = AF_INET; // IPV4
  serverInfo.sin_addr = * (struct in_addr *) serverEntry->h_addr_list[0];
  serverInfo.sin_port = htons(portNum); 
  
  int mySocket = socket(AF_INET, SOCK_STREAM, 0);
  if (mySocket == 0) // socket() call failed
    {
      cerr << "socket() call for creating socket failed!" << endl;
      return SOCKET_ERROR;
    }

  int rVal =
    connect(mySocket, (struct sockaddr *) &serverInfo, sizeof(serverInfo));

  if (rVal<0)
    {
      cerr << "connect() failed!" << endl;
      return CONNECT_ERROR; 
    }

  // This is where we store our input from the client
  string buffer;

  // Use getline for the add command specifically, we handle picking out the number
  // on the server side
  std::getline(cin, buffer);

  // send input to the server
  char *cbuff = (char *) buffer.c_str(); 

  int needed = buffer.length();

  // need to put in a loop ...
  while(needed > 0) {
    int num_written= write(mySocket, cbuff, needed);

    needed -= num_written;
    cbuff += num_written;
  }

  // Read back whatever the server is supposed to send back
  char bufferRead[81];
  int charsRead;

  charsRead = read(mySocket, bufferRead, 80);

  while (charsRead > 0) {
    bufferRead[charsRead] = '\0'; // terminate string sentfrom server.
    cout << bufferRead;

    charsRead = read(mySocket, bufferRead, 80);
  }

  if (charsRead <0) {
    cerr << "read() failed. " << endl;
    return READ_ERROR;
  }


  close(mySocket); // let server know we are done. 
  
  return SUCCESS;
}