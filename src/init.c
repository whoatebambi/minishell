#include "minishell.h"

// Interactive mode: the shell waits for the user input line via readline()
// Non-interactive mode: it gets the first line via get_next_line()
char	*init_prompt(int fd, t_mode mode)
{
	char		*line;

	line = NULL;
	if (mode == INTERACTIVE)
	{
		line = readline(PROMPT);
		if (*line && ft_strlen(line) > 0)
			add_history(line);
	}
	if (mode == NON_INTERACTIVE)
	{
		line = get_next_line(fd);
		printf(PROMPT);
		printf("%s", line);
	}
	// if (!line) // || mode == NON_INTERACTIVE)
	// {			
	// 	free(line);
	// 	return (NULL);
	// }
	return (line);
}

// The shell may be executed interactively or non-interactively.
// In interactive mode (argc = 1), the shell accepts input from the keyboard.
// In non-interactive mode (argc = 2), it executes commands read from a file.
t_minishell	*init_minishell(int argc)
{
	t_minishell	*minishell;
	minishell = calloc_or_exit(sizeof(t_minishell), 1);
	minishell = get_minishell(minishell);
	if (argc == 1)
		minishell->mode = INTERACTIVE;
	else
		minishell->mode = NON_INTERACTIVE;
	minishell->commands = NULL;
	minishell->nb_command = 0;
	// printf("TEST - init_minishell\n");
	return (minishell);
}

int	open_or_exit(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		printf("Error when trying to open the file.\n");
		exit(1);
	}
	return (fd);
}