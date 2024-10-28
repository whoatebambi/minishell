/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/28 14:59:31 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_newline(char **tab, int *flag)
{
	int	n;
	int	i;

	n = 1;
	while (tab[n] && tab[n][0] == '-' && tab[n][1] == 'n')
	{
		i = 1;
		if (tab[n][0] == '-' && tab[n][1] == 'n')
		{
			while (tab[n][i] == 'n')
				i++;
			if (tab[n][i] != '\0')
				return (n);
		}
		*flag = 1;
		n++;
	}
	return (n);
}

void builtin_echo(char **tab, int fd_out, t_shell *minishell, int i)
{
    int j;
	int	flag;
	int	ext;

	flag = 0;
    j = check_newline(tab, &flag);
	if (tab[1])
	{
		while (tab[j])
		{
			if (tab[j] && !tab[j + 1])
				safe_write(fd_out, tab[j], NULL);	
			else if (tab[j])
				safe_write(fd_out, tab[j], " ", NULL);
			j++;
		}
	}
	if (!flag)
        safe_write(fd_out, "\n", NULL);
	if (minishell->cmd_table[i  + 1])
		ext = minishell->excode;
	else
		ext = minishell->tmpexcode;
	free_minishell(minishell);
	exit(ext);
}