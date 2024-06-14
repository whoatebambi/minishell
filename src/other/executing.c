/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/14 17:27:49 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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


void	exec_in_child(t_shell *minishell, int i)
{
	
	// fork() here 
	// set signals
	// if child pid = 0 do under
	printf("///// exec_in_child()\n");
	if (is_builtin(minishell->cmd_table[i]->cmd_args[0]))
		execute_builtin(minishell->cmd_table[i], minishell->env_lst);
	else
		exec_system(minishell, i);
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
		if (is_builtin(minishell->cmd_table[i]->cmd_args[0]) && nb_commands == 1)
			execute_builtin(minishell->cmd_table[i], minishell->env_lst); 
		// otherwise fork and then check
		else
			exec_in_child(minishell, i);       
        if (minishell->cmd_table[i]->fd_out != 1)
            close(minishell->cmd_table[i]->fd_out);
		i++;
	}
}
