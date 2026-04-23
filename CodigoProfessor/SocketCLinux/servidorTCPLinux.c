#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h>

#define LOCAL_SERVER_PORT 40002
#define MAX_MSG 255

int main(int argc, char *argv[]) {

  int sd, conexao, rc, n, cliLen;
  struct sockaddr_in cliente, servAddr;
  socklen_t c;
  char msg[MAX_MSG];
// para pegar o IP e porta do cliente
  char *cliente_ip;
  int cliente_port;

  /* socket creation */
  sd=socket(AF_INET, SOCK_STREAM, 0);
  if(sd<0) {
    printf("%s: cannot open socket \n",argv[0]);
    exit(1);
  }

  /* bind local server port */
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(LOCAL_SERVER_PORT);
  rc = bind (sd, (struct sockaddr *) &servAddr,sizeof(servAddr));
  if(rc<0) {
    printf("%s: cannot bind port number %d \n",
           argv[0], LOCAL_SERVER_PORT);
    exit(1);
  }

  listen(sd, 3);

  while (1) {
    printf("Aguardando conexao!!\n");
	c = sizeof(cliente);
    conexao = accept(sd, (struct sockaddr *) &cliente, (socklen_t*) &c);
    if (conexao < 0) {
      perror("Erro ao receber conexao\n");
      return -1;
    }
    cliente_ip = inet_ntoa(cliente.sin_addr);
    cliente_port = ntohs(cliente.sin_port);
    printf("cliente conectou: %s : [ %d ]\n", cliente_ip, cliente_port);
    printf("Waiting for data\n");
    // server infinite loop
//    int count = 0;
    while(1) {
      // init buffer
      memset(msg,0x0,MAX_MSG);
      // receive message
      n = recv(conexao, msg, sizeof(msg), 0);

      if(n<0) {
        printf("%s: cannot receive data \n",argv[0]);
        continue;
      }else if (n == 0){
        break;
      }else {
        // print received message
        printf("%s\n",n , msg);
      }
//      count++;
    }
	close(conexao);
  }
  close(sd);
  return 0;