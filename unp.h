#ifndef unp
#define unp
# define MAX 10 
#include<stdio.h>
#include <port.h>
#include<stdlib.h>
#include <strings.h>
#include <sys/socket.h> /* socket and other defination */
#include <netinet/in.h> /* sockaddr_in and other inetrnet def */
#include <fcntl.h>
# include <sys/devpoll.h>
# include <errno.h>
int Socket(int, int, int);
#endif //#ifndef unp
