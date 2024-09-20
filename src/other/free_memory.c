/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/20 16:42:09 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_env_lst(t_list *env_lst)
{
    t_list  *tmp;
    t_env   *env_var;
    
    if (env_lst == NULL)
        return ;
    while (env_lst)
    {
        tmp = env_lst->next;
        env_var = (t_env *)env_lst->content;
        if (env_var)
        {
            free(env_var->key);
            free(env_var->var);
            free(env_var);
        }
        free(env_lst);
        env_lst = tmp;
    }
}

// to remove in a bit since i make a new one (similar)
void    free_cmd_table(t_cmd_table **cmd_table)
{
    int     i;
    int     j;

    if (cmd_table == NULL)
        return ;
    i = 0;
    while (cmd_table[i] != NULL)
    {
        j = 0;
        while (cmd_table[i]->cmd_args[j] != NULL)
        {
            free(cmd_table[i]->cmd_args[j]);
            j++;
        }
        free(cmd_table[i]->cmd_args);
        free(cmd_table[i]);
        i++;
    }
    free(cmd_table);
}


void	free_minishell(t_shell *minishell)
{
	if (minishell == NULL)
		return;
	free_env_lst(minishell->env_lst);
	ft_free_cmd_table_final(minishell->cmd_table, minishell->count_pipes);
    if (minishell->envp)
	    ft_free_double_char(minishell->envp);
	// ft_free_line(minishell->line);
	// ft_free_line(minishell->clean_line);
	free(minishell);
	minishell = NULL;
}

void	free_minishell_loop(t_shell *minishell)
{
	if (minishell == NULL)
		return;
	ft_free_cmd_table_loop(minishell->cmd_table, minishell->count_pipes);
    if (minishell->envp)
	    ft_free_double_char(minishell->envp);
	ft_free_line(minishell->line);
	ft_free_line(minishell->clean_line);
}