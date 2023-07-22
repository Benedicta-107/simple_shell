#ifndef SIMPLESHELL_H
#define SIMPLESHELL_H

#define MAX_COMMAND_LENGTH 1024

void display_prompt(void);
void execute_command(const char *command);
int main(void);
void handle_command(const char *command, char *args[]);

#endif
