/*
 * @Author: jiejie
 * @Date: 2020-08-13 16:08:19
 * @LastEditors: jiejie
 * @LastEditTime: 2020-08-18 10:48:58
 * @Description: the code belongs to huituo, confidentiality is required, disclosure is prohibited, please keep the author information and source code according to the license.
 */
#include <stdio.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
 
#define SER_PORT  (8000)
#define CLI_PORT  (9000)
 
#define GROUP_IP ("239.0.0.2")
#define LOCAL_IP ("0.0.0.0")
 
 
int main(void)
{
	int sockfd = -1;
	struct sockaddr_in ser_addr,cli_addr;
	struct ip_mreq group_addr;
	char muticast_buf[1024] = {"muti caset data"};
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	/*第一步，绑定Server端的IP地址和端口号*/
	memset(&ser_addr,0,sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(SER_PORT);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(sockfd, (const struct sockaddr *)&ser_addr,sizeof(ser_addr));
 
	/*第二步，设置组播地址并指定发送组播数据*/
	memset(&group_addr,0,sizeof(group_addr));
	inet_pton(AF_INET,GROUP_IP,&group_addr.imr_multiaddr); //设置组播地址
	inet_pton(AF_INET,LOCAL_IP,&group_addr.imr_interface); //设置本地IP地址
											
	setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &group_addr, sizeof(group_addr)); // 制定发送组播数据包
																					  //IP_MULTICAST_IF:指定发送组播数据的命令
																					  
	/*第三步，构造Client端的IP地址和端口号*/
	memset(&cli_addr,0,sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(CLI_PORT); 
	inet_pton(AF_INET,GROUP_IP,&cli_addr.sin_addr.s_addr);	//注意：这里的组播客户端的地址就是组播地址
	
	printf("mutiast config finished ,then send muticast datas\n");
	/*第四步，向制定的组播地址发送数据*/
	while(1)
	{
		//调用sendto来向组播地址发送组播包
		sendto(sockfd, muticast_buf, strlen(muticast_buf), 0,(const struct sockaddr *)&cli_addr, sizeof(cli_addr));
		
		printf("muticast packet send ok\n");
		
		sleep(2);//2s发送一次
	}
	close(sockfd);
	return 0;
}
 
 
 
 