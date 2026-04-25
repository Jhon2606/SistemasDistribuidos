#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Link com a biblioteca Winsock
#pragma comment(lib, "ws2_32.lib")

#define PORT 40001 // Porta do servidor
#define BUFFER_SIZE 255

int main()
{
    WSADATA wsa;
    SOCKET serverSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    int iResult = 0;

    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Falha ao inicializar. Codigo: %d\n", WSAGetLastError());
        return 1;
    }

    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        printf("Falha ao criar socket. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Aceita todas as interfaces
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Bind falhou: %d", WSAGetLastError());
        return 1;
    }

    printf("Recebendo dados ....\n");

    while (1)
    {
        strcpy(buffer, "");
        iResult = recv(serverSocket, buffer, BUFFER_SIZE, 0);
        if (iResult > 0)
        {
            printf("%s\n", buffer);
        }
        else if (iResult == 0)
            printf("Conexao fechada pelo cliente...\n");
        else
        {
            printf("recv() falhou! Erro: %d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}