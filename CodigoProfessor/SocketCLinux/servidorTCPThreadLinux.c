#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <pthread.h>
#include <stdlib.h>

#define LOCAL_SERVER_PORT 40002
#define MAX_MSG 255


void *thread_func(void *arg) {

  int n;
  char msgRet[MAX_MSG], msg[MAX_MSG];
  int *par = (int *)arg;
  int conexao = *par;

//  printf("Conexao = %d\n", conexao);
//  printf("Waiting for data\n");
//  tread infinite loop
  while(1) {

    // init buffer
    memset(msg,0x0,MAX_MSG);
    strcpy(msg, "");
    // receive message
    //cliLen = sizeof(cliAddr);
    n = recv(conexao, msg, sizeof(msg), 0);

    if(n<0) {
//      printf("Cannot receive data \n");
      continue;
    }else if (n == 0){
      break;
    }else {
    // return message
//      memset(msgRet, 0x0, MAX_MSG);
      strcpy(msgRet, "ECO : ");
      strcat(msgRet, msg);
      n = send(conexao, msgRet, strlen(msgRet), 0);
      if(n<0) {
        printf("Cannot send data\n");
        close(conexao);
        exit(1);
      }
      // print received message
//      memset(msgRet, 0x0, MAX_MSG);
//      strcpy(msgRet, "ECO: ");
//      strcat(msgRet, msg);
//      printf("%d recebidos: %s\n",n , msgRet);
    }
  }
}

int main(int argc, char *argv[]) {

  int sd, conexao, rc, n, cliLen;
  struct sockaddr_in cliente, servAddr;
  socklen_t c;
//  char msg[MAX_MSG];
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

  // looping conexao
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
//    printf("cliente conectou: %s : [ %d ] nova porta servidor: %d\n", cliente_ip, cliente_port, conexao);

    pthread_t newThread;
    int err = pthread_create(&newThread, NULL, thread_func, &conexao);
    if (err != 0) {
      printf("Erro ao criar a thread!!\n");
      return -1;
    }
//    printf("Criou a thread vai fazer join\n");
    pthread_detach(newThread);
  }
  close(sd);
  return 0;
}
