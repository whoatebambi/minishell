/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 11:05:04 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_sig = 0;

int	main(int argc, char **argv, char **envp)
{
	int		fd;
	t_shell	minishell;
	int		flag;

	fd = 0;
	fd = init_argc(argc, argv, fd);
	init_minishell(&minishell, envp, argc);
	set_signals();
	while (1)
	{
		minishell.line = get_line(minishell.mode, fd);
		if (minishell.line == NULL)
			break ;
		flag = ft_parseur(&minishell);
		if (ft_check_emptyline(minishell.clean_line) == 0 && flag == 1)
			launch_exec(&minishell);
		reset_loop(&minishell);
	}
	close(fd);
	free_minishell(&minishell);
	exit(minishell.tmpexcode);
}
