/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-11-14 00:54:15
 * @LastEditTime: 2019-11-20 02:11:19
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

//#define HOST "jiedev.com"
#define HOST "192.168.0.3"
#define PORT 5002
#define MAX_DATA (10*1024)


int main(void)
{
    int sockfd;
    struct hostent *he;
    struct in_addr addr;
    struct sockaddr_in server;
    char recv_data[MAX_DATA];

    if (inet_aton(HOST, &addr) == 0) {
        printf("get %s ip addr...\n", HOST);
        if ((he = gethostbyname(HOST)) == NULL) {
            printf("get host ip addr error.\n");
            exit(EXIT_FAILURE);
        } else {
            addr = *((struct in_addr *)he->h_addr); 
            printf("host name: %s, ip addr:%s\n",HOST, inet_ntoa(addr));
        }
    } else {
        printf("host addr:%s\n",inet_ntoa(addr));
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create an endpoint for communication fail!\n");
        exit(1);
    } 

    bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr = addr;

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
        printf("connect server fail\n");
        close(sockfd);
        exit(1);
    } 

    printf("connect server success\n");

    while (1) {

        if (recv(sockfd, recv_data, MAX_DATA, 0)) {
            printf("server message: %s.\n", recv_data);
            memset(recv_data,0,MAX_DATA); 
        } else {
            printf("recv data fail!\n");
            close(sockfd);
            exit(1);
        }
    }

    close(sockfd);
    exit(0);
}

