#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>         // close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "40002"

void clear_stdin_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char **argv) 
{
  int ConnectSocket = -1;
  struct addrinfo *result = NULL, *ptr = NULL, hints;
  char sendbuf[255];
  char recvbuf[DEFAULT_BUFLEN];
  int iResult;
  int recvbuflen = DEFAULT_BUFLEN;

  // Usage
  if (argc != 2) {
    printf("\t\tUsage: %s endereco do host\n", argv[0]);
    return 1;
  }

  printf("\n\t-> Inicializando sockets...");
  // (Linux não precisa de WSAStartup)
  printf("\t\t[+] OK!\n");

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  printf("\t-> Resolvendo endereco e porta do servidor...");
  iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
  if (iResult != 0) {
    printf("\t\t[-] getaddrinfo() falhou!\n\n");
    return 1;
  }
  printf("\t\t[+] Sucesso!\n");

  printf("\t-> Iniciando procedimento de conexao...");

  for(ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    printf("\n\t-> Criando SOCKET para conexao...");
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (ConnectSocket == -1) {
      perror("\t\t\t\t[-] socket falhou");
      return 1;
    }
    printf("\t\t\t[+] Socket criado com sucesso!\n");

    printf("\t-> Conectando ao host...");
    iResult = connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);
    if (iResult == -1) {
      close(ConnectSocket);
      ConnectSocket = -1;
      continue;
    }
    break;
  }

  freeaddrinfo(result);

  if (ConnectSocket == -1) {
    printf("\t\t\t\t[-] Conexao com o server invalida!\n\n");
    return 1;
  }
  printf("\t\t\t\t[+] Sucesso!!!\n");

  printf("\t-> Enviando mensagens...\n");

  while (1) {
    strcpy(sendbuf, "");
    printf("Digite a mensagem: ");
    scanf("%254[^\n]", sendbuf);

    iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
    if (iResult == -1) {
      perror("\t\t\t\t[-] send() falhou");
      close(ConnectSocket);
      return 1;
    }

    if (strlen(sendbuf) == 0) {
      printf("Helder\n");
      break;
    }

    clear_stdin_buffer();
  }

  printf("\n\n\t### Bye!!! ###\n");

  close(ConnectSocket);

  return 0;
}