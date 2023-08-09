#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "simpleshell.h"

/**
 * display_prompt - to dispaly the prompt
 *
 * Return: Always 0.
 */

void display_prompt(void)
{
	printf("shell# ");
	fflush(stdout);
}

/**
 * execute_command - to execute the command
 * @command: command to be executed
 *
 * Return: Always 0.
 */

void execute_command(const char *command)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid == 0)
	{
		if (execlp(command, command, (char *)NULL) < 0)
		{
			perror("execlp");
			exit(EXIT_FAILURE);
		}
	} else
	{
		int status;

	waitpid(pid, &status, 0);

		if (WIFEXITED(status))
		{
			int exit_status = WEXITSTATUS(status);

			if (exit_status != 0)
			{
				printf("Command '%s' exited with status %d\n", command, exit_status);
			}
		} else if (WIFSIGNALED(status))
		{
			int term_signal = WTERMSIG(status);

			printf("Command '%s' terminated by signal %d\n", command, term_signal);
		}
	}
}

/**
 * main - check the code
 *
 * Return: Always 0.
 */

int main(void)
{
	char command[MAX_COMMAND_LENGTH];

	while (1)
	{
		display_prompt();
		if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
		{
			printf("\n");
			break;
		}
		command[strcspn(command, "\n")] = '\0';
		execute_command(command);
	}
	return (0);
}
