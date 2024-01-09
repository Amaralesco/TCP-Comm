#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------------------------------------
// How to run this file
// gcc server.cpp -o server && ./server 7777
//
// To test a connection, optionally you can use putty with IP 127.0.0.1 and port used above
// in this case 7777 
// ------------------------------------------------------------------------------------------

#ifdef _WIN32
    #include <winsock2.h>
#elif __unix__ // Previously i was trying to use the following for Windows and it was not working
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    typedef int SOCKET;
    #define INVALID_SOCKET -1
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    #define SOCKET_ERROR -1
#else
    #error "Unknown"
#endif

using namespace std;

static void ErrExit(char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argc != 2) ErrExit("usage: command <port>\n");
#ifdef _WIN32
    // ------------------------------------------------------------------------------------------
    // initialize Winsock
    // MAKEWORD(2, 2) is a version, 
    // and wsaData will be filled with initialized library information.
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrExit("WSAStartup() error!");
    printf("WSAStartup() succeeded.\n");
#endif
    // ------------------------------------------------------------------------------------------
    // create server socket
    // Protocol Family: PF_INET, PF_INET6, PF_LOCAL, PF_PACKET, PF_IPX
    // Type: SOCK_STREAM, SOCK_DGRAM
    SOCKET serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == INVALID_SOCKET) ErrExit("socket() error!");
    printf("socket() succeeded.\n");
    // ------------------------------------------------------------------------------------------
    // create server address
    SOCKADDR_IN serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  // IPv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    // ------------------------------------------------------------------------------------------
    // bind socket
    if (bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) ErrExit("listen() error!");
    printf("bind on port %s...\n", argv[1]);
    // ------------------------------------------------------------------------------------------
    // listen socket
    if (listen(serv_sock, 5) == SOCKET_ERROR) ErrExit("listen() error!");
    printf("listen() succeeded.\n");
    // ------------------------------------------------------------------------------------------
    // accept socket
    SOCKADDR_IN clnt_addr;
    int sz_clnt_addr = sizeof(clnt_addr);
    SOCKET clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, (socklen_t*) &sz_clnt_addr);
    if (clnt_sock == INVALID_SOCKET) ErrExit("accept() error!");
    printf("accept() succeeded.\n");

    // ------------------------------------------------------------------------------------------
    // send data
    char buff[4096];
    //buff = "Welcome";
    int sent_len = send(clnt_sock, "Welcome", 8, 0);
    printf("sending %d bytes.\n", sent_len);  // sending 8 bytes.


    while (true){
        //ZeroMemory(buff,4096); // Fills our buffer with zeros.
        memset(buff,0,4096);
        

        //Wait for client to send data
        int read_len = recv(clnt_sock, buff, 4096,0);
        if(read_len == SOCKET_ERROR){ // SOCKET_ERROR is -1
            printf("Error in recv(). Quitting\n");
            break;
        } 

        if(read_len == 0){
            printf("Client disconnected\n");
            break;
        }
        if (buff[0] == 'q' && read_len == 1)
        {
            printf("Terminating upon Client request\n");
            break;
        }
        

        //send(clnt_sock, buff, read_len+1,0 );
        printf("%s",buff);


    }

    
    // close socket // Needs to be changed for multiple connections
#ifdef _WIN32
    closesocket(clnt_sock);
    closesocket(serv_sock);
    WSACleanup();
#elif __unix__
    close(clnt_sock);
    close(serv_sock);
#endif
    // ------------------------------------------------------------------------------------------
}

