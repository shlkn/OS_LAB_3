#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void onCtrlC(int sig); // signal handler
pid_t pid; // child proccess id 
void parse_words(char *cmd, char *pcmd[]); // parses words for execvp()
void main_cycle(); // main cycle of this program

int main()
{
	main_cycle();
	exit(0);
}

void main_cycle()
{
	char *pcmd[10], cmd[256];
	printf("Enter action\n");
	while(1)
	{
		printf("$");
		scanf("%[^\r\n]", cmd); // reads full string
		parse_words(cmd, pcmd); // parses words for execvp()

		pid = fork(); // call fork()
		switch(pid)
		{
			case -1: // in case of error in fork()
			{
				printf("Error, in fork\n");
				break;
			}
			case 0: // for child process
			{
				if(execvp(pcmd[0], pcmd) == -1) // call execvp, v - means vector of arguments, p - path
				{
					printf("Error, can`t exec %s\n", pcmd[0]);
					exit(1);
				}
			}
			default: // for parent process
			{
				int stat; // exit status
				(void)signal(SIGINT, onCtrlC); // signal handler
				wait(&stat); // waiting for end of child process
				if(WIFEXITED(stat)) // normal end of child process? 
					printf("Process ended successfully\n");
			}
		}
		getchar();
	}
}

void parse_words(char *cmd, char *pcmd[])
{
	char  components[10][30], *temp;
	int i = 0;
	if(!strcmp(cmd, "exit")) // user entered "exit"?
		exit(1);
	temp = strtok(cmd, " "); // statrs to parse string that contains command
	while(temp != NULL) // is it end of string?
	{
		strcpy(components[i], temp); // copy string from temp to vector of srtigns
		pcmd[i] = components[i]; // getting pointer to string for vector of pointers
		i++;
		temp = strtok(NULL, " "); // continue parse string
	}
	pcmd[i] = NULL; // shows end of vector of pointers for exec()
}

void onCtrlC(int sig)
{
	printf("I got signal\n");
	if(kill(pid, SIGINT) == -1) 
	{
		printf("Error, can`t find pid of child process, exited main program\n");
		exit(1);
	}
	else printf("process killed\n");
} 