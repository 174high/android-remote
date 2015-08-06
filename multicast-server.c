#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAXDATASIZE 1024
#define BACKLOG 10
struct in_addr localInterface;
struct sockaddr_in groupSock;
int sd;
char databuf[500] ;
int datalen = sizeof(databuf);

 
int main (int argc, char *argv[ ])
{
	struct sockaddr_in server_sockaddr,client_sockaddr;
	socklen_t sin_size ;
	//socklen_t sin_size= sizeof(struct sockaddr);
	int recvbytes;
	int sockfd,client_fd;
	char buf[MAXDATASIZE]={0};

	/* Create a datagram socket on which to send. */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0)
	{
	  perror("Opening datagram socket error");
	  exit(1);
	}
	else
	  printf("Opening the datagram socket...OK.\n");
	 
	/* Initialize the group sockaddr structure with a */
	/* group address of 225.1.1.1 and port 5555. */
	memset((char *) &groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr("239.0.0.1");
	groupSock.sin_port = htons(1900);
	 
	/* Disable loopback so you do not receive your own datagrams.
	{
	char loopch = 0;
	if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0)
	{
	perror("Setting IP_MULTICAST_LOOP error");
	close(sd);
	exit(1);
	}
	else
	printf("Disabling the loopback...OK.\n");
	}
	*/
	 
	/* Set local interface for outbound multicast datagrams. */
	/* The IP address specified must be associated with a local, */
	/* multicast capable interface. */

	localInterface.s_addr = inet_addr("192.168.100.120");
	if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
	{
	  perror("Setting local interface error");
	  exit(1);
	}
	else
	  printf("Setting the local interface...OK\n");

	/* Send a message to the multicast group specified by the*/
	/* groupSock sockaddr structure. */
	/*int datalen = 1024;*/

	strcpy(databuf,"uuid:9ad490ab-26b7-41b0-ada9-fd7cdf23bda9");
	strcpy(databuf+50,"NT=HIDIP:TouchScreen");
	strcpy(databuf+100,"NTS=ssdp:alive");
	strcpy(databuf+150,"USN=FLC_HIDIP:TS");
	strcpy(databuf+200,"AL=192.168.100.120:60001");

	if(sendto(sd, databuf, datalen, 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0)
	{perror("Sending datagram message error");}
	else
	  printf("Sending datagram message...OK\n");
	 
	//Try the re-read from the socket if the loopback is not disable
	/*if(read(sd, databuf, datalen) < 0)
	{
		perror("Reading datagram message error\n");
		close(sd);
		exit(1);
	}
	else
	{
		printf("Reading datagram message from client...OK\n");
		printf("The message is: %s\n", databuf);
	}
	*/

	if((sockfd = socket(AF_INET,SOCK_STREAM,0))== -1)
	{
		perror("socket");
		exit(1);
	}

	printf("socket success!,sockfd=%d\n",sockfd);

	server_sockaddr.sin_family=AF_INET;
	                               //duankou
	server_sockaddr.sin_port=htons(60001);
	server_sockaddr.sin_addr.s_addr=INADDR_ANY;
	bzero(&(server_sockaddr.sin_zero),8);

	if(bind(sockfd,(struct sockaddr *)&server_sockaddr,sizeof(struct
	sockaddr))== -1){
		perror("bind");
		exit(1);
	}
	printf("bind success!\n");

	if(listen(sockfd,BACKLOG)== -1){
		perror("listen");
		exit(1);
	}

	printf("listening....\n");

	//memset(&client_sockaddr, 0, sizeof(struct sockaddr_in));

	//sin_size = 1;
    	printf("sin_size=%d\n",sin_size);

	if((client_fd=accept(sockfd,(struct sockaddr *)&client_sockaddr,&sin_size))==-1)
	{
		perror("accept");
		exit(1);
	}


	while(1)
	{
		if((recvbytes=recv(client_fd,buf,100,0))== -1)
		{
			perror("recv");
			exit(1);
		}

		printf("received a connection :%s\n",buf);

		if(send(client_fd,buf,strlen(buf),0)== -1)
		{
			perror("send");
			exit(1);
		}

	}


	close(sockfd);

	return 0;
}


