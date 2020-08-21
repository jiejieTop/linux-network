/*
 * @Author: jiejie
 * @Date: 2020-08-13 16:08:36
 * @LastEditors: jiejie
 * @LastEditTime: 2020-08-19 10:03:28
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
    // if(argc != 3)
    // {
    //     printf("Usage:%s ip port\n",argv[0]); 
    //     return 1;
    // }

    //1. 创建套接字
    int sock = socket(AF_INET, SOCK_DGRAM , 0 );// SOCK_DGRAM 表示UDP
    if(sock < 0)
    {
        perror("socket");
        return 2;
    }

    printf("sock:%d\n",sock);

  //2. 命名套接字

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi("8000"));//端口号是2位的数
    server.sin_addr.s_addr = inet_addr("192.168.1.169");

    //3. 客户端尽量不绑定端口号
    char buf[1024];//缓冲区

    //4. 客户端发送信息

    while(1)
    {
        printf("Please Enter# ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf)-1);//-1是为了去掉‘/0’
        if( s > 0 )
        {
            buf[s-1] = 0;
            sendto( sock, buf, strlen(buf) , 0 , (struct sockaddr*)&server, sizeof(server));
            // ssize_t _s = recvfrom(sock,buf,sizeof(buf)-1,0,NULL,NULL);
            // if(_s > 0)
            // {
            //     buf[_s] = 0;
            //     printf("server echo# %s\n",buf);
            // }
        }
    }
	
	close(sock);
	return 0;
}
 
 
 
 
 
 
 
 
 