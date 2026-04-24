#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

#define REMOTE_SERVER_PORT 40002
#define MAX_MSG 255

void clear_stdin_buffer()
{

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main(int argc, char *argv[])
{

    int sd, rc, i;
    struct sockaddr_in cliAddr, servidor;
    struct hostent *h;

    if (argc < 2)
    {
        printf("Usando: %s como servidor\n", argv[0]);
        exit(1);
    }

    h = gethostbyname(argv[1]);
    if (h == NULL)
    {
        printf("%s: não foi possível resolver o host '%s'\n", argv[0], argv[1]);
        exit(1);
    }

    printf("%s: sending data to '%s' (IP : %s) \n", argv[0], h->h_name, inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

    /*
     * inet_ntoa() converte o endereço IP binário (32 bits) de volta
     * para string legível ("192.168.0.1") — usado só para exibir na tela.
     */

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        printf("%s: não foi possível abrir o socket\n", argv[0]);
        exit(1);
    }

    servidor.sin_family = h->h_addrtype;                                       // Família de endereço (AF_INET para IPv4)
    memcpy((char *)&servidor.sin_addr.s_addr, h->h_addr_list[0], h->h_length); // memcpy(destino, origem, bytes)
    servidor.sin_port = htons(REMOTE_SERVER_PORT);                             // htons Converte a porta do formato do processador para o formato padrão de rede

    if (connect(sd, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        printf("Nao foi possivel conectar\n");
        return -1;
    }

    while (1)
    {
        char msg[MAX_MSG];
        strcpy(msg, "");
        clear_stdin_buffer();
        printf("Digite a mensagem: ");
        scanf("%255[^\n]", msg);
        rc = send(sd, msg, strlen(msg), 0);

        if (rc < 0)
        {
            printf("%s: nao foi possivel enviar os dados  %d \n", argv[0], i - 1);
            close(sd);
            exit(1);
        }
    }
    close(sd);
    return 1;
}