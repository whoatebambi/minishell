/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/12 16:07:16 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_env(void *content)
{
    t_env   *current_env;

    if (content == NULL)
        return ;
    current_env = (t_env *)content;
    free(current_env->key);
    free(current_env->var);
    // content = NULL;
}

void    free_env_lst(t_list *env_lst)
{
    t_list  *tmp_next;
    
    if (env_lst == NULL)
        return ;
    while (env_lst)
    {
        tmp_next = env_lst->next;
        free_env(env_lst->content);
        free(env_lst->content);
        free(env_lst);
        env_lst = tmp_next;
    }
    free(env_lst);
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
	ft_free_double_char(minishell->env);
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
	ft_free_double_char(minishell->env);
	ft_free_line(minishell->line);
	ft_free_line(minishell->clean_line);
}