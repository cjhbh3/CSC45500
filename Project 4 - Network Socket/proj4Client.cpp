/*
    Name: CJ Hess
    Date: 4/17/2023
    File: proj4Client.cpp
*/

#include <netdb.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#define SUCCESS 0
#define USAGE_ERROR 1
#define HOST_ERROR 2
#define SOCKET_ERROR 3
#define CONNECT_ERROR 4
#define READ_ERROR 5

using namespace std;

int do_client(char *serverName);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Error: Usage: " << argv[0] << " <TOD-server>" << endl;
        return USAGE_ERROR;
    }
    return do_client(argv[1]);
}

int do_client(char *serverName)
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
  serverInfo.sin_port = htons(13); 
  
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

  // now, we're ready to "talk with" the server!
  char buffer[81];
  int charsRead;

  charsRead = read(mySocket, buffer, 80);

  while (charsRead>0)
    {
      buffer[charsRead] = '\0'; // terminate string sentfrom server.
      cout << buffer;

      charsRead = read(mySocket, buffer, 80);
    }

  if (charsRead <0)
    {
      cerr << "read() failed. " << endl;
      return READ_ERROR;
    }


  close(mySocket); // let server know we are done. 
  
  return SUCCESS;
}