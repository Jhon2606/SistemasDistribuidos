#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */
#include <stdlib.h>

#define REMOTE_SERVER_PORT 40001
#define MAX_MSG 100

void clear_stdin_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char *argv[]) {

  int sd, rc, i;
  struct sockaddr_in cliAddr, remoteServAddr;
  struct hostent *h;

  /* check command line args */
  if(argc<2) {
    printf("usage : %s <server> \n", argv[0]);
    exit(1);
  }

  /* get server IP address (no check if input is IP address or DNS name */
  h = gethostbyname(argv[1]);
  if(h==NULL) {
    printf("%s: unknown host '%s' \n", argv[0], argv[1]);
    exit(1);
  }

  printf("%s: sending data to '%s' (IP : %s) \n", argv[0], h->h_name,
         inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

//  remoteServAddr.sin_family = h->h_addrtype;
//  memcpy((char *) &remoteServAddr.sin_addr.s_addr,h->h_addr_list[0], h->h_length);
//  remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);

  /* socket creation */
  sd = socket(AF_INET,SOCK_DGRAM,0);
  if(sd<0) {
    printf("%s: cannot open socket \n",argv[0]);
    exit(1);
  }
/*
  // bind any port
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  cliAddr.sin_port = htons(0);

  rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
  if(rc<0) {
    printf("%s: cannot bind port\n", argv[0]);
    exit(1);
  }
*/
/*
  servidor.sin_addr.s_addr = inet_addr(argv[1]);
  servidor.sin_family = AF_INET;
  servidor.sin_port = htons(REMOTE_SERVER_PORT);
  //Conectando no servidor remoto
  if (connect(sd, (struct sockaddr *) &servidor, sizeof (servidor)) < 0) {
    printf("Nao foi possivel conectar\n");
    return -1;
  }
*/

  memset(&remoteServAddr, 0, sizeof(remoteServAddr));
  remoteServAddr.sin_family = AF_INET;
  remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);
  remoteServAddr.sin_addr.s_addr = inet_addr(argv[1]);
//  remoteServAddr.sin_addr.s_addr = INADDR_ANY;

  /* send data */
  char msg[MAX_MSG];
  while (1) {
    strcpy(msg, "");
    printf("Digite a mensagem: ");
        scanf("%255[^\n]", msg);
    rc = sendto(sd, msg, strlen(msg), 0, (struct sockaddr *)&remoteServAddr, sizeof(remoteServAddr));
//   rc = send(sd, msg, strlen(msg)+1, 0);
    clear_stdin_buffer();

    if(rc<0) {
      printf("%s: cannot send data %d \n",argv[0],i-1);
      close(sd);
      exit(1);
    }

  }

  return 1;

}
