//OTP_DEC_D.C
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h> 
#define BUFSIZE 1024 

//Function for dycryption:
int charToInt(char c);

int main(int argc, char *argv[]) {
	int sockfd, acceptfd, port;
	socklen_t clilen;
	char recvbuf[BUFSIZE], sendbuf[BUFSIZE];
	struct sockaddr_in serv_addr, cli_addr;
	int n, i;
	pid_t pid, sid;
	pid = fork();
	int cipherNum, keyNum, plainNum;
	char ciphertext[BUFSIZE];
	char keyText[BUFSIZE];
	if (pid < 0) { //Case error
		exit(1);
	}
	if (pid > 0) { 
		exit(2);
	}
	sid = setsid();
	if (sid < 0) {
		exit(1);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	sockfd = socket(AF_INET, SOCK_STREAM, 0); // Set socket file descriptor for IP, TCP
	if (sockfd < 0) {
		perror("ERROR opening socket.\n");
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	port = atoi(argv[1]);
	//Set the address family for socket as TCP/IP:
	serv_addr.sin_family = AF_INET;
	//Accept any incoming connection:
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	//Set the port number to TCP / IP mode:
	serv_addr.sin_port = htons(port);
	//Ensure that asock address can be reused:
	int yes = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	setsockopt(acceptfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	//Bind to socket:
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding.\n");
	}
//	listen(sockfd, 5);
	listen(sockfd, 15);
	clilen = sizeof(cli_addr);
	while (1) {
		//Accept incoming connection from client process
		acceptfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (acceptfd < 0) {
			perror("ERROR on accept.\n");
		}
		bzero(recvbuf, BUFSIZE);
		n = read(acceptfd, ciphertext, BUFSIZE); //get ciphertext into string ciphertext
		if (n < 0) {
			perror("ERROR reading from socket.\n");
		}
		bzero(recvbuf, BUFSIZE);
		n = read(acceptfd, keyText, BUFSIZE); //Get key into string keyText
		char ptext[strlen(ciphertext)]; //Initialize string to send out
		char alphabet[28] = { ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
		//for loop below: 
		//charToInt is used to get the number for each corresponding
		//letter in keyText and ptext, then those numbers are used to calculate
		//the number for the letter in the dycrypted file.
		//The index numbers of the string alphabet correspond to the letter that they imply. 
		for (i = 0; i < strlen(ciphertext); i++) {
			cipherNum = charToInt(ciphertext[i]);
			keyNum = charToInt(keyText[i]);
			plainNum = (cipherNum - keyNum);
			if (plainNum < 0) {
				plainNum += 27;
			}
			// Use plainNum as index for alphabet letter and assign to appropriate index of ptext:
			ptext[i] = alphabet[plainNum]; 
		}
		//Add terminating character to ptext:
		ptext[strlen(ciphertext) - 1] = '\0';
		bzero(sendbuf, BUFSIZE);
		snprintf(sendbuf, BUFSIZE, "%s", ptext);
		//Write the decrypted text to the buffer:
		n = write(acceptfd, sendbuf, strlen(sendbuf));
		if (n == -1) {
			perror("ERROR writing to socket.\n");
		}
	}
	return 0;
}

int charToInt(char c) {
	switch (c) {
	case 'A':
		return 0;
	case 'B':
		return 1;
	case 'C':
		return 2;
	case 'D':
		return 3;
	case 'E':
		return 4;
	case 'F':
		return 5;
	case 'G':
		return 6;
	case 'H':
		return 7;
	case 'I':
		return 8;
	case 'J':
		return 9;
	case 'K':
		return 10;
	case 'L':
		return 11;
	case 'M':
		return 12;
	case 'N':
		return 13;
	case 'O':
		return 14;
	case 'P':
		return 15;
	case 'Q':
		return 16;
	case 'R':
		return 17;
	case 'S':
		return 18;
	case 'T':
		return 19;
	case 'U':
		return 20;
	case 'V':
		return 21;
	case 'W':
		return 22;
	case 'X':
		return 23;
	case 'Y':
		return 24;
	case 'Z':
		return 25;
	case ' ':
		return 26;
	}
}

