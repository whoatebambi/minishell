/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 01:27:13 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_fds *fd)
{
	if (!(fd->redir[0] == -42) && fd->redir[0] >= 0)
		close(fd->redir[0]);
	if (!(fd->redir[1] == -42) && fd->redir[1] >= 0)
		close(fd->redir[1]);
	if (fd->input != -42 && fd->input >= 0)
		close(fd->input);
	if (fd->output != -42 && fd->output >= 0)
		close(fd->output);
	if (!(fd->pipes[0] == -42) && fd->pipes[0] >= 0)
		close(fd->pipes[0]);
	if (!(fd->pipes[1] == -42) && fd->pipes[1] >= 0)
		close(fd->pipes[1]);
	if (!(fd->savedpipe == -42) && fd->savedpipe >= 0)
		close(fd->savedpipe);
}

void	init_struct_fd(t_fds *fd)
{
	fd->pipes[0] = -42;
	fd->pipes[1] = -42;
	fd->redir[1] = -42;
	fd->in = -42;
	fd->input = -42;
	fd->output = -42;
	fd->savedpipe = -42;
}

void	exec_redirs_in(t_shell *minishell, t_redir *copy_in, t_fds *fd)
{
	if (fd->input != -42 && fd->input != -1)
		close(fd->input);
	if (copy_in->type == REDIR_IN)
	{
		fd->input = open(copy_in->redir_name, O_RDONLY);
		if (fd->input == -1)
		{
			safe_write(2, "minishell: ", copy_in->redir_name,
				": No such file or directory\n", NULL);
			minishell->excode = 1;
		}
	}
	else if (copy_in->type == DELIMITER)
	{
		set_signal_heredoc();
		if (!copy_in->quote)
			exec_here_doc_exp(minishell, copy_in, fd);
		else
			exec_here_doc(minishell, copy_in, fd);
		set_signals();
	}
}

void	exec_redirs_out(t_shell *minishell, t_redir *copy_out, t_fds *fd)
{
	if (fd->output != -42 && fd->output != -1)
		close(fd->output);
	if (copy_out->type == APPEND)
		fd->output = open(copy_out->redir_name,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (copy_out->type == REDIR_OUT)
		fd->output = open(copy_out->redir_name,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd->output == -1)
	{
		safe_write(2, "minishell: ", copy_out->redir_name,
			": Permission denied\n", NULL);
		minishell->excode = 1;
	}
}

void	init_redirections(t_shell *minishell, t_fds *fd, int i)
{
	t_redir	*copy_in;
	t_redir	*copy_out;

	copy_in = minishell->cmd_table[i]->redirs_in;
	copy_out = minishell->cmd_table[i]->redirs_out;
	while (copy_in)
	{
		exec_redirs_in(minishell, copy_in, fd);
		copy_in = copy_in->next;
	}
	while (copy_out)
	{
		exec_redirs_out(minishell, copy_out, fd);
		copy_out = copy_out->next;
	}
}
