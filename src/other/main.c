/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/24 18:19:10 by fcouserg         ###   ########.fr       */
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
	if (mode == NON_INTERACTIVE)
	{
		line = get_next_line_bonus(fd);
		// TD close()
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
	int		fd;
	t_shell	minishell;

	(void)envp;
	fd = init_argc(argc, argv, fd);      
	   
	init_minishell(&minishell, envp, argc);
	ft_signals();       
	while (1)
	{
		minishell.line = get_line(minishell.mode, fd);
		if (minishell.line == NULL)
			break ;
		ft_parseur(&minishell);
		execute(&minishell);	
		reset_loop(&minishell);
	}
	close(fd);
	free_minishell(&minishell);
	return (0);
}
