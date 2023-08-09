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
 * sig_handler - checks if Ctrl C is pressed
 * @sig_num: int
 */
void sig_handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		_puts("\n#cisfun$ ");
	}
}

/**
* _EOF - handles the End of File
* @len: return value of getline function
* @buff: buffer
 */
void _EOF(int len, char *buff)
{
	(void)buff;
	if (len == -1)
	{
		if (isatty(STDIN_FILENO))
		{
			_puts("\n");
			free(buff);
		}
		exit(0);
	}
}
/**
  * _isatty - verif if terminal
  */

void _isatty(void)
{
	if (isatty(STDIN_FILENO))
		_puts("#cisfun$ ");
}
/**
 * main - Shell
 * Return: 0 on success
 */

int main(void)
{
	ssize_t len = 0;
	char *buff = NULL, *value, *pathname, **arv;
	size_t size = 0;
	list_path *head = '\0';
	void (*f)(char **);

	signal(SIGINT, sig_handler);
	while (len != EOF)
	{
		_isatty();
		len = getline(&buff, &size, stdin);
		_EOF(len, buff);
		arv = splitstring(buff, " \n");
		if (!arv || !arv[0])
			execute(arv);
		else
		{
			value = _getenv("PATH");
			head = linkpath(value);
			pathname = _which(arv[0], head);
			f = checkbuild(arv);
			if (f)
			{
				free(buff);
				f(arv);
			}
			else if (!pathname)
				execute(arv);
			else if (pathname)
			{
				free(arv[0]);
				arv[0] = pathname;
				execute(arv);
			}
		}
	}
	free_list(head);
	freearv(arv);
	free(buff);
	return (0);
}
