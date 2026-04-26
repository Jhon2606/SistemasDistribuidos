#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // se ainda não foi definido, define agora

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")  // Extensões Microsoft do Winsock
#pragma comment(lib, "AdvApi32.lib") // Funções avançadas do Windows

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "40002"

void clear_stdin_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int __cdecl main(int argc, char **argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    char sendbuf[255];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    if (argc != 2)
    {
        printf("Usando %s como endereco do host\n", argv[0]);
        return 1;
    }

    printf("Inicializando a biblioteca de Winsock...\n");
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("[-]     WSASturtup falhou! Erro: %d\n\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // aceito IPv4 ou IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    printf("\t-> Resolvendo endereco e porta do servidor...");
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("[-] getaddrinfo() falhou! Erro: %d\n\n", iResult);
        WSACleanup();
        return 1;
    }
    printf("[+] Sucesso!\n");

    printf("Iniciando procedimento de conexao...\n");
    // Lanca tentativas de conexao com o host até obter sucesso... ou nao.
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {

        printf("Criando SOCKET para conexao...\n");
        // Criando socket
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            printf("[-] Socket falhou! Erro: %ld\n\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        printf("Conectando ao host...\n");
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            break;
        }
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("[-] Conexao com o server invalida!\n\n");
        WSACleanup();
        return 1;
    }

    printf("Enviando mensagens...\n");
    while (1)
    {
        strcpy(sendbuf, "");
        printf("Digite a mensagem: ");
        scanf("%255[^\n]", sendbuf);
        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("[-] send() falhou! Erro: %d\n\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
        if (strlen(sendbuf) == 0)
        {
            break;
        }
        clear_stdin_buffer();
    }
    printf("\n### Adeus :( ###\n");
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
/*
main(argc, argv)
       |
       ↓
  argc != 2? ──── SIM ──→ Imprime uso e encerra
       |
      NÃO
       ↓
  WSAStartup()          ← Liga o Winsock
       ↓
  getaddrinfo(argv[1])  ← Resolve IP do servidor
       ↓
  for(ptr=result...)    ← Tenta cada endereço
    socket()            ← Cria socket
    connect()           ← Conecta ao servidor  ← (pode falhar e tentar próximo)
    break
       ↓
  ConnectSocket         ← Conexão OK?
  == INVALID? ──SIM──→ Erro e encerra
       |
      NÃO
       ↓
  while(1)              ← Loop de mensagens
    scanf()             ← Lê do teclado
    send()              ← Envia ao servidor
    strlen==0? ─SIM──→ break
    clear_stdin_buffer()
       ↓
  closesocket()
  WSACleanup()
  return 0
*/