/*************************************************************************
    > File Name: ser.c
    > Created Time: Sat 17 Nov 2018 07:45:58 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/un.h>

int main()
{

	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	assert(sockfd != -1);
	
	struct sockaddr_in ser,cli;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6500);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int err = bind(sockfd,(struct sockaddr*)&ser,sizeof(ser));
	assert(err != -1);

	while(1)
	{
		char buff[128] = {0};
		int len = sizeof(cli);
		err = recvfrom(sockfd, buff, 127, 0, (struct sockaddr*)&cli, &len);
		if(err <= 0)
		{
			continue;
		}
		printf("%s\n",buff);
		
		err = sendto(sockfd, "ok", 2, 0, (struct sockaddr*)&cli, sizeof(cli));
		assert(err != -1);
	}
	close(sockfd);
	return 0;
}




