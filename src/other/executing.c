/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/13 20:11:16 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	execute_builtin(t_cmd_table *cmd_table, t_list *env_lst)
{
	if (ft_strncmp(cmd_table->cmd_args[0], "pwd", 3) == 0) //&& ft_strlen(command->commands) == 3)
		pwd(cmd_table->fd_out);
	else if (ft_strncmp(cmd_table->cmd_args[0], "echo", 4) == 0)
		echo(cmd_table->cmd_args, cmd_table->fd_out);
	else if (ft_strncmp(cmd_table->cmd_args[0], "env", 3) == 0)
		env(env_lst, cmd_table->fd_out);
	else if (ft_strncmp(cmd_table->cmd_args[0], "export", 6) == 0)
		export(env_lst, cmd_table, cmd_table->fd_out);
}

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

void	exec_redirs_in(t_cmd_table *cmd, t_redir *redir_out)
{
	if (redir_out->type == APPEND)
	{
		cmd->fd_out = open(redir_out->redir_name, O_RDWR | O_CREAT | O_APPEND,
				00755);
		if (redir_out->next)
			close(cmd->fd_out);
	}
	else if (redir_out->type == REDIR_OUT)
		cmd->fd_out = open(redir_out->redir_name, O_RDWR | O_CREAT | O_TRUNC,
				00755);
	if (cmd->fd_out == -1)
		return ;
}

void	exec_redirs_out(t_cmd_table *cmd, t_redir *redir_out)
{
	if (redir_out->type == APPEND)
	{
		cmd->fd_out = open(redir_out->redir_name, O_RDWR | O_CREAT | O_APPEND,
				00755);
		if (redir_out->next)
			close(cmd->fd_out);
	}
	else if (redir_out->type == REDIR_OUT)
	{
		cmd->fd_out = open(redir_out->redir_name, O_RDWR | O_CREAT | O_TRUNC,
				00755);
		if (redir_out->next)
			close(cmd->fd_out);
	}
	// if (cmd->fd_out == -1)
		//checker message avec les free et tout le tralala
}

void	exec_redirs(t_cmd_table *cmd, t_redir *redir_in, t_redir *redir_out)
{
	// while (redir_in)
	// {
	// 	if (redir_in->type == REDIR_IN)
	// 	{
	// 		printf("REDIR_IN\n");
	// 		exec_redirs_in(cmd, redir_out);
	// 	}
	// 	redir_out = redir_out->next;
	// }
	while (redir_out)
	{
		exec_redirs_out(cmd, redir_out);
		redir_out = redir_out->next;
	}
}

void	execute(t_shell *minishell, char *line)
{
	int	i;
	int	nb_commands;

	i = 0;
	nb_commands = minishell->count_pipes;
	// THIS IS THE PARENT
	while (i < nb_commands)
	{
		// pipe
		if (i < nb_commands - 1)
		{
			if (pipe(minishell->cmd_table[i]->fd_pipe) == -1)
			{
				perror("pipe");
				exit(1);
			}
		}
		// open file/redir
		if (minishell->cmd_table[i]->redirs_in || minishell->cmd_table[i]->redirs_out)
		{
			exec_redirs(minishell->cmd_table[i], minishell->cmd_table[i]->redirs_in, minishell->cmd_table[i]->redirs_out);
		}
        else
        {
            minishell->cmd_table[i]->fd_in = 0;
            minishell->cmd_table[i]->fd_out = 1;
        }
		// set fds
		// if builtin vs execve
		// if one command that is builtin no fork
		// otherwise fork and then check
		if (is_builtin(minishell->cmd_table[i]->cmd_args[0]))
			execute_builtin(minishell->cmd_table[i], minishell->env_lst);       
        if (minishell->cmd_table[i]->fd_out != 1)
            close(minishell->cmd_table[i]->fd_out);
		i++;
	}
}
