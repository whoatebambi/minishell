/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/02/23 20:09:35 by fcouserg         ###   ########.fr       */
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
		//  When a system call fails, it usually returns -1 and sets the variable errno
		// to a value describing what went wrong. (These values can be found in <errno.h>.) 
		// Many library functions do likewise.  The function perror() serves to translate
		// this error code into human-readable form.
		exit(EXIT_FAILURE);
		// Indicates unsuccessful termination. However, its usage is non-portable.
		// For example, on OpenVMS, exit(1) actually indicates success.
		// Only EXIT_FAILURE is the standard value for returning unsuccessful termination,
		// but 1 is used for the same in many implementations.
	}
    return (fd);
}

// This is a singleton. The concept is to create an object once
// and then be able to access it any time while not using global variable
// thanks to a stored pointer. (from Ophelie)
t_minishell	*get_minishell(t_minishell *minishell)
{
	static t_minishell	*pointer_to_minishell = NULL;

	if (!pointer_to_minishell && minishell)
		pointer_to_minishell = minishell;
	return (pointer_to_minishell);
}

t_minishell	*init_minishell(char **envp, int argc)
{
	t_minishell	*minishell;

	minishell = calloc(sizeof(t_minishell), 1);
    // manage calloc error here
	get_minishell(minishell);
	if (argc == 1)
		minishell->mode = INTERACTIVE;
	else
		minishell->mode = NON_INTERACTIVE;
	minishell->nb_cmds = 0;
	minishell->instructions = NULL;
	// minishell->env = init_env_lst(envp);
	// minishell->env_paths = NULL;
	// minishell->cmd_table = NULL;
	minishell->child_pids = NULL;
	minishell->exit_code = 0;
	return (minishell);
}

char    *get_line(t_mode mode, int fd)
{
    char	*line;
	// int		nb_bits_read;

	if (mode == INTERACTIVE)
	{
		line = readline("shell >>> ");
		// if (line && ft_strlen(line) > 0)
		// 	add_history(line);
	}
	if (mode == NON_INTERACTIVE)
        line = get_next_line_bonus(fd); // why is there no bits read handling?
	if (!line || mode == NON_INTERACTIVE)
	{
		free(line);
		return (NULL);
	}
	return (line);
}


int	main(int argc, char **argv, char **envp)
{
    int         fd;
    t_minishell *minishell;
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
        ft_printf("%s\n", line);
        free(line);
        // execute(minishell);
    }
    // free_minishell(minishell);
	return (0);
}
