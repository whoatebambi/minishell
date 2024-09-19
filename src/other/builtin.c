/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/19 19:27:35 by fcouserg         ###   ########.fr       */
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

void    cd(char **cmd_args, t_list *env_lst)
{
	char	old_pwd[1024];
    char	new_pwd[1024];
    int		exit_code;

	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
		return (perror("getcwd"));
    if (cmd_args[1] == NULL)
		exit_code = chdir(getenv("HOME"));
	else if (safe_strcmp(cmd_args[1], "-") == 0)
		exit_code = chdir(getenv("OLDPWD"));
	else
		exit_code = chdir(cmd_args[1]);
    if (exit_code == -1)
		return (perror("chdir"));
	if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
		return (perror("getcwd"));
	replace_env_var(old_pwd, "OLDPWD", env_lst);
	replace_env_var(new_pwd, "PWD", env_lst);
	if (cmd_args[1] && safe_strcmp(cmd_args[1], "-") == 0)
		pwd(1);
	// free(new_pwd);
}

void	pwd(int fd_out)
{
	char	*buffer;

	buffer = NULL;
	buffer = getcwd(buffer, 4096); // what value to choose ? 0?
	if (buffer == NULL)
		return (perror("getcwd"));
	safe_write(fd_out, buffer, "\n", NULL);
	free(buffer);
}

int	check_newline(char **cmd_args, int *flag)
{
	int	n;
	int	i;

	n = 1;
	while (cmd_args[n] && cmd_args[n][0] == '-' && cmd_args[n][1] == 'n')
	{
		i = 1;
		if (cmd_args[n][0] == '-' && cmd_args[n][1] == 'n')
		{
			while (cmd_args[n][i] == 'n')
				i++;
			if (cmd_args[n][i] != '\0')
				return (n);
		}
		*flag = 1;
		n++;
	}
	return (n);
}


void echo(char **cmd_args, int fd_out)
{
    int i;
	int	flag;

	flag = 0;
    i = check_newline(cmd_args, &flag);
	if (cmd_args[1])
	{
		while (cmd_args[i])
		{
			if (cmd_args[i] && !cmd_args[i + 1])
				safe_write(fd_out, cmd_args[i], " ", NULL);
			else if (cmd_args[i])
				safe_write(fd_out, cmd_args[i], NULL);
			i++;
		}
	}
	if (!flag)
        safe_write(fd_out, "\n", NULL);
}

void	env(t_list *env_lst, int fd_out)
{
    int     exit_code;
    char    *key;
    char    *var;

	if (env_lst == NULL)
		return ;
	while (env_lst)
	{
        key = ((t_env *)(env_lst->content))->key;
        var = ((t_env *)(env_lst->content))->var;
        safe_write(fd_out, key, "=", var, "\n", NULL);
		env_lst = env_lst->next;
	}
}

void	print_export(t_list *env_lst, int fd_out)
{
    char    *key;
    char    *var;

	if (env_lst == NULL)
		return ;
	while (env_lst)
	{
        key = ((t_env *)(env_lst->content))->key;
        var = ((t_env *)(env_lst->content))->var;
        safe_write(fd_out, "export ", key, "=", var, "\n", NULL);
		env_lst = env_lst->next;
	}
}

void	add_env_list(char *arg, t_list *env_lst, int fd_out)
{
	int		i;
	char	*key;
	char	*var;

	if (!ft_isalpha(arg[0]))
		return (safe_write(1, "export: `", arg,"': not a valid identifier\n", NULL));
	i = 1;
	while (arg[i] && arg[i] != '=')
		i++;
	if (arg[i] != '=' || !arg[i + 1])
		return ;
	key = ft_substr(arg, 0, i);
	var = ft_strdup(arg + i + 1);
	// if (get_env_lst(key, env_lst))
	// {
	// 	set_env_value(key, var);
	// 	free(key);
	// 	free(var);
	// }
	// else
	// 	create_and_add_env_list(key, var, env_lst);
}

void	export(t_list *env_lst, char **cmd_args, int fd_out)
{
	int i;

	if (!cmd_args[1])
		print_export(env_lst, fd_out);
	i = 1;
	while (cmd_args[i])
	{
		add_env_list(cmd_args[i], env_lst, fd_out);
		i++;
	}
}
