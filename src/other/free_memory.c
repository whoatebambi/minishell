/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/26 17:01:49 by codespace        ###   ########.fr       */
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
            free(env_var->value);
            free(env_var);
        }
        free(env_lst);
        env_lst = tmp;
    }
}

void	free_cmd_table(t_shell *minishell)
{
	int i = 0;
	
	if (minishell->cmd_table == NULL)
		return ;
	while (i < minishell->count_pipes)
	{
		if (minishell->cmd_table[i] != NULL)
		{
			// safe_free(minishell->cmd_table[i]->group_command);
			// ft_free_node(minishell->cmd_table[i]->nodes);
			// ft_free_double_char(minishell->cmd_table[i]->cmd_args);
			// ft_free_redir(minishell->cmd_table[i]->redirs_in);
			// ft_free_redir(minishell->cmd_table[i]->redirs_out);
			// safe_free(minishell->cmd_table[i]->infile_tmp);
            free(minishell->cmd_table[i]);
            minishell->cmd_table[i] = NULL;
        }
		i++;
	}
	free(minishell->cmd_table);
	minishell->cmd_table = NULL;
}

void	reset_loop(t_shell *minishell) // || reset_shell
{
	if (minishell->cmd_table)
		free_cmd_table(minishell);
    if (minishell->child_pids)
    {
        free(minishell->child_pids);
        minishell->child_pids = NULL;
    }
    if (minishell->line)
    {
        free(minishell->line);
        minishell->line = NULL;
    }
    if (minishell->clean_line)
    {
        free(minishell->clean_line);
        minishell->clean_line = NULL;
    }
    // if (minishell->tabpath)
    // {
    //     ft_free_double_char(minishell->tabpath);
    //     minishell->tabpath = NULL;
    // }
    if (minishell->cwd)
    {
        free(minishell->cwd);
        minishell->cwd = NULL;
    }
	if (minishell->inp)
    {
        free(minishell->inp);
        minishell->inp = NULL;
    }
	if (minishell->newinp)
    {
        free(minishell->newinp);
		minishell->newinp = NULL;
    }
	if (minishell->finalinp)
	{
		free(minishell->finalinp);
		minishell->finalinp = NULL;
	};
	minishell->tmpexcode = minishell->excode;
	minishell->excode = 0;
	minishell->lex = NULL;
	minishell->cmd = NULL;
}

void    free_env(t_env *env)
{
    t_env *tmp;
    
    if (env == NULL)
        return ;
    while (env)
    {
        tmp = env->next;
        if (env->key)
        {
            free(env->key);
            env->key = NULL;
        }
        if (env->value)
        {
            free(env->value);
            env->value = NULL;
        }
        free(env);
        env = tmp;
    }
}

void    free_path(t_path *path)
{
    if (path == NULL)
        return ;
    if (path->pwd)
    {
        free(path->pwd);
        path->pwd = NULL;
    }
    if (path->oldpwd)
    {
        free(path->oldpwd);
        path->oldpwd = NULL;
    }
    free(path);
}

void	free_minishell(t_shell *minishell)
{
	if (minishell == NULL)
		return;
    reset_loop(minishell);
    free_env(minishell->env);
    free_path(minishell->path);
    if (minishell->env_lst)
    {
        free_env_lst(minishell->env_lst);
        minishell->env_lst = NULL;
    }
    if (minishell->envp)
    {
        ft_free_double_char(minishell->envp);
        minishell->envp = NULL;
    }      
}

void	exitmsg(t_shell *minishell, char *errmsg)
{
	int	ext;

	ext = minishell->excode;
	if (errmsg)
		perror(errmsg);
	free_minishell(minishell);
	exit(ext);
}