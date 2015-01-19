#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXSIZE 1024
#define MAXPTHREAD 10
#define Input "../Input/1G.img"
#define Output "../Output/1G.img"

typedef struct ThreadArg
{
	int start;
	int end;
	int lable;
	int fd;
	pthread_t pid;
}ThreadArg;

void receive(ThreadArg *arg)
{
	int start=arg->start;
        int end=arg->end;
        int length=end-start;
        int buflen=MAXSIZE;
	int fd=arg->fd;
        int count=length/buflen;
        int extra=length-buflen*count;
	printf("sockfd==%d\npid==%lu\nlength==%d\ncount==%d\nextra==%d\n\n",fd,arg->pid,length,count,extra);
	int outfd=open(Output,O_CREAT|O_WRONLY,0644);
	if(outfd==-1)
	{
		printf("error while open file!\n");
		return;
	}
	lseek(outfd,start,SEEK_SET);
	int i;
	char buf[MAXSIZE];
	int nrecv;
	for(i=1;i<=count;i++)
	{
		nrecv=recv(fd,buf,buflen,0);
		write(outfd,buf,nrecv);
        }
	memset(buf,0,buflen);
	if(extra!=0)
	{
		nrecv=recv(fd,buf,buflen,0);
		write(outfd,buf,nrecv);
	}
	close(outfd);
	close(fd);
	pthread_exit(NULL);
}

int client_init() 
{
	struct sockaddr_in server;
	int sockfd;
	if(-1==(sockfd=socket(AF_INET,SOCK_STREAM,0)))
	{
		printf("Create socket failed.\n");
		exit(1);
	}
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(8000);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(-1 == (connect(sockfd,(struct sockaddr *)&server,sizeof(server))))
	{
		printf("Connect failed.\n");
		close(sockfd);
		exit(1);
	}
	return sockfd;
}

void main()
{
	time_t t_start,t_end;	
	int infd=open(Input,O_RDONLY);
	if(infd==-1)
	{
		printf("error while open file!\n");
		return;
	}
	struct stat st;
	fstat(infd,&st);
	close(infd);
	int fd_size=st.st_size;
	int thread_size=MAXPTHREAD;
	int percent=fd_size/thread_size;
	int i;
	ThreadArg threadarg[thread_size+1];
	printf("begin timing......\n");
	t_start=time(NULL);
	printf("ready for socket server......\n");
	for(i=1;i<=thread_size;i++)
	{
		threadarg[i].lable=i;
		threadarg[i].fd=client_init();
		threadarg[i].start=(i-1)*percent;
		threadarg[i].end=i*percent;
	}  
	threadarg[--i].end=fd_size;
	for(i=1;i<=thread_size;i++)
	{
		pthread_create(&(threadarg[i].pid),NULL,(void *)receive,&threadarg[i]);
	}
	for(i=1;i<=thread_size;i++)
	{
		pthread_join(threadarg[i].pid,NULL);
	}
	printf("receiving succeed!\n");
	printf("timing end!\n");
	t_end=time(NULL);
	printf("time of transmission:%.0fs\n",difftime(t_end,t_start));
}
