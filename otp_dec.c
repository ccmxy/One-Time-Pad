//OTP_DEC.C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
	int sockfd, port;
	struct sockaddr_in serv_addr;
	char buffer[BUFSIZE];
	ssize_t nread, nwritten;
	int textLength, keyLength;
	char textContents[BUFSIZE];
	int fd;
	char keyContents[BUFSIZE];
	char fileName[BUFSIZE];

	//open Ciphertext and get it into textContents
	snprintf(fileName, 512, "%s", argv[1]);
	fd = open(fileName, O_RDWR);
	nread = read(fd, textContents, 512);
	textLength = nread;
	close(fd);
	textContents[strlen(textContents) + 1] = '\0';//Add null terminating character to end
	//open key and get it into keyContents
	memset(fileName, 0, strlen(fileName));
	memset(keyContents, 0, strlen(keyContents));
	snprintf(fileName, 512, "%s", argv[2]);
	fd = open(fileName, O_RDWR);
	nread = read(fd, keyContents, 512);
	close(fd);
	keyLength = nread;
	if (keyLength < textLength){
		perror("Key too short!");
	}

	//Get port number (service endpoint) into port:
	port = atoi(argv[3]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);// Set socket file descriptor for IP, TCP

	if (sockfd < 0) {
		error("ERROR opening socket.\n");
		exit(1);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	//Set the address family for socket as TCP/IP:
	serv_addr.sin_family = AF_INET;
	//Set the port number to TCP / IP mode:
	serv_addr.sin_port = htons(port);
	//Accept any incoming connection:
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		perror("otp_dec could not find otp_dec_d.\n");
		exit(2);
	}
	snprintf(buffer, BUFSIZE, "%s", textContents);
	nwritten = write(sockfd, buffer, strlen(buffer));
	if (nwritten == -1) {
		perror("Could not write to socket");
		exit(1);
	}

	snprintf(buffer, BUFSIZE, "%s", keyContents);
	nwritten = write(sockfd, buffer, strlen(buffer));
	if (nwritten == -1) {
		perror("ERROR writing to socket.\n");
		exit(1);
	}

	bzero(buffer, BUFSIZE);
	nread = read(sockfd, buffer, BUFSIZE);
	if (nread == -1) {
		perror("ERROR reading from socket.\n");
		exit(1);
	}

	printf("%s\n", buffer); //Output decrtyped text
	close(sockfd);

	return 0;
}
