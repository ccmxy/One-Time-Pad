#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
	int idx = 0, length;
	srand(time(NULL)); //Generate psuedo-random number
	sscanf (argv[1],"%d",&length); //Get the number of elements into length
	char c;
	char string[length];
	for(idx = 0; idx < length; idx++){
	//Use the ascii letters A-Z for the key:
		c = (65 + rand()%26); 
		string[idx] = c;
	}
	string[length] = '\n';
	printf("%s", string);
	return (0);
}
