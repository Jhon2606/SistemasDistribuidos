#include <sys/types.h>
#include <sys/socket.h>
//bibliotecas centrais para craição de socket, contendo definições de estruturas básicas para chamadas de sistema como: socket() bind() sendto()
#include <netinet/in.h>
#include <arpa/inet.h>
//fornecem estruturas para endereços de internet (sockaddr_in) e funções de manipulação de IPs (inet_adr())
//obs: inet_adr(): converte strings de IP para binarios da rede
#include <netdb.h> //resolução de nomes DNS usando a função gethostbyname()
#include <stdio.h>
#include <stdlib.h>//para usar o exit(1), mostrando para o sistema que a operação falhou
#include <string.h>//manipulação de strings
#include <unistd.h>//biblioteca padrão que dá acesso à API do padrão POSIX, basicamnte trata tudo como um arquivo, por exemplo um socket seria apenas um file descriptor, isntruindo o kernel de algumas formas
#include <sys/time.h>//estrutura de tempos e relogio do sistema

#define REMOTE_SERVER_PORT 40001 //o servidor vai escutar essa porta
#define MAX_MSG 100

void clear_stdin_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}
//limpa o buffer, até achar o \n correspondente a tecla enter ou algum erro

int main(int argc, char *argv[]){
    int sd, rc, i = 0;//sd: socket descriptor,  rc: status da operação 
    struct sockaddr_in cliAddr, remoteServAddr;
    //sockaddr_in: guarda a família do endereço (AF_INET), a porta de rede e o endereço IP de destino
    //cliAddr: endereço local da máquina
    //remoteServAddr: coordenada de destino, sendo preenchido com o IP de porta 40001
    struct hostent *h; //endereço de memória que contém o IP descoberto pelo DNS
    
    if(argc < 2){ //verifica se passou o servidor corretamentne como segundo argumento na linha de comando
        printf("Usando: %s como servidor\n", argv[0]);
        exit(1);
    }
    h = gethostbyname(argv[1]);
    if(h == NULL){//verifica o host não foi encontrado
        printf("%s: não foi possível resolver o host '%s'\n", argv[0], argv[1]);
        exit(1);
    }

    printf("%s: enviando dados para '%s' (IP: %s)\n", argv[0], h->h_name,
    inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));
    //inet_ntoa(*(struct in_addr *)h->h_addr_list[0])): Pega o primeiro IP do host e transforma em texto
    
    sd = socket(AF_INET, SOCK_DGRAM, 0); //pede ao kernel do Linux para criar um endpoint de comunicação (um numero inteiro que o sistema usa para rastrear a conexão)
    //AF_INET: família de endereços IPPV4
    //SOCK_DGRAM: define o uso so protocolo UDP especificando que o sistema usará datagramas
    //0: sistema escolhe o protocolo padrão
    if(sd<0){
        printf("%s: não foi possível abrir o socket\n", argv[0]);
        exit(1);
    }
    memset(&remoteServAddr, 0, sizeof(remoteServAddr)); //zera a estrutura da memoria de remoteServAddr
    remoteServAddr.sin_family = AF_INET;
    remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT); //converte a porta 40001 do formato da sua máquina local para o formato da rede
    remoteServAddr.sin_addr.s_addr =inet_addr(argv[1]); //converte o IP em binário que o sistema de redes entende

    char msg[MAX_MSG];
    while (1){
        strcpy(msg, "");
        printf("Digite a mensagem: ");
        scanf("%99[^\n]", msg); //lê tudo até encontrar \n, alterei para 99 para evitar buffer overflow
        rc = sendto(sd, msg, strlen(msg), 0, (struct sockaddr *)&remoteServAddr, sizeof(remoteServAddr)); //transmite os dados para o servidor e verifica se a operação foi bem-sucedida
        clear_stdin_buffer();

        if(rc<0){
            printf("%s: não foi possível enviar os dados %d \n", argv[0], i-1);
            close(sd);
            exit(1);
        }
    }
    return 1;
}
