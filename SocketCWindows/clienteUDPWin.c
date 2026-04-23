#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Link com a biblioteca Winsock
#pragma comment(lib, "ws2_32.lib")

//#define SERVER_IP "192.168.39.213" // Endereço IP do servidor
#define PORT 40001             // Porta do servidor
#define BUFFER_SIZE 255

void clear_stdin_buffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char *argv[]) {
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    int bufLength;

    /* check command line args */
    if(argc<2) {
        printf("usage : %s <server> \n", argv[0]);
        exit(1);
    }
    printf("%s\n", argv[1]);
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
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    // 4. Enviar dados
	while (1) {
		strcpy(buffer, "");
		printf("Digite a mensagem: ");
		scanf("%255[^\n]", buffer);
		bufLength = strlen(buffer);
		printf("%d\n", bufLength);
        if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            printf("sendto() falhou. Codigo: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
		clear_stdin_buffer();
		if (bufLength == 0) {
			printf("Helder\n");
			break;
		}
	}
//    printf("Mensagem enviada.\n");
    // 6. Fechar socket e liberar Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}