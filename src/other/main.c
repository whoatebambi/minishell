/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/22 13:30:44 by codespace        ###   ########.fr       */
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
	// if (mode == NON_INTERACTIVE)
	// {
	// 	line = get_next_line_bonus(fd);
	// 	printf("line = %s\n", line);
	// 	// TD close()
	// }
	else
	{
		line = get_next_line_bonus(fd);
		// printf("line = %s\n", line);
	}
	// if (mode == NON_INTERACTIVE && line[0] == '\n')
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

	fd = 0;
	fd = init_argc(argc, argv, fd);
	init_minishell(&minishell, envp, argc);
	ft_signals();
	// set_signals(INIT, minishell->mode, minishell);
	while (1)
	{
		minishell.line = get_line(minishell.mode, fd);
		// printf("excode -> %d     tmpexcode-> %d\n", minishell.excode, minishell.tmpexcode);
		if (minishell.line == NULL)// && minishell.mode == NON_INTERACTIVE)
			break ;
		ft_parseur(&minishell);
		// printf("excode -> %d     tmpexcode-> %d\n", minishell.excode, minishell.tmpexcode);
		start_exec(&minishell);
		// printf("excode -> %d     tmpexcode-> %d\n", minishell.excode, minishell.tmpexcode);
		reset_loop(&minishell);
		// printf("excode -> %d     tmpexcode-> %d\n", minishell.excode, minishell.tmpexcode);
	}
	close(fd);
	// printf("excode -> %d     tmpexcode-> %d\n", minishell.excode, minishell.tmpexcode);
	free_minishell(&minishell);
	// printf("excode -> %d     tmpexcode-> %d\n", minishell.excode, minishell.tmpexcode);
	exit(minishell.tmpexcode);
}
