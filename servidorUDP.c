/* fpont 12/99 */
/* pont.net    */
/* udpServer.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */

#define LOCAL_SERVER_PORT 40001
#define MAX_MSG 100

int main(int argc, char *argv[]) {

  int sd, conexao, rc, n, cliLen;
  struct sockaddr_in cliente, servAddr;
  char msg[MAX_MSG];
// para pegar o IP e porta do cliente
  char *cliente_ip;
  int cliente_port;

  /* socket creation */
  sd=socket(AF_INET, SOCK_DGRAM, 0);
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

  /* server infinite loop */
  while(1) {
    /* init buffer */
    memset(msg,0x0,MAX_MSG);
    /* receive message */
//    cliLen = sizeof(cliAddr);
    n = read(sd, msg, MAX_MSG);

    if(n<0) {
      printf("%s: cannot receive data \n",argv[0]);
      continue;
    }

    /* print received message */
    printf("recebido: %s\n", msg);

//    cliente_ip = inet_ntoa(cliente.sin_addr);
//    cliente_port = ntohs(cliente.sin_port);
//    printf("cliente conectou: %s : [ %d ]\n", cliente_ip, cliente_port);
//    printf("Waiting for data\n");

  }/* end of server infinite loop */

return 0;

}

