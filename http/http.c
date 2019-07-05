/*************************************************************************
    > File Name: http.c
    > Created Time: Fri 23 Nov 2018 10:53:22 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<fcntl.h>

void SendError(int c)
{
	char buff[1024] = {0};
	strcpy(buff, "HTTP/1.1 404 NOT FOUND\n\r");
	strcat(buff, "Server:myhttp/1.0\n\r");
	strcat(buff, "Content-Length; ");
	sprintf(buff+strlen(buff), "%d", 0);
	strcat(buff, "\n\r");
	strcat(buff, "Content-Type:text/html;charset=utf-8\n\r");
	strcat(buff, "\n\r");
	strcat(buff, "404 NOT FOUND");
	send(c, buff, strlen(buff), 0);
}

void SendHead(int size, int c)
{
	char buff[1024] = {0};
	strcpy(buff, "HTTP/1.1 200 OK\n\r");
	strcat(buff, "Server:myhttp/1.0\n\r");
	strcat(buff, "Content-Length; ");
	sprintf(buff+strlen(buff), "%d", size);
	strcat(buff, "\n\r");
	strcat(buff, "Content-Type:text/html;charset=utf-8\n\r");
	strcat(buff, "\n\r");
	send(c, buff, strlen(buff), 0);

}

void AnayRequest(char *buff, char *pathname)
{
	char *p = strtok(buff, " ");
	p = strtok(NULL, " ");
	strcpy(pathname, "/var/www/html");
	strcat(pathname, p);
}

void SendData(char *pathname, int c)
{
	struct stat st;
	if(-1 != stat(pathname, &st))
	{
		SendHead(st.st_size, c);
	}
	else
	{
		SendError(c);
		return;
	}
	
	int fd = open(pathname, O_RDONLY);
	if(fd == -1)
	{
		SendError(c);
		return;
	}

	while(1)
	{
		char buff[128] = {0};
		int n = read(fd, buff, 127);
		if(n <= 0)
		{
			close(fd);
			break;
		}
		send(c, buff, strlen(buff), 0);
	}
}

int main()
{

	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(sockfd != -1);

	struct sockaddr_in ser,cli;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(80);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int err = bind(sockfd, (struct sockaddr*)&ser, sizeof(ser));
	assert(err == 0);

	err = listen(sockfd, 5);
	assert(err != -1);

	while(1)
	{
		int len = sizeof(cli);
		int c = accept(sockfd, (struct sockaddr*)&cli, &len);
		if(c < 0)
		{
			continue;
		}
		char buff[1024] = {0};
		err = recv(c, buff, 1023, 0);
		if(err <= 0)
		{
			close(c);
			continue;
		}
		printf("%s\n", buff);

		char pathname[128] = {0};
		AnayRequest(buff, pathname);
		SendData(pathname, c);
		close(c);

	}
	return 0;
}





