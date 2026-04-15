#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Link com a biblioteca Winsock
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "192.168.39.213" // Endereço IP do servidor
#define PORT 40001             // Porta do servidor
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    char *message = "Ola, servidor!";

    // 1. Inicializar Winsock
    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Falha ao inicializar. Codigo: %d\n", WSAGetLastError());
        return 1;
    }

    // 2. Criar socket UDP (SOCK_DGRAM)
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        printf("Falha ao criar socket. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // 3. Configurar endereço do servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 4. Enviar dados
    printf("Enviando dados para %s:%d...\n", SERVER_IP, PORT);
    if (sendto(clientSocket, message, strlen(message), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("sendto() falhou. Codigo: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    printf("Mensagem enviada.\n");
/*
    // 5. Receber resposta (opcional no UDP)
    struct sockaddr_in fromAddr;
    int fromLen = sizeof(fromAddr);
    int recvLen = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&fromAddr, &fromLen);
    
    if (recvLen != SOCKET_ERROR) {
        buffer[recvLen] = '\0';
        printf("Resposta do servidor: %s\n", buffer);
    }
*/
    // 6. Fechar socket e liberar Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}