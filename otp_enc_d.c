//OTP_ENC_D.C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h> 

#define BUFSIZE 1024

//Encryption functions:
char getLetter(int num);
int getNum(char c);

int main(int argc, char *argv[]) {
	int sockFD;
	int newsockFD; //Socket to send data through
	int port; //Holds address of service endpoint (the port in the machine)
	socklen_t clilen; //Holds the length of the client address
	char inBuf[BUFSIZE], outBuf[BUFSIZE]; //strings to receive and send data
	struct sockaddr_in serv_addr, cli_addr; //Strucks for server and client
	int n, idx;

	pid_t pid, sid; // process ID and Session ID

	//Create new process
	pid = fork();
	if (pid < 0) {
		exit(1);
	}

	// Have parent exit:
	if (pid > 0) {
		exit(0);
	}

	//Set session ID:
	sid = setsid();
	if (sid < 0) {//If pid failed...
		exit(1);
	}
	//Close standard file descriptors so daemon can't access terminal:
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	sockFD = socket(AF_INET, SOCK_STREAM, 0); // Set socket file descriptor for IP, TCP
	if (sockFD < 0) {
		perror("Socket error.\n");
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	port = atoi(argv[1]);
	//Set the address family for socket as TCP/IP:
	serv_addr.sin_family = AF_INET;
	//Accept any incoming connection:
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	//Set the port number to TCP / IP mode:
	serv_addr.sin_port = htons(port);
	int yes = 1;
	//Ensure that sock address can be re-used 
	setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	setsockopt(newsockFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	//Bind to socket:
	if (bind(sockFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Bind error.\n");
	}
	//Listen for incoming connections:
	listen(sockFD, 5);
	//Get length of client address for accept:
	clilen = sizeof(cli_addr);

	while (1) {
		newsockFD = accept(sockFD, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockFD < 0) {
			error("Failed to accept!\n");
		}

		bzero(inBuf, BUFSIZE);
		//Read the plaintext into inBuf:
		n = read(newsockFD, inBuf, BUFSIZE);
		if (n < 0) {
			error("Failed to read from socket!\n");
		}
		char plaintextContent[BUFSIZE];
		//Put it in plaintextConent:
		snprintf(plaintextContent, strlen(inBuf), "%s", inBuf);
		bzero(inBuf, BUFSIZE);
		n = read(newsockFD, inBuf, BUFSIZE);
		if (n < 0) {
			error("Failed to read from socket!\n");
		}
		char keyContent[BUFSIZE];
		snprintf(keyContent, strlen(inBuf), "%s", inBuf);
		int ptxtNum; //Holds numbers that correspond to plaintext letters
		int keyNum; //Holds numbers that correspond to key text letters
		int sum;
		char c;
		int sizeX = strlen(plaintextContent);
		char newArray[sizeX];
		int length = strlen(plaintextContent);
		//Encrypt the text:
		for (idx = 0; idx < length; idx++){
			//getNum returns the numeric equivilants of the letters
			//in the keytext and ciphertext:
			ptxtNum = getNum(plaintextContent[idx]);
			keyNum = getNum(keyContent[idx]);
			sum = ptxtNum + keyNum;
			if (sum > 27){
				sum = (ptxtNum + keyNum - 27);
			}
			c = getLetter(sum);//Get the letter equivilant of sum:	
			newArray[idx] = c;//Assign to position in ciphertext
		}
		newArray[strlen(newArray) + 1] = '\0';//Set terminating char
		bzero(outBuf, BUFSIZE);
		snprintf(outBuf, BUFSIZE, "%s", newArray);
		n = write(newsockFD, outBuf, strlen(outBuf));
		if (n < 0) {
			perror("Failed to write to socket!\n");
		}
	}
	return 0;
}


char getLetter(int num){
	switch (num){
	case 1:
		return 'A';
	case 2:
		return 'B';
	case 3:
		return 'C';
	case 4:
		return 'D';
	case 5:
		return 'E';
	case 6:
		return 'F';
	case 7:
		return 'G';
	case 8:
		return 'H';
	case 9:
		return 'I';
	case 10:
		return 'J';
	case 11:
		return 'K';
	case 12:
		return 'L';
	case 13:
		return 'M';
	case 14:
		return 'N';
	case 15:
		return 'O';
	case 16:
		return 'P';
	case 17:
		return 'Q';
	case 18:
		return 'R';
	case 19:
		return 'S';
	case 20:
		return 'T';
	case 21:
		return 'U';
	case 22:
		return 'V';
	case 23:
		return 'W';
	case 24:
		return 'X';
	case 25:
		return 'Y';
	case 26:
		return 'Z';
	case 27:
		return ' ';
	default:
		perror("Line 273: Invalid number");
	}
}


int getNum(char c) {
	switch (c) {
	case 'A':
		return 1;
	case 'B':
		return 2;
	case 'C':
		return 3;
	case 'D':
		return 4;
	case 'E':
		return 5;
	case 'F':
		return 6;
	case 'G':
		return 7;
	case 'H':
		return 8;
	case 'I':
		return 9;
	case 'J':
		return 10;
	case 'K':
		return 11;
	case 'L':
		return 12;
	case 'M':
		return 13;
	case 'N':
		return 14;
	case 'O':
		return 15;
	case 'P':
		return 16;
	case 'Q':
		return 17;
	case 'R':
		return 18;
	case 'S':
		return 19;
	case 'T':
		return 20;
	case 'U':
		return 21;
	case 'V':
		return 22;
	case 'W':
		return 23;
	case 'X':
		return 24;
	case 'Y':
		return 25;
	case 'Z':
		return 26;
	case ' ':
		return 27;
	default:
		perror("Bad file!\n");
	}
}




