#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define DOMAIN_NAME "www.jiejietop.cn"
#define PORT 80
#define MAX_DATA (14*1024)

int main(void)
{
    int sockfd, num;
    struct hostent *hptr;
    struct sockaddr_in server;
    char recv_data[MAX_DATA];

	if ((hptr = gethostbyname(DOMAIN_NAME)) == NULL) {
		printf("get host ip addr error.\n");
		exit(1);
	} else {
        printf("host name:%s, ip:%s\n",DOMAIN_NAME, inet_ntoa(*((struct in_addr *)hptr->h_addr)));
    }

    if (socket(AF_INET, SOCK_STREAM, 0) == -1) {
        printf("create an endpoint for communication fail!\n");
        exit(1);
    } 
    

    bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr = *((struct in_addr *)hptr->h_addr);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("connect server fail\n");
        exit(1);
    } else {
        printf("connect server success\n");
        
        if ((num = recv(sockfd, recv_data, MAX_DATA, 0) == -1)) {
            printf("recv data fail!\n");
            exit(1);
        }

        recv_data[num] = '\0';
	    printf("server message: %s.\n", recv_data);
	    close(sockfd);
    }

    exit(0);
}