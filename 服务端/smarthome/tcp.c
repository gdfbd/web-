#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define MAX_BACKLOG	1024

int tcp_server_init()  //服务器初始化函数
{
	int opt = 1;
	//1.socket
	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd < 0)
	{
		printf("create socket failed!\n");
		return -1;
	}
	printf("create socket ok!\n");
	//端口重用，这句话不要动
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	//2.bind
	struct sockaddr_in stServer;
	memset(&stServer,0,sizeof(stServer));
	stServer.sin_family = AF_INET;
	stServer.sin_port = htons(8888);
	stServer.sin_addr.s_addr = inet_addr("192.168.15.3");
	
	int ret = bind(listenfd,(struct sockaddr *)&stServer,sizeof(stServer));
	if(ret<0)
	{
		printf("bind failed!\n");
		return -1;
	}
	printf("bind ok!\n");
	//3.listen
	ret = listen(listenfd,5);
	if(ret<0)
	{
		printf("listen failed!\n");
		return -1;
	}
	printf("listen ok!\n");
	//返回socket描述符
	return listenfd;
}

int tcp_server_wait_connect(int listenfd)  //参数是socket描述符，accept第一个参数要用
{
	//accept   
	struct sockaddr_in stClient;
	memset(&stClient,0,sizeof(stClient));
	socklen_t len = sizeof(stClient);
	int iClient = accept(listenfd,(struct sockaddr *)&stClient,&len);
	if(iClient < 0)
	{
		printf("accept failed!\n");
		return -1;
	}
	//返回连接成功后的描述符
	printf("连接成功\n");
	return iClient;
}

int tcp_server_disconnect(int connfd)
{
	if (close(connfd)) {
		perror("server->close");
		return -1;
	}
	return 0;
}

int tcp_server_exit(int listenfd)
{
	if (close(listenfd)) {
		perror("server->close");
		return -1;
	}
	return 0;
}










