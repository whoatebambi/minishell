/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/13 19:29:29 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_line(t_mode mode, int fd)
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
		// TD close()
	}
	if (!line) // || mode == NON_INTERACTIVE)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd;
	t_shell	*minishell;

	(void)envp;
	fd = init_argc(argc, argv, fd);        
	minishell = init_minishell(envp, argc);
	set_signals(minishell->mode);          
	while (1)
	{
		minishell->line = get_line(minishell->mode, fd);
		if (minishell->line == NULL)
			break ;
		ft_parseur(minishell);

		execute(minishell, minishell->clean_line);		
		free_minishell_loop(minishell);
	}
	close(fd);
	free_minishell(minishell);
	return (0);
}
