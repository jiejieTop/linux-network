/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-11-14 00:54:15
 * @LastEditTime: 2019-11-25 23:50:42
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define HOST "jiedev.com"
//#define HOST "192.168.1.102"
#define PORT 80
#define MAX_DATA (10*1024)


int main(void)
{
    int sockfd, ret;
    struct hostent *he;
    struct in_addr addr;
    struct sockaddr_in server;
    char addr_str[INET_ADDRSTRLEN];
    char recv_data[MAX_DATA];

    ret = inet_pton(AF_INET, HOST, &addr);
    if (ret == 0) {
        printf("get %s ip addr...\n", HOST);
        if ((he = gethostbyname(HOST)) == NULL) {
            printf("get host ip addr error.\n");
            exit(EXIT_FAILURE);
        } else {
            addr = *((struct in_addr *)he->h_addr);
            if(inet_ntop(AF_INET, &addr, addr_str, sizeof(addr_str)) != NULL)
                printf("host name: %s, ip addr:%s\n",HOST, addr_str);
        }
    } else if(ret = 1) {
        if(inet_ntop(AF_INET, &addr, addr_str, sizeof(addr_str)) != NULL)
            printf("host addr: %s\n", addr_str);
    } else {
        printf("inet_pton function return %d\n", ret);
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
        printf("connect server fail...\n");
        close(sockfd);
        exit(1);
    } 

    printf("connect server success...\n");

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

