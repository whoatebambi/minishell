/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/29 18:10:59 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_sig = 0;

char	*get_line(t_mode mode, int fd)
{
	char	*line;

	if (mode == INTERACTIVE)
	{
		line = readline("shell >>> ");
		if (line && ft_strlen(line) > 0)
			add_history(line);
	}
	else
		line = get_next_line_bonus(fd);
	return (line);
}

#include <ctype.h>

int ft_check_emptyline(char *line)
{
    if (line == NULL)
        return 1;

    while (*line)
    {
        if (*line != ' ')
            return 0;
        line++;
    }
    return 1;
}

int	main(int argc, char **argv, char **envp)
{
	int		fd;
	t_shell	minishell;

	fd = 0;
	fd = init_argc(argc, argv, fd);
	init_minishell(&minishell, envp, argc);
	ft_signals();
	while (1)
	{
		minishell.line = get_line(minishell.mode, fd);
		if (minishell.line == NULL)
			break ;
		if (ft_check_emptyline(minishell.clean_line) == 0)
		{
			ft_parseur(&minishell);
			start_exec(&minishell);
		}
		reset_loop(&minishell);
	}
	close(fd);
	free_minishell(&minishell);
	exit(minishell.tmpexcode);
}
