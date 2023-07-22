#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "simpleshell.h"

/**
 * handle_command - to execute the command
 * @command: command to be executed
 * @args: argument to be executed
 *
 * Return: Always 0.
 */

void handle_command(const char *command, char *args[])
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork");
	} else if (pid == 0)
	{
		if (execvp(command, args) < 0)
		{
			perror("execvp");
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

