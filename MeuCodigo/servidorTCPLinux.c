#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define LOCAL_SERVER_PORT 40002
#define MAX_MSG 255

int main(int argc, char *argv[])
{
    int sd, conexao, rc, n, cliLen;
    struct sockaddr_in cliente, servAddr;
    socklen_t c;
    char msg[MAX_MSG];

    char *cliente_ip;
    int cliente_port;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        printf("%s: não foi possível abrir o socket\n", argv[0]);
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(LOCAL_SERVER_PORT);
    rc = bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (rc < 0)
    {
        printf("%s: nao foi possivel vincular o numero de porta %d", argv[0], LOCAL_SERVER_PORT);
        exit(1);
    }

    listen(sd, 3); // coloca o socket pra escutar, com fila de até 3 conexões esperando

    while (1)
    {
        printf("Aguardando conexao...\n");
        c = sizeof(cliente);
        conexao = accept(sd, (struct sockaddr *)&cliente, (socklen_t *)&c);
        if (conexao < 0)
        {
            perror("Erro ao receber conexao\n");
            return -1;
        }
        cliente_ip = inet_ntoa(cliente.sin_addr);
        cliente_port = ntohs(cliente.sin_port);
        printf("cliente conectou: %s na porta [ %d ]\n", cliente_ip, cliente_port);
        printf("Esperando Dados...\n");

        while (1)
        {
            memset(msg, 0x0, MAX_MSG);              // zerando tudo
            n = recv(conexao, msg, sizeof(msg), 0); // retorna qnts bytes chegaram
            if (n < 0)
            {
                printf("%s: não foi possivel receber os dados \n", argv[0]);
                break;
            }
            else if (n == 0)
            {
                break;
            }
            else
            {
                printf("%s\n", msg);
                char *msgvolta = "mensagem recebida!";
                send(conexao, msgvolta, strlen(msgvolta), 0);
            }
        }
        close(conexao);
    }
    close(sd);
    return 0;
}