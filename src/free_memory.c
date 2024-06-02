/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/05/28 16:31:27 by fcouserg         ###   ########.fr       */
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

void    free_minishell(t_shell *minishell)
{
    free_env_lst(minishell->env_lst);
    free_cmd_table(minishell->cmd_table);
    free(minishell);
}