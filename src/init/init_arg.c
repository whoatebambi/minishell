/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_arg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 01:52:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_line(t_mode mode, int fd)
{
	char	*line;

	if (mode == INTERACTIVE)
	{
		line = readline("minishell >>> ");
		if (line && ft_strlen(line) > 0)
			add_history(line);
	}
	else
		line = get_next_line_bonus(fd);
	return (line);
}

int	ft_check_emptyline(char *line)
{
	if (line == NULL)
		return (1);
	while (*line)
	{
		if (*line != ' ')
			return (0);
		line++;
	}
	return (1);
}

int	init_argc(int argc, char **argv, int fd)
{
	fd = 0;
	if (argc > 2)
	{
		errno = E2BIG;
		perror("Error");
		exit(E2BIG);
	}
	if (argc == 2)
		fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}
	return (fd);
}
