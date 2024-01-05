#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
#elif __unix__
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

static void ErrExit(char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

static int Read(SOCKET sock, char* resvline, int len)
{
#ifdef _WIN32
    return recv(sock, resvline, len, 0);
#elif __unix__
    return read(sock, resvline, len);
#endif
}

int main(int argc, char* argv[])
{
    if (argc != 3) ErrExit("usage: command <IP> <port>\n");
#ifdef _WIN32
    // ------------------------------------------------------------------------------------------
    // initialize Winsock
    // MAKEWORD(2, 2) is a version, 
    // and wsaData will be filled with initialized library information.
    WSADATA wsaData;
    int wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData); 

    if (wsResult != 0) {
        ErrExit("WSAStartup() error!");

    }
    printf("WSAStartup() succeed.\n");
#endif
    // ------------------------------------------------------------------------------------------
    // create client socket
    // Protocol Family: PF_INET, PF_INET6, PF_LOCAL, PF_PACKET, PF_IPX
    // Type: SOCK_STREAM, SOCK_DGRAM
    SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) ErrExit("socket() error!");
    printf("socket() succeeded.\n");
    // ------------------------------------------------------------------------------------------
    // create server address
    SOCKADDR_IN serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  // IPv4
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2])); // htons = host to networtk short
    // ------------------------------------------------------------------------------------------
    // connect to server
    printf("ready to connect to %s:%s...\n", argv[1], argv[2]);
    int connResult =connect(sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)); 
    if (connResult == SOCKET_ERROR){
        ErrExit("connect() error!");
        //If it does create an error, we want to cleanup, since we already have a Socket
        /* closesocket(sock);
        WSACleanup(); */
    }
    printf("connect() succeeded.\n");
    // ------------------------------------------------------------------------------------------
    // read data
    char buff[30];
    int str_len = 0;
/* #if 0
    // to show that TCP transportation doesn't have boundary,
    // we can read data multiple time.
    int read_len = 0;
    int idx = 0;
    
    while (read_len = recv(sock, &buff[idx++], 1, 0))
    {
        printf("reading one time...[%d]\n", idx);
        if (read_len == -1) ErrExit("read() error!");
        str_len += read_len;
    }


    
#else */
    // Receive "Welcome"
    str_len = recv(sock, buff, sizeof(buff) - 1, 0);
    if (str_len == -1) ErrExit("read() error!");
    printf("read() succeeded. str_len=%d\nReceived Message from server: %s\n", str_len, buff);

    // Send random number [1,10] 
    int sent;
    do{
        memset(buff,0,30);
        sent = rand() %9+1;
        buff[0] = '0'+sent;
        buff[1] = '\n';
        printf("Sending:%s",buff);
        int sendResult = send(sock, buff, sizeof(buff), 0);
        //sleep(10);
    }while(sent != 9);//}while(true);
/* #endif */

    // ------------------------------------------------------------------------------------------
    // close socket
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#elif __unix__
    close(sock);
#endif
    // ------------------------------------------------------------------------------------------
}