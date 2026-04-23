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
        printf("Usando: %s como servidor\n", argv[1]);
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
    
}
