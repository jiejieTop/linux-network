/*
 * @Author: jiejie
 * @Date: 2020-08-13 16:08:36
 * @LastEditors: jiejie
 * @LastEditTime: 2020-08-13 16:18:18
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
#include <arpa/inet.h>

#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
 
 
#define SER_PORT  (8000)
#define CLI_PORT  (9000)
 
#define GROUP_IP ("239.0.0.2")
#define LOCAL_IP ("0.0.0.0")
 
 
int main(void)
{
	int sockfd = -1;
	struct sockaddr_in cli_addr;
	struct ip_mreq group_addr;
	char recv_buf[1024] = {0};
	int recv_len = -1;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	/*第一步，绑定本地IP地址结构*/
	memset(&cli_addr,0,sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(CLI_PORT);
	inet_pton(AF_INET,LOCAL_IP,&cli_addr.sin_addr.s_addr);
	
	bind(sockfd, (const struct sockaddr *)&cli_addr,sizeof(cli_addr));
	
	/*第二步，设置组播地址并将client加入组播组*/
	memset(&group_addr,0,sizeof(group_addr));
	inet_pton(AF_INET,GROUP_IP,&group_addr.imr_multiaddr); //设置组播地址
	inet_pton(AF_INET,LOCAL_IP,&group_addr.imr_interface); //设置本地IP地址
											
	setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group_addr, sizeof(group_addr)); // 加入组包组
																					   //IP_ADD_MEMBERSHIP:加入组播组的命令
																					  	
	/*第三步，接受组播数据*/
	while(1)
	{
		memset(recv_buf,0,1024);
		recv_len = recvfrom(sockfd, recv_buf, 1024, 0,NULL, NULL); 
		
		if(recv_len > 0)
		{
			printf("recv_len = %d, recv_buf:%s\n",recv_len,recv_buf);
		}	
		
	}
	close(sockfd);
	return 0;
}
 
 
 
 
 
 
 
 
 