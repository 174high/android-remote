/* Receiver/client multicast Datagram example. */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>


struct sockaddr_in localSock;
struct ip_mreq group;
int sd;
int datalen;
char databuf[1024];
#define MAXDATASIZE 1024

int main(int argc, char *argv[])
{
	int sockfd,sendbytes;
	char buf[MAXDATASIZE]={0};
	struct hostent *host;
	struct sockaddr_in serv_addr;


	/* Create a datagram socket on which to receive. */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0)
	{
		perror("Opening datagram socket error");
		exit(1);
	}
	else
	printf("Opening datagram socket....OK.\n");
	 
	/* Enable SO_REUSEADDR to allow multiple instances of this */
	/* application to receive copies of the multicast datagrams. */
	{
		int reuse = 1;
		if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
		{
			perror("Setting SO_REUSEADDR error");
			close(sd);
			exit(1);
		}
		else
		printf("Setting SO_REUSEADDR...OK.\n");
	}
	 
	/* Bind to the proper port number with the IP address */
	/* specified as INADDR_ANY. */
	memset((char *) &localSock, 0, sizeof(localSock));
	localSock.sin_family = AF_INET;
	localSock.sin_port = htons(1900);
	localSock.sin_addr.s_addr = INADDR_ANY;
	if(bind(sd, (struct sockaddr*)&localSock, sizeof(localSock)))
	{
		perror("Binding datagram socket error");
		close(sd);
		exit(1);
	}
	else
	printf("Binding datagram socket...OK.\n");
	 
	/* Join the multicast group 226.1.1.1 on the local 203.106.93.94 */
	/* interface. Note that this IP_ADD_MEMBERSHIP option must be */
	/* called for each local interface over which the multicast */
	/* datagrams are to be received. */
	group.imr_multiaddr.s_addr = inet_addr("239.0.0.1");
	group.imr_interface.s_addr = inet_addr("192.168.100.120");
	if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
	{
		perror("Adding multicast group error");
		close(sd);
		exit(1);
	}
	else
	printf("Adding multicast group...OK.\n");
	 
	/* Read from the socket. */
	datalen = sizeof(databuf);
	if(read(sd, databuf, datalen) < 0)
	{
		perror("Reading datagram message error");
		close(sd);
		exit(1);
	}
	else
	{
		printf("Reading datagram message...OK.\n");
		printf("The message from multicast server is: %s\n", databuf);
		printf("The message from multicast server is: %s\n", databuf+50);
		printf("The message from multicast server is: %s\n", databuf+100);
		printf("The message from multicast server is: %s\n", databuf+150);
		printf("The message from multicast server is: %s\n", databuf+200);
		//argv[1]=databuf+203;

		if((host=gethostbyname("192.168.100.120"))==NULL)
		{
			perror("gethostbyname");
			exit(1);
	    	}

		if((sockfd=socket(AF_INET,SOCK_STREAM,0))== -1){
			perror("socket");
			exit(1);
		}

		serv_addr.sin_family=AF_INET;
		serv_addr.sin_port=htons(60001);
		serv_addr.sin_addr=*((struct in_addr *)host->h_addr);
		bzero(&(serv_addr.sin_zero),8);

		if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))== -1)
		{
			perror("connect");
			exit(1);
		}

		while(1)
		{
			gets(buf);

			if(send(sockfd,buf,strlen(buf),0)== -1)
			{
				perror("send");
				exit(1);
			}

			if((recv(sockfd,buf,100,0))== -1)
			{
				perror("recv");
				exit(1);
			}
			printf("received a connection :%s\n",buf);
		}
	}
	return 0;
}




















