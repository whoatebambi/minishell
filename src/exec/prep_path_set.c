/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_path_set.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 01:03:18 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_cmdtable_path_still(t_shell *minishell, t_cmd_table *c)
{
	if (c->path)
		(free(c->path), c->path = NULL);
	minishell->excode = 127;
	c->path = ft_strdup(c->tab[0]);
	if (!c->path)
		exitmsg(minishell, "Malloc error");
}

void	set_cmdtable_path(t_shell *minishell, t_cmd_table *c,
	char *tmp, int *flag)
{
	free(c->tab[0]);
	c->tab[0] = ft_strdup(tmp);
	if (!c->tab[0])
		(free(tmp), c->tab[0] = NULL, exitmsg(minishell, "Malloc error"));
	c->path = ft_strdup(tmp);
	if (!c->path)
		(free(tmp), exitmsg(minishell, "Malloc error"));
	(*flag) = 1;
}
