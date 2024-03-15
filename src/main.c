#include "minishell.h"

// This function parses the line into commands,
// and fills the command_table with tokens via lexer()
t_minishell	*parse(char *line)
{
	t_minishell	*minishell;
	minishell = get_minishell(NULL);
	// if (minishell->commands)
	// 	free_commands(commands);
	minishell->commands = get_commands(line, PIPE);
	minishell->nb_command = ft_nb_command(line, PIPE);
	// printf("TEST - parse\n");
	return (minishell);
}

int	main(int argc, char **argv)
{
	t_minishell	*minishell;
	char		*line;
	int			fd;

	if (argc > 2)
	{
		printf("Wrong number of arguments.\n");
		exit(1);
	}
	minishell = init_minishell(argc);
	fd = 0;
	if (argc == 2)
		fd = open_or_exit(argv[1]);
	line = init_prompt(fd, minishell->mode);
	minishell = parse(line);
	int i = 0;
	while (minishell->commands[i] != NULL)
	{
		printf("[%d] = %s\n", i, minishell->commands[i]);
		i++;
	}
	return (0);
}
