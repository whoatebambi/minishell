/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/07 13:11:42 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_arg)
{
	if (safe_strcmp(cmd_arg, "pwd") == 0)
		return (1);
	else if (safe_strcmp(cmd_arg, "echo") == 0)
		return (1);
	else if (safe_strcmp(cmd_arg, "envp") == 0)
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

void	replace_env_var(char *pwd, char *key, t_env *env)
{
	while (env)
	{
		if (safe_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(pwd);
		}
		env = env->next;
	}
}

void    cd(char **tab, t_env *env)
{
	char	old_pwd[1024];
    char	new_pwd[1024];
    int		excode;

	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
		return (perror("getcwd"));
    if (tab[1] == NULL)
		excode = chdir(getenv("HOME"));
	else if (safe_strcmp(tab[1], "-") == 0)
		excode = chdir(getenv("OLDPWD"));
	else
		excode = chdir(tab[1]);
    if (excode == -1)
		return (perror("chdir"));
	if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
		return (perror("getcwd"));
	replace_env_var(old_pwd, "OLDPWD", env);
	replace_env_var(new_pwd, "PWD", env);
	if (tab[1] && safe_strcmp(tab[1], "-") == 0)
		pwd(1);
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

void echo(char **tab, int fd_out)
{
    int i;
	int	flag;

	flag = 0;
    i = check_newline(tab, &flag);
	if (tab[1])
	{
		while (tab[i])
		{
			if (tab[i] && !tab[i + 1])
				safe_write(fd_out, tab[i], NULL);	
			else if (tab[i])
				safe_write(fd_out, tab[i], " ", NULL);
			i++;
		}
	}
	if (!flag)
        safe_write(fd_out, "\n", NULL);
}

void	envp(t_env *env, int fd_out)
{
    // int     excode;
    char    *key;
    char    *value;

	if (env == NULL)
		return ;
	while (env)
	{
        key = env->key;
        value = env->value;
        safe_write(fd_out, key, "=", value, "\n", NULL);
		env = env->next;
	}
}

void	print_export(t_env *env, int fd_out)
{
    char    *key;
    char    *value;

	if (env == NULL)
		return ;
	while (env)
	{
        key = env->key;
        value = env->value;
        safe_write(fd_out, "export ", key, "=", value, "\n", NULL);
		env = env->next;
	}
}

int	add_env_list(char *arg, t_env *env, int fd_out, t_shell *minishell)
{
	int		i;
	char	*key;
	char	*value;

	if (!ft_isalpha(arg[0]))
	{
		minishell->excode = 1;
		return (0);
	}
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == '-' || arg[i] == '+')
		{
			minishell->excode = 1;
			return (0);
		}
		i++;
	}
	if (arg[i] != '=' || !arg[i + 1])
	{
		minishell->excode = 0;
		return (0);
	}
		
	key = ft_substr(arg, 0, i);
	value = ft_strdup(arg + i + 1);
	// if (get_env_lst(key, env))
	// {
	// 	set_env_value(key, value);
	// 	free(key);
	// 	free(value);
	// }
	// else
	// 	create_and_add_env_list(key, value, env_lst);
	return (1);
}

void	export(t_env *env, char **tab, int fd_out, t_shell *minishell)
{
	int i;

	i = 1;
	if (!tab[i])
		print_export(env, fd_out);
	while (tab[i])
	{
		if (add_env_list(tab[i], env, fd_out, minishell) == 0)
		{
			if (minishell->excode != 0)
				safe_write(2, "export: `", tab[i],"': not a valid identifier\n", NULL);
			// safe_write(1, "export: `", arg,"': ", NULL);
			// minishell->excode = EINVAL;
			// errno = EINVAL;  // Set errno to a non-zero value
			exitmsg(minishell, NULL);
		}
			
		i++;
	}
}
