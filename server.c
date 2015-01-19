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

typedef struct ThreadArg
{
    int start;
    int end;
    int lable;
    int fd;
    pthread_t pid;
}ThreadArg;

void sending(ThreadArg *arg) 
{
	int start=arg->start;
	int end=arg->end;
	int length=end-start;
	int buflen=MAXSIZE;
	int fd=arg->fd;
	int count=length/buflen;
	int extra=length-buflen*count;
	printf("sockfd==%d\npid==%lu\nlength==%d\ncount==%d\nextra==%d\n\n",fd,arg->pid,length,count,extra);
	int infd=open(Input,O_RDONLY);
	if(infd==-1)
	{
		printf("error while open file!\n");
		return;
	}
	lseek(infd,start,SEEK_SET);
	int i;
	char buf[MAXSIZE];
	int nread;
	for(i=1;i<=count;i++)
	{
		nread=read(infd,buf,buflen);
		send(fd,buf,nread,0);
	}
	memset(buf,0,buflen);
	if(extra!=0)
	{
		nread=read(infd,buf,extra);
		send(fd,buf,nread,0);
	}
	close(infd);
	close(fd);
	pthread_exit(NULL);
}

void main()
{
	int infd = open(Input,O_RDONLY);
	if(infd == -1)
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
	time_t t_start,t_end;
	ThreadArg threadarg[thread_size+1];
	for(i=1;i<=thread_size;i++)
	{
		threadarg[i].lable=i;
		threadarg[i].start=(i-1)*percent;
		threadarg[i].end=i*percent;
	}
	threadarg[--i].end=fd_size;
	int listenfd;
	struct sockaddr_in servaddr;
	if(-1==(listenfd=socket(AF_INET,SOCK_STREAM,0)))
	{
		printf("Socket failure.Please run again.\n");
		exit(1);
	} 
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servaddr.sin_port=htons(8000);
	if(-1==bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)))
	{
	printf("bind failure.Please run again.\n");
	exit(1);
	}
	printf("begin timing......\n");
	t_start=time(NULL);
	printf("ready for socket server......\n");
	listen(listenfd,MAXPTHREAD);
	for(i=1;i<=thread_size;i++)
	{
		int len;
		len=sizeof(servaddr);
		threadarg[i].fd= accept(listenfd,(struct sockaddr*)&servaddr,&len); 
		pthread_create(&(threadarg[i].pid),NULL,(void *)sending,&threadarg[i]);
	}  
	for(i=1;i<=thread_size;i++)
	{
		pthread_join(threadarg[i].pid,NULL);
	}
	printf("sneding succeed!\n");
	printf("timing end!\n");
	t_end=time(NULL);
	printf("time of transmission:%.0fs\n",difftime(t_end,t_start));
	close(listenfd);
}
