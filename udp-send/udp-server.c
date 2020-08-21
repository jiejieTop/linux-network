/*
 * @Author: jiejie
 * @Date: 2020-08-17 15:08:43
 * @LastEditors: jiejie
 * @LastEditTime: 2020-08-17 15:09:00
 * @Description: the code belongs to huituo, confidentiality is required, disclosure is prohibited, please keep the author information and source code according to the license.
 */
#include <stdio.h>                                                                                                                                                
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("Usage:%s ip port\n",argv[0]);
        return 1;
    }

    //1. 创建套接字

    int sock = socket(AF_INET, SOCK_DGRAM , 0 );// SOCK_DGRAM 表示UDP
    if(sock < 0)
    {
        perror("socket");
        return 2;
    }
    printf("Socker:%d\n",sock);


    //2. 命名套接字

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[2]));//端口号是2位的数
    local.sin_addr.s_addr = inet_addr(argv[1]);

    //3. 绑定端口号
    if( bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0  )
    {
        perror("bind");
        return 3;
    }

    char buf[1024];//缓冲区

    //4. 服务器接收客户信息

    while(1)
    {
        struct sockaddr_in client;

        socklen_t len = sizeof(client);
        ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0,(struct sockaddr*)&client,&len);//后面两个参数是 输出型参数
        if( s > 0 )//读成功
        {
            buf[s] = 0;
            printf("[%s:%d:%s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);
            sendto( sock, buf, strlen(buf) , 0 , (struct sockaddr*)&client, sizeof(client));
        }
    }

    close(sock);
    return 0;
}
