/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/26 15:12:02 by codespace        ###   ########.fr       */
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

char	*get_dir(t_shell *minishell)
{
	char	*buf;
	char	*str;

	buf = ft_calloc(PATH_MAX, sizeof(char *));
	if (buf == NULL)
	{
		perror("malloc");
		free_minishell(minishell);
		exit(EXIT_FAILURE);
	}
	buf = getcwd(buf, PATH_MAX);
	if (buf == NULL)
	{
		perror("getcwd");
		free(buf);
		free_minishell(minishell);
		exit(ERANGE);
	}
	str = ft_strjoin(buf, "> ");
	if (str == NULL)
	{
		perror("malloc");
		free(buf);
		free_minishell(minishell);
		exit(EXIT_FAILURE);
	}
	return (str);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd;
	t_shell	minishell;

	fd = init_argc(argc, argv, fd);
	init_minishell(&minishell, envp, argc);
	ft_signals();   
	// while (1)
	// {
	// 	minishell.cwd = get_dir(&minishell);
	// 	minishell.line = get_line(minishell.mode, fd);
	// 	if (minishell.line == NULL)
	// 		break ;
	// 	// ft_parseur(&minishell);
	// 	// start_exec(&minishell);
	// 	reset_loop(&minishell);
	// }
	close(fd);
	free_minishell(&minishell);
	// printf("END\n");
	return (0);
}
