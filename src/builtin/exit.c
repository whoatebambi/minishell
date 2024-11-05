/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/04 23:48:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_args(char **tab, int *flag)
{
	int	i;

	i = 0;
	if (!ft_isdigit(tab[1][0]) && tab[1][0] != '+' && tab[1][0] != '-')
	{
		safe_write(2, "minishell: exit: ", tab[i],
			": numeric argument required\n", NULL);
		(*flag) = 1;
		return (2);
	}
	while (tab[1][++i])
	{
		if (!ft_isdigit(tab[1][i]) && !(*flag))
		{
			safe_write(2, "minishell: exit: ", tab[i],
				": numeric argument required\n", NULL);
			(*flag) = 1;
			return (2);
		}
	}
	return (ft_atoi(tab[1]) % 256);
}

void	builtin_exit(char **tab, t_shell *minishell, t_fds *fd)
{
	int	ext;
	int	flag;

	flag = 0;
	if (tab[1])
		minishell->tmpexcode = exit_args(tab, &flag);
	if (tab[1] && tab[2] && !flag)
	{
		safe_write(2, "minishell: `", tab[0], "': too many arguments\n", NULL);
		safe_write(1, "exit\n", NULL);
		minishell->excode = 1;
		minishell->tmpexcode = 1;
		return ;
	}
	if (minishell->count_pipes > 1)
		return ;
	ext = minishell->tmpexcode;
	free_minishell(minishell);
	close_fds(fd);
	safe_write(1, "exit\n", NULL);
	exit(ext);
}
