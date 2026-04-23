#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "40002"

int __cdecl main(void) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    printf("Inicializando a biblioteca de Winsock...\n");
    // Inicializando a biblioteca de Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("[-] WSASturtup falhou! Erro: %d\n\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    printf("Resolvendo endereco e porta do servidor...\n");
    // Resolvendo o endereco e porta do servidor
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("[-] getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    printf("Criando SOCKET para conexao...\n");
    // Criando Socket
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("[-] Socket falhou! Erro: %ld\n\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    printf("Setando TCP listening socket...\n");
    printf("bind()...\n");
    // Setup TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
            printf("[-] bind() falhou! Erro: %d\n\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
    }

//    freeaddrinfo(result);

    printf("listen()...\n");
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
            printf("[-] listen() falhou! Erro: %d\n\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
    }

    printf("Aguardando conexoes! accept()...\n");
    // Aceitando um cliente socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
            printf("[-] accept() falhou! Erro: %d\n\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
    }

    closesocket(ListenSocket);

    printf("[+] Servidor pronto para receber dados!!!\n\n");
    // Recebe ate o cliente encerrar a conexao
    do {
		memset(recvbuf, 0, sizeof(recvbuf));
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("%d,   %s\n", iResult, recvbuf);
        }
        else if (iResult == 0)
            printf("[-] Conexao fexada pelo cliente!\n");
        else  {
            printf("[-] recv() falhou! Erro: %d\n", WSAGetLastError());
            break;
        }

    } while (iResult > 0);

    printf("Encerrando...");
    // Fexando a conexao quando terminarmos
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
            printf("[-] shutdown() falhou! Erro: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
    }
    printf("[+] Encerrado!\n");
    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;

}