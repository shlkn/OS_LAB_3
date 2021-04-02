#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	printf("Enter action\n");
	char cmd[256], components[10][30], *pcmd[10], *temp;
	while(1)
	{
		printf("$");
		scanf("%[^\r\n]", cmd);
		int i = 0;
		if(strcmp(cmd, "exit") == 0)
			break;
		temp = strtok(cmd, " ");
		while(temp != NULL)
		{
			pcmd[i] = components[i];
			strcpy(components[i], temp);
			i++;
			temp = strtok(NULL, " ");
		}
		pcmd[i] = NULL;
		pid_t pid;
		int stat;
		pid = fork();
		switch(pid)
		{
			case -1:
			{
				printf("Error, in fork\n");
				break;
			}
			case 0:
			{
				execvp(pcmd[0], pcmd);
				exit(1);
				break;
			}
			default:
			{
				int stat_val;
				pid_t child_pid = wait(&stat_val);
				wait(&stat);
				if(WIFEXITED(stat))
					printf("done\n");
				getchar();
			}
		}
	}
	
	exit(0);
}