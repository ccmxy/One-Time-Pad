//OTP_ENC.C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <time.h>
#define BUFSIZE 1024

//checkIfValid runs thorough the Ascii
//capital letters and the space char
//to check if char is valid. Returns true(0) if 
//send in char is a valid option, and false(1) if not.
int checkIfValid(char c){
	char valid;
	int idx;
	if (c == ' '){
		return 0;
	}
	//Run through the ascii  alphabet: 
	for (idx = 0; idx < 26; idx++){
		valid = (65 + idx);
		if (c == valid){
			return 0;
		}
	}
	return 1;
}

int main(int argc, char *argv[]) {
	int sockfd, port, n, idx;
	struct sockaddr_in serv_addr;
	char buffer[BUFSIZE];
	char textContents[BUFSIZE];
	char keyContents[BUFSIZE];
	char fileName[BUFSIZE];
	int fd, cx = 0;
	FILE * file;
	socklen_t clilen;
	ssize_t nread, nwritten;
	int textLength, keyLength;

	//Open plaintext and get it into textContents:
	snprintf(fileName, 512, "%s", argv[1]);
	fd = open(fileName, O_RDWR);
	nread = read(fd, textContents, 512);
	textLength = nread;
	close(fd);
	textContents[strlen(textContents) + 1] = '\0';//Add null terminating character to end
	//open key and get it into keyContents
	for (idx = 0; idx < 10; idx++){
		//Check that all the characters are valid:
		cx = checkIfValid(textContents[idx]);
		if (cx == 1){
			fprintf(stderr, "\nERROR: %s contains invalid character at position %d!\n\n", fileName, idx);
			exit(2);
		}
	}
	memset(fileName, 0, strlen(fileName));
	memset(keyContents, 0, strlen(keyContents));
	snprintf(fileName, 512, "%s", argv[2]);
	fd = open(fileName, O_RDWR);
	nread = read(fd, keyContents, 512);
	close(fd);
	keyLength = nread;
	if (keyLength < textLength){
		perror("Key too short!");
		exit(1);
	}
	//Get port number (the service endpoint) into port:
	port = atoi(argv[3]);

	//Creating a new socket with socket()-- 
	// AF_INET is set as family to use IPv4 protocals,
	// and SOCK_STREAM is set as the type for a two-way connection:
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket.\n");
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	//Set the address family for socket as TCP/IP:
	serv_addr.sin_family = AF_INET;
	//Convert port to big endian for TCP/IP network byte order:
	serv_addr.sin_port = htons(port);
	//Accept any incoming connecection:
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//Connect to the server:
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
	//	fprintf(stderr, "ERROR connecting to port %d.\n", port); //frintf set to std err
		exit(2);
	}
	n = write(sockfd, textContents, strlen(textContents));
	if (n < 0) {
		fprintf(stderr, "ERROR writing %s to socket.\n", argv[1]);
	}
	n = write(sockfd, keyContents, strlen(keyContents));
	if (n < 0) {
		perror("ERROR writing to socket.\n");
	}
	bzero(buffer, BUFSIZE);
	n = read(sockfd, buffer, BUFSIZE); // Waiting for server response
	if (n < 0) {
		perror("ERROR reading from socket.\n");
	}
	printf("%s\n", buffer);//Output the ciphertext
	close(sockfd);//close socket
	return 0;
}
