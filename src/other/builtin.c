/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/20 19:24:58 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_arg)
{
	if (safe_strcmp(cmd_arg, "pwd") == 0)
		return (1);
	else if (safe_strcmp(cmd_arg, "echo") == 0)
		return (1);
	else if (safe_strcmp(cmd_arg, "env") == 0)
		return (1);
	else if (safe_strcmp(cmd_arg, "export") == 0)
		return (2);
    else if (safe_strcmp(cmd_arg, "cd") == 0)
		return (1);
	// unset
	// exit
	else
		return (0);
}

void	replace_env_var(char *pwd, char *key, t_list *env_lst)
{
	while (env_lst)
	{
		if (safe_strcmp(((t_env *)(env_lst->content))->key, key) == 0)
		{
			free(((t_env *)(env_lst->content))->var);
			((t_env *)(env_lst->content))->var = ft_strdup(pwd);
		}
		env_lst = env_lst->next;
	}
}

int    cd(char **cmd_args, t_list *env_lst)
{
	char	old_pwd[1024];
    char	new_pwd[1024];
    int		exit_code;

	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		perror("getcwd");
		return (-1);
	}
    if (cmd_args[1] == NULL)
		exit_code = chdir(getenv("HOME"));
	else if (safe_strcmp(cmd_args[1], "-") == 0)
		exit_code = chdir(getenv("OLDPWD"));
	else
		exit_code = chdir(cmd_args[1]);
    if (exit_code == -1)
	{
		perror("chdir");
		return (exit_code);
	}

	if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
	{
		perror("getcwd");
		return (-1);
	}
	replace_env_var(old_pwd, "OLDPWD", env_lst);
	replace_env_var(new_pwd, "PWD", env_lst);
	if (cmd_args[1] && safe_strcmp(cmd_args[1], "-") == 0)
		exit_code = pwd(1);
	// free(new_pwd);
    return (exit_code);
}

int	pwd(int fd_out)
{
	char	*buffer;
    int     exit_code;

	buffer = NULL;
	buffer = getcwd(buffer, 4096); // what value to choose ? 0?
	if (buffer == NULL)
	{
		perror("getcwd");
		free(buffer); // not sure i need to free it
		return (0);
	}
	exit_code = safe_write(fd_out, buffer, "\n", NULL);
	free(buffer);
    if (exit_code == -1)
        return (exit_code);
    return (0);
}

int echo(char **cmd_args, int fd_out)
{
    int exit_code;
    int i;

    i = 1;
    if (cmd_args[1] && safe_strcmp(cmd_args[1], "-n") == 0)
        i = 2;
	while (cmd_args[i])
	{
        if (cmd_args[i + 1])
            exit_code = safe_write(fd_out, cmd_args[i], " ", NULL);
        else
            exit_code = safe_write(fd_out, cmd_args[i], NULL);
        if (exit_code == -1)
            return (exit_code);
		i++;
	}
	if (cmd_args[1] && safe_strcmp(cmd_args[1], "-n") == -1)
        return (safe_write(fd_out, "\n", NULL));
	return (0);
}

int	env(t_list *env_lst, int fd_out)
{
    int     exit_code;
    char    *key;
    char    *var;

	// if (env_lst == NULL)
	// 	return (-1);
	while (env_lst)
	{
        key = ((t_env *)(env_lst->content))->key;
        var = ((t_env *)(env_lst->content))->var;
        exit_code = safe_write(fd_out, key, "=", var, "\n", NULL);
        if (exit_code == -1)
            return (exit_code);
		env_lst = env_lst->next;
	}
    return (0);
}

int	print_export(t_list *env_lst, int fd_out)
{
    int exit_code;
    char    *key;
    char    *var;

	// if (env_lst == NULL)
	// 	return (0);
	while (env_lst)
	{
        key = ((t_env *)(env_lst->content))->key;
        var = ((t_env *)(env_lst->content))->var;
        exit_code = safe_write(fd_out, "export ", key, "=", var, "\n", NULL);
		if (exit_code == -1)
            return (exit_code);
		env_lst = env_lst->next;
	}
    return (0);
}

t_list	*dup_env_lst(t_list *env_lst)
{
	t_list	*dup_env_lst;
	t_list	*new;
	t_env	*env_var;

	dup_env_lst = NULL;
	env_var = NULL;
	while (env_lst)
	{
		env_var = (t_env *)malloc(sizeof(t_env));
		if (!env_var)
			return (NULL);
		env_var->key = ((t_env *)(env_lst->content))->key;
		env_var->var = ((t_env *)(env_lst->content))->var;
		new = ft_lstnew((void *)env_var);
		if (!new)
			return (NULL);
		ft_lstadd_back(&dup_env_lst, new);
		env_lst = env_lst->next;
	}
	return (dup_env_lst);
}

void sort_env_lst(t_list **list)
{
    t_list *i;
    t_list *j;
    t_env *env_i;
    t_env *env_j;

    if (!list || !(*list))
        return;
    i = *list;
    while (i->next != NULL)
    {
        j = i->next;
        while (j != NULL)
        {
            env_i = (t_env *)(i->content);
            env_j = (t_env *)(j->content);
            if (ft_strncmp(env_i->key, env_j->key, 900) > 0)
                swap_env(i, j);
            j = j->next;
        }
        i = i->next;
    }
}

int	export(t_list *env_lst, t_cmd_table *cmd_table, int fd_out)
{
	t_list	*sorted_env_lst;
    int exit_code;

    sorted_env_lst = dup_env_lst(env_lst);
    sort_env_lst(&sorted_env_lst);
    exit_code = print_export(sorted_env_lst, fd_out);
    // free_env_lst(sorted_env_lst);
    return (0);
}
