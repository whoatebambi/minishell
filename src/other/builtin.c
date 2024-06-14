/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/14 16:11:40 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_arg)
{
	if (ft_strncmp(cmd_arg, "pwd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd_arg, "echo", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd_arg, "env", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd_arg, "export", 6) == 0)
		return (1);
	else
		return (0);
}

void	pwd(int fd_out)
{
	char	*buffer;

	// TD char buffer[PATH_MAX] // # define PATH_MAX  4096
	buffer = NULL;
	buffer = getcwd(buffer, 4096);
	if (buffer == NULL)
	{
		perror("Error");
		free(buffer);
		return ;
	}
	ft_putstr_fd(buffer, fd_out);
	write(fd_out, "\n", 1);
	free(buffer);
}

void	echo(char **cmd_args, int fd_out)
{
	int	i;

	i = 1;
	while (cmd_args[i])
	{
		ft_putstr_fd(cmd_args[i], fd_out);
		if (cmd_args[i + 1])
			write(fd_out, " ", 1);
		i++;
	}
	// TD (flag == '-n')
	write(fd_out, "\n", 1);
}

void	env(t_list *env_lst, int fd_out)
{
	if (env_lst == NULL)
		return ;
	while (env_lst)
	{
		ft_putstr_fd(((t_env *)(env_lst->content))->key, fd_out);
		write(fd_out, "=", 1);
		ft_putstr_fd(((t_env *)(env_lst->content))->var, fd_out);
		write(fd_out, "\n", 1);
		env_lst = env_lst->next;
	}
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

void	print_export(t_list *env_lst, int fd_out)
{
	if (env_lst == NULL)
		return ;
	while (env_lst)
	{
		ft_putstr_fd("export ", fd_out);
		ft_putstr_fd(((t_env *)(env_lst->content))->key, fd_out);
		write(fd_out, "=", 1);
		ft_putstr_fd(((t_env *)(env_lst->content))->var, fd_out);
		write(fd_out, "\n", 1);
		env_lst = env_lst->next;
	}
}

void	export(t_list *env_lst, t_cmd_table *cmd_table, int fd_out)
{
	t_list	*sorted_env_lst;
	int		i;

	i = 0;
	if (cmd_table->cmd_args[1] == 0)
	{
		sorted_env_lst = dup_env_lst(env_lst);
		// sort alphabetically
		print_export(sorted_env_lst, fd_out);
		// clean
	}
}
