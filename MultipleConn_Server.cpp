//#######################################################
//# FILE UNDER DEVELOPMENT - WORK IN PROGRESS           #
//#######################################################~

// ------------------------------------------------------------------------------------------
// How to run this file
// gcc MultipleConn_Server.cpp -o MultipleConn_Server && ./MultipleConn_Server 7777
//
// To test a connection, optionally you can use putty with IP 127.0.0.1 and port used above
// in this case 7777 
// ------------------------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
#elif __unix__ // Previously i was trying to use the following for Windows and it was not working
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <string.h>
    #include <sys/select.h>
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
    // Tell Winsock the socket is for listening 
    if (listen(serv_sock, 5) == SOCKET_ERROR) ErrExit("listen() error!");
    printf("listen() succeeded.\n");

    //Create the Master file Descriptor
    fd_set master;
    FD_ZERO(&master);

    //Add our Listening socket, responsible to detect incoming connections
    FD_SET(serv_sock, &master);

    bool close = false;
    //Handle multiple connections
    while(!close){

        //We create a copy, because everytime we call Select it destroys the FD
        fd_sset copy = master;

        /* The select function determines the status of one or more sockets, waiting if necessary, to perform synchronous I/O.*/
        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        //Our sockets will be a list, in which we will iterate through all the connections or check for new ones
        for(int i = 0; i<socketCount; i++){
            SOCKET sock = copy.fd_array[i];
            if(sock == serv_sock){
                // Accept a new connection //TODO REMOVE THESE 2 lines if needed
                SOCKADDR_IN clnt_addr;
                int sz_clnt_addr = sizeof(clnt_addr);   
                SOCKET clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, (socklen_t*) &sz_clnt_addr);

                // Add the new connection to the list
                FD_SET(clnt_sock, &master);
                
                // Send a Welcome message to the new client
                int sent_len = send(clnt_sock, "Welcome", 8, 0);
            }
            else{
                // Accept a new message
                char buff[4096];
                memset(buff,0,4096);

                //Wait for client to send data
                int read_len = recv(sock, buff, 4096,0);
                if(read_len == SOCKET_ERROR){ // SOCKET_ERROR is -1
                    printf("Error in recv(). Quitting\n");
                    //Drop this particular Client
/*                     closesocket(sock);
                    FD_CLR(sock, &master); */
                } 

                if(read_len == 0){
                    printf("Client disconnected\n");
                    //Drop this particular Client
/*                     closesocket(sock);
                    FD_CLR(sock, &master); */
                }
                if (buff[0] == 'q' && read_len == 1)
                {
                    printf("Terminating upon Client request\n");
                    close = true;
                    break;
                }
            
            
            }
        }



    }




    // ------------------------------------------------------------------------------------------
    // close socket 
     #ifdef _WIN32
        WSACleanup();
    #elif __unix__
/*         close(clnt_sock);
        close(serv_sock); */
    #endif */


}
