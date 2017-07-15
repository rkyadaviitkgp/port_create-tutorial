#include "unp.h"
#include <errno.h>
//#define MAX 10
int main(int argc, char **argv)
{
  struct sockaddr_in saddr;
  int sockfd,lsnrfd;
  char buff[256];
  time_t ticks;
  int j;

  struct pollfd fds;

  /* binding and listening */
  lsnrfd = Socket(PF_INET, SOCK_STREAM, 0);
  bzero(&saddr, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(1193);
  if(inet_pton(AF_INET, argv[1], &saddr.sin_addr) < 0)
    printf("inet_pton error \n");
  Bind(lsnrfd, (struct sockaddr* )&saddr, sizeof(saddr));
  Listen(lsnrfd, 10);
  /* Binding and listening is complete */
  printf("listen is complete \n");
  /* call here whichever function you want to call */
  serve_client_port(lsnrfd);
}
