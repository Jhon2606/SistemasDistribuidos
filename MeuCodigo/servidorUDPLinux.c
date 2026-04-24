#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define LOCAL_SERVER_PORT 40001 //Por que porta 40001? Portas abaixo de 1024 são reservadas para o sistema e exigem permissão de root. Qualquer valor de 1024 a 65535 é livre para uso.
#define MAX_MSG 255

int main(int argc, char *argv[]){
    int sd, conexao, rc, n, cliLen;
    struct sockaddr_in cliente, servAddr;
    char msg[MAX_MSG];
    
    char *cliente_ip;
    int cliente_port;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd<0){
        printf("%s: não foi possivel abri o socket\n",argv[0]);
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // Aceita de qualquer interface de rede
    servAddr.sin_port = htons(LOCAL_SERVER_PORT);

    rc = bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr)); //O bind() amarra o socket a um endereço e porta específicos.
    if(rc<0){
        printf("%s: nao foi possivel vincular o numero de porta %d", argv[0], LOCAL_SERVER_PORT);
        exit(1);
    }

    while (1){
        memset(msg, 0x0, MAX_MSG); // Limpa o buffer antes de receber
        n = read(sd, msg, MAX_MSG);
        /*
         * Lê até MAX_MSG bytes do socket e armazena em msg. Pelo read() o programa para aqui e fica esperando até chegar alguma mensagem. O código usa read() em * vez de recvfrom(). Com recvfrom() também retornaria o endereço (IP e porta) de quem enviou, o que permitiria responder ao cliente. O read() recebe os * dados mas descarta.
        */

        if(n < 0){
            printf("%s: nao foi possivel receber os dados \n", argv[0]);
            continue;  
        }
        printf("recebido: %s\n", msg);
    }
    

}