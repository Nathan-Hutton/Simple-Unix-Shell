#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// function prototypes

int parse_command(char command[], char *args[]);

#define MAX_LINE		80 /* 80 chars per line, per command */

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
   	int should_run = 1;
	int command_length;

	char command[MAX_LINE];
	char *stored_command;

    	while (should_run)
    	{
        	printf("prompt>");
        	fflush(stdout);

    		fgets(command, MAX_LINE, stdin);

		if (strncmp(command, "!!", 2) == 0 && strlen(stored_command) == 1){
			printf("No command history\n");
			continue;
		}

		if (strncmp(command, "!!", 2) == 0){
			strcpy(command, stored_command);
		}

		if (strncmp(command, "exit", 4) == 0){
			return 0;
		}

		strcpy(stored_command, command);

	    	command_length = parse_command(command, args);
		args[command_length] = NULL;

		pid_t pid = fork();

		if (pid < 0)
		{
			printf("Failed");
		}
		else if (pid == 0)
		{
			if (execvp(args[0], args) == -1) {
				printf("Command not found\n");
				return 0;
			}
		}
		else
		{
			wait(NULL);
		}
	}


	return 0;
}

/**
 * Parses the command and places each token
 * into args array.
 */
int parse_command(char command[], char *args[])
{
	char *spart = strtok(command, " \n");
	int length = 0;

	while (spart != NULL)
	{
		args[length] = spart;
		length++;
		spart = strtok(NULL, " \n");
	}

	return length;
}
