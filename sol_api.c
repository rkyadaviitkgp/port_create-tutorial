#include "unp.h"
/* socket related API */

int Socket(int family, int type, int protocol)
{
  int sockfd;
  if((sockfd = socket(family, type, protocol)) < 0)
   printf("socket err");
  return sockfd;
}

int Bind(int sockfd, struct sockaddr *sa, int size)
{
  int fd ;
  if((fd = bind(sockfd, sa, size)) < 0)
    printf("bind error \n");
  return fd;
}

void Listen(int sockfd, int no)
{
  if((listen(sockfd, no)) < 0)
  {
    printf("listen error \n");
  }
}

int Accept(int sockfd,  struct sockaddr *sa, int *size)
{
  int fd ;
  if((fd = accept(sockfd, sa, size)) < 0)
    printf("Accept error \n ");
  return fd;
}

int Write(int sockfd, char buff[], int len)
{
  printf("Write : writing to socket \n");
  write(sockfd, buff, len);
  printf("Write exite(); \n ");
}

int Read(int sockfd, char recbuffer[], int len)
{
  int n;
  printf("Inside Read \n");
  //int n;
  while((n = read(sockfd, recbuffer,256)) > 0){
   recbuffer[n] = 0;
   if(fputs(recbuffer, stdout) == EOF)
     printf("fputs error \n ");
  }
  printf("Read exit \n ");
  return n;
}

int Receive(int socket, char str[], int len , int flags){
  int n;
  printf("Inside Receive : \n");
  n = recv(socket, str, len, flags);
  str[n] = 0;
  printf("Data Received is  %s \n", str);
  printf("Exit Receive : \n");
  return n;
}

int Send(int socket, char str[], int len , int flags){
  printf("Inside Send function  \n");
  str[len] = 0;
  printf("Data Send is %s \n", str);
  send(socket, str, len, flags);
  printf("Exit Send function \n");
}

/* end of socket related API */

//#ifdef SUN_OS5

/* dev/poll related API */

int Open(){ /* open dev/poll descriptor */
  int fd ;  
  printf("Opening dev/poll descriptor \n");
  fd = open("/dev/poll", O_RDWR);
  if(fd < 0)
  {
    printf("error occured while opening dev/poll descriptor \n");
    exit(0);
  }
  printf("exit open(dev/poll) function \n");
  return fd;
}

int Write_devpoll(int fd, struct pollfd buff[], int buffsize)
{  /* write set of fd to dev/poll fd */
  
  int ret;
  printf("entering write_devpoll function ");
  ret = write(fd, buff, buffsize);
  if(ret < 0)
  {
    printf("error occured in writing to dev/poll fd ");
    exit(0);
  }
  printf("exiting write_devpoll function \n ");
  return ret;
  
}

int Ioctl(int fd, struct dvpoll *arg)
{ /* polling for event on dev poll fd */
  int n;
  printf("Polling dev poll for events \n");
  n = ioctl(fd, DP_POLL, arg);
  if(n < 0)
  {
    printf("error occured while polling dev poll\n");
    exit(-1);
  }
  return n;
}

int check_portfd_pollable(int port_fd) /* This function checks whether port fd itself is pollable or not */
{
  struct pollfd fds;
  int rfd;
  fds.fd = port_fd;
  fds.events = 0;
  fds.events = fds.events | POLLIN; /* polling for read event */
  for(;;) /* listening on listener fd or processing alredy connedted fd */
  {
     poll(&fds, 1, 10000000);
     rfd = fds.fd;
     if(rfd==port_fd)
     {
        printf("port_fd is pollable \n");
        return 1;
     }
     printf("pollfd is not pollable \n");
     return 0;
  }
  
}
/* end of dev/poll API */
//#endif

//#ifdef SUN_OS5

/* port_create related API */

void AssociateN(int fd, int arr[], int len){
	int i;
        int events = 0;
 	void *mypointer;
	events = events | POLLIN;
	for(i=0; i<len; i++)
 	{
		printf("Registering file descriptor %d\n", arr[i]);
		port_associate(fd, PORT_SOURCE_FD, arr[i], events, mypointer);
	}
}

void Associate(int pfd, int fd)
{
	int i;
        int events = 0;
        void *mypointer;
        events = events | POLLIN;
        printf("Registering file descriptor %d\n", fd);
        port_associate(pfd, PORT_SOURCE_FD, fd, events, mypointer);
}

int Port_get(int port_fd, port_event_t *pe, timespec_t *timeout)
{
  int ret;
  printf("Calling port_get function \n ");
  if((ret = port_get(port_fd, pe, timeout)) < 0)
  {
    printf("error ocuured in port_get \n");
  }
  printf("Port_get exit()");
  return ret;
}

int Port_getn(int port_fd, port_event_t buff[], int max, int *nget, timespec_t *time)
{
  int ret ;
  printf("Calling port_getn function \n");
  if((ret = port_getn(port_fd, buff, max, nget, time)) < 0)
  {
    printf("error occured in port_getn function \n");
    printf("error :  %s\n", strerror(errno)); 
  }
  printf("port_getn exit()\n");
  return ret;
}
void serve_client_port(int lsnrfd) /* Sample function which will receive client request and reply client with hi message (server uses port create fd for handling event) */
{
  int port_fd;
  int events = 0;
  int cfd[100];
  char buff[256];
  void *mypointer;
  port_event_t pe[MAX];
  timespec_t timeout;
  int i=0, nget, rfd, ret, j;
  int byte_received;
  struct sockaddr cli_addr;
  int sz; 
   
  printf("inside serve client port\n"); 
  /* creating port fd and associating listener fd to this */
  port_fd = port_create();
  events = events | POLLIN;
  Associate(port_fd, lsnrfd);
  /* Association is complete */

  /* We can have timeout value */
  bzero(&cli_addr, sizeof(cli_addr)); 
  sz = sizeof(cli_addr); 
  timeout.tv_sec =10;
  timeout.tv_nsec = 0;

   for(;;) /* listening on listener fd or processing alredy connedted fd */
  {
    /* if you want to chek whether port_fd is pollable or not the include below line */
   if( check_portfd_pollable(port_fd) <= 0)       
    {
      exit_close(ret, cfd, i, lsnrfd);
    }

    nget = 1;
    ret = Port_getn(port_fd, pe, MAX, &nget, &timeout);

    if(ret == -1)
    {
      exit_close(ret, cfd, i, lsnrfd);
    }
    for( j=0; j<nget; j++)
    {
      rfd = pe[j].portev_object;
      if(rfd == lsnrfd) /* Is it a new connection ? */
      {
        printf(" Accepting next connection \n");
        cfd[i] = Accept(lsnrfd, (struct sockaddr* ) &cli_addr, &sz);
        Associate(port_fd, cfd[i]);
        Associate(port_fd, lsnrfd);
        i++;
      }
      else /* no it is a already connected client */
      {
        printf("Processing already connecteded clients \n");
        byte_received = Receive(rfd, buff, 256, 0);
        if(byte_received != 0) /* Socket is not closed at other side */
        {
          snprintf(buff, sizeof(buff),"hi");
          Send(rfd, buff, 2, 0);
          Associate(port_fd, rfd);
        }
        else /* This is case of Socket closing at client side  */
        {
          close(rfd);
        }
      }
    }
  }
  close(port_fd);
  exit_close(-1, cfd, i, lsnrfd);
}
/* end of port_create related API */

//#endif

/* Common API */

int exit_close(int ret, int arr[], int len, int lsnrfd){
  int i;
  printf("Error has occcured\n");
  if(errno == ETIME) /* Is it a timeout case ? */
  {
    printf("error :  %s\n", strerror(errno));
    //errno = 0;
  }
  else
  {
    printf("We got error close all open fd plzz \n ");
    for(i=0; i<len; i++)
      close(arr[i]);
    close(lsnrfd);
    exit(0);
  }
}

/* end of common API */

