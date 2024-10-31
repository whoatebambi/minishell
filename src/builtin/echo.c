/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/31 16:28:09 by fcouserg         ###   ########.fr       */
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

void	echo_excode(t_shell *minishell, int i, int *ext)
{
	if (i + 1 < minishell->count_pipes)
		*ext = minishell->excode;
	else
		*ext = minishell->tmpexcode;
}

void	builtin_echo(char **tab, t_shell *minishell, int i)
{
	int	j;
	int	flag;
	int	ext;

	flag = 0;
	if (!tab || !tab[0])
		return ;
	j = check_newline(tab, &flag);
	if (tab[1])
	{
		while (tab[j])
		{
			if (!tab[j + 1])
				printf("%s", tab[j]);
			else
				printf("%s ", tab[j]);
			j++;
		}
	}
	if (!flag)
		printf("\n");
	echo_excode(minishell, i, &ext);
	free_minishell(minishell);
	exit(ext);
}

// if (i + 1 < minishell->count_pipes)
// 	minishell->excode = minishell->tmpexcode;
// else
// 	minishell->tmpexcode = minishell->excode;