/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-11-22 19:16:13
 * @LastEditTime: 2019-12-07 17:48:00
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 10*1024
#define PORT 50000
#define	LISTENQ		1024

void process_client(int sockfd, char *buff, int len);

// Driver function
int main()
{
    char buff[MAX];
    int nready, client_num[FD_SETSIZE];
    int sockfd, listenfd, connfd, maxfd, len, i, maxi;
	fd_set rset, allset;
    struct sockaddr_in server, client;

    // socket create and verification
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        printf("socket creation failed...\n");
        exit(1);
    }

    printf("socket successfully created..\n");
    bzero(&server, sizeof(server));

    // assign IP, PORT
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // binding newly created socket to given IP and verification
    if ((bind(listenfd, (struct sockaddr*)&server, sizeof(server))) != 0) {
        printf("socket bind failed...\n");
        exit(1);
    }
    
    printf("socket successfully binded..\n");

    // now server is ready to listen and verification
    if ((listen(listenfd, LISTENQ)) != 0) {
        printf("Listen failed...\n");
        exit(1);
    }

    printf("server listening...\n");
    
    maxfd = listenfd;
	for (i = 0; i < FD_SETSIZE; i++) 
		client_num[i] = -1;			/* -1 indicates available entry */

	FD_ZERO(&allset); 
	FD_SET(listenfd, &allset); 

    while (1)
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        
        printf("nready = %d\n", nready);

        if(nready <= 0)
        {
            printf("not find client connent...\n");
            continue;
        }

        if (FD_ISSET(listenfd, &rset)) {

            printf("is a listenfd\n");

            len = sizeof(client);
            // accept the data packet from client and verification
            connfd = accept(listenfd, (struct sockaddr*)&client, &len);
            if (connfd < 0) {
                printf("server acccept failed...\n");
                continue;
            }

			for (i = 0; i < FD_SETSIZE; i++) {
                if (client_num[i] < 0) {
					client_num[i] = connfd;	
					break; 
				}
            }
            if (i == FD_SETSIZE) {
                printf("too many clients...\n");
                exit(1);
            }
            
            FD_SET(connfd, &allset);
            if (connfd > maxfd)
                maxfd = connfd;
            if(i > maxi)
                maxi = i;
			// if(--nready <= 0)
            //     continue;
        }



        for(i = 0; i <= maxi; i++) {
            if((sockfd = client_num[i]) < 0) {
                continue;
            }

            printf("client_num[%d] = %d\n", i, client_num[i]);

            if(FD_ISSET(sockfd, &rset)) {
                // read the messtruct sockaddrge from client and copy it in buffer
                if ((len = read(sockfd, buff, sizeof(buff))) <= 0) {
                    printf("client close...\n");
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client_num[i] = -1;
                } else {
                    printf("len is %d\n", len);
                    process_client(sockfd, buff, len);
                }

                if(--nready <= 0)
                    break;
            }
        }
    }
    
    close(listenfd);
    exit(0);
}

void process_client(int sockfd, char *buff, int len)  
{  
    // print buffer which contains the client contents
    printf("from client data: %s, len: %d\n", buff, len);

    // and send that buffer to client
    write(sockfd, buff, len);
    
    bzero(buff, len);
}  