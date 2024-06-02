/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/05/28 16:31:27 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *get_line(t_mode mode, int fd)
{
    char	*line;

	if (mode == INTERACTIVE)
	{
		line = readline("shell >>> ");
		if (line && ft_strlen(line) > 0)
			add_history(line);
	}
	if (mode == NON_INTERACTIVE)
	{
		line = get_next_line_bonus(fd);
        // printf("line: %s\n", line);
		// TD close()
	}
	if (!line)// || mode == NON_INTERACTIVE)
	{
		free(line);
		return (NULL);
	}
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
    set_signals(minishell->mode); // sets the signals (ctrl + c, ctrl + d, ctrl + \)
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
    free_minishell(minishell);
	return (0);
}
