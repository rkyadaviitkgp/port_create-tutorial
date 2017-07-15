#include "unp.h"
void main(int argc, char **argv){
  
  struct sockaddr_in sadd;
  int  sockfd, n;
  char recbuffer[256];
  char buff[100];
  int r;
  int read_again = 1; 
  if(argc != 2)
  {
    printf("a.out <IP address>\n");
  }
  
  sockfd = Socket(AF_INET, SOCK_STREAM, 0); 
  bzero(&sadd, sizeof(sadd));
  sadd.sin_family = AF_INET;
  sadd.sin_port = htons(1193);
  if(inet_pton(AF_INET, argv[1], &sadd.sin_addr) < 0)
    printf("inet_pton error \n");
  
  if(connect(sockfd, (struct sockaddr *)&sadd, sizeof(sadd)) < 0)
    printf("connect error \n");
  //sleep(5);
  do{
      printf("writing hi server \n ");
      r = snprintf(buff, sizeof(buff),"hi server");
      if(r<0 || r>9)
      {
        printf("error occured in snprintf \n ");
      }
      Send(sockfd, buff, r, 0);
      Receive(sockfd, recbuffer, 256, 0);
      /*while((n = read(sockfd, recbuffer,256)) > 0){
        recbuffer[n] = 0;
      if(fputs(recbuffer, stdout) == EOF)
       printf("fputs error \n ");
     }*/
  printf("enetr 1 if you want to send again \n ");
  scanf("%d", &read_again);
  }while(read_again == 1);

  if(n < 0)
  {
    printf("read error \n ");
  }
  close(sockfd);
  exit(0);
}
