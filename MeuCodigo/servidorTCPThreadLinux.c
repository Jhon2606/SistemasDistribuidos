#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#define LOCAL_SERVER_PORT 40002
#define MAX_MSG 255

void *thread_func(void *arg)
{
    int n;
    char msgRet[MAX_MSG], msg[MAX_MSG];
    int *par = (int *)arg;
    int conexao = *par;

    while (1)
    {
        memset(msg, 0x0, MAX_MSG);
        n = recv(conexao, msg, sizeof(msg), 0); // é a função que recebe dados do socket. Ela fica esperando até dados chegarem
        if (n < 0)
        {
            continue;
        }
        else if (n == 0)
        {
            break;
        }
        else
        {
            // Monta a resposta: "ECO : " + a mensagem recebida
            strcpy(msgRet, "ECO : ");
            strcat(msgRet, msg);
            n = send(conexao, msgRet, strlen(msgRet), 0); // Envia a resposta de volta para o cliente. O send manda strlen(msgRet) bytes pela conexão
            if (n < 0)
            {
                printf("nao foi possivel enviar os dados\n");
                close(conexao);
                exit(1);
            }
            printf("recebidos: %s\n", msgRet);
        }
    }
}

int main(int argc, char *argv[])
{
    int sd, conexao, rc, n, cliLen;
    struct sockaddr_in cliente, servAddr;
    socklen_t c;
    char *cliente_ip;
    int cliente_port;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        printf("%s: não foi possível abrir o socket \n", argv[0]);
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(LOCAL_SERVER_PORT);
    rc = bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (rc < 0)
    {
        printf("%s: nao foi possivel vincular o numero de porta %d \n", argv[0], LOCAL_SERVER_PORT);
        exit(1);
    }

    listen(sd, 3);

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

        pthread_t newThread; // Cria uma thread passando o fd da conexão
        int err = pthread_create(&newThread, NULL, thread_func, &conexao);
        if (err != 0)
        {
            printf("Erro ao criar a thread!!\n");
            return -1;
        }
        pthread_detach(newThread); // pthread_detach diz ao sistema: "quando essa thread terminar, limpe os recursos automaticamente, eu não vou esperar por ela." Isso permite que o loop principal volte logo para aceitar o próximo cliente
    }
    close(sd);
    return 0;
}
/*
main()
  │
  ├── Cria socket (sd)
  ├── Bind na porta 40002
  ├── Listen (fica escutando)
  │
  └── while(1):
        ├── accept() ← ESPERA cliente conectar
        ├── Cria Thread para esse cliente
        │     └── thread_func():
        │           └── while(1):
        │                 ├── recv() ← recebe mensagem
        │                 └── send() ← envia "ECO : mensagem"
        └── Volta para o accept() (próximo cliente)

MAIN (loop)                  THREAD 1           THREAD 2           THREAD 3
    │
    ├─ accept() ← Cliente 1 conecta
    ├─ pthread_create → ────────────────────► roda sozinha...
    ├─ pthread_detach
    │
    ├─ accept() ← Cliente 2 conecta
    ├─ pthread_create → ─────────────────────────────────────► roda sozinha...
    ├─ pthread_detach
    │
    ├─ accept() ← Cliente 3 conecta
    ├─ pthread_create → ──────────────────────────────────────────────────────► roda sozinha...
    ├─ pthread_detach
    │
    ├─ accept() ← esperando o próximo...
*/