/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/05/24 20:27:06 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_argc(int argc, char **argv,int fd)
{
	fd = 0;
	if (argc > 2) // usage is either "./minishell" or "./minishell <file>"
	{
		errno = E2BIG; // the int errno is set to the macro "E2BIG"
		perror("Error");
		exit(E2BIG); // "E2BIG" macro from <errno.h> expands to "Argument list too long."
	}
		
	if (argc == 2) // opens the file (we are in non-interactive mode)
		fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		// no need to set errno to a macro here, the system call open() automatically did it
		perror("Error");
		exit(EXIT_FAILURE);
	}
    return (fd);
}

t_shell	*init_minishell(char **envp, int argc)
{
	t_shell	*minishell;

	minishell = calloc(sizeof(t_shell), 1);
    // manage calloc error here
	get_minishell(minishell);
	if (argc == 1)
		minishell->mode = INTERACTIVE;
	else
		minishell->mode = NON_INTERACTIVE;
	minishell->cmd_table = NULL;
	return (minishell);
	// char			**environment;
	// t_pipe		*pipes;
	// t_env		*env_var;
	// t_parseur	*parseur;
}

char    *get_line(t_mode mode, int fd)
{
    char	*line;

	if (mode == INTERACTIVE)
	{
		line = readline("shell >>> ");
		// if (line && ft_strlen(line) > 0)
		// 	add_history(line);
	}
	if (mode == NON_INTERACTIVE)
	{
		line = get_next_line_bonus(fd);
		// printf("%s", line);
		// if (close(fd) == -1)
		// 	printf("close error\n");
		// if (close(fd) == 0)
		// 	printf("close success\n");
	}
	// if (!line || mode == NON_INTERACTIVE)
	// {
	// 	free(line);
	// 	return (NULL);
	// }
	return (line);
}


int	main(int argc, char **argv, char **envp)
{
    int         fd;
    t_shell *minishell;
    char        *line;

    (void)envp;
    fd = init_argc(argc, argv, fd); // checks the number of arguments and initialize fd if argc = 2
    minishell = init_minishell(envp, argc); // sets the minishell structure and creates singleton logic
    while (1)
    {
        line = get_line(minishell->mode, fd); // gets the line from the user or from the file
        if (line == NULL)
            break;
        // parse(line);
        // free(line);
        execute(minishell, line);
		free(line);
    }
    // free_minishell(minishell);
	return (0);
}
