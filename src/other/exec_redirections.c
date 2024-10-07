/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/06 21:54:52 by codespace        ###   ########.fr       */
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
}

void	ft_init_fds(t_fds *fd)
{
	fd->input = -42;
	fd->in = -42;
	fd->output = -42;
	fd->pipes[0] = -42;
	fd->pipes[1] = -42;
	fd->redir[0] = -42; // Add this line
	fd->redir[1] = -42;
}

void	set_redirs(t_fds *fd)
{
	if (fd->pipes[1] != -42)
		fd->redir[1] = fd->pipes[1];
	if (fd->input != -42)
	{
		if (fd->redir[0] != -42)
			close(fd->redir[0]);
		fd->redir[0] = fd->input;
	}
	if (fd->output != -42)
	{
		if (fd->pipes[1] != -42)
			close(fd->pipes[1]);
		fd->redir[1] = fd->output;
	}
}

static void	exec_redirs_in(t_shell *minishell, t_redir *copy_in, t_fds *fd)
{
	if (fd->input != -42)
		close(fd->input);
	if (copy_in->type == REDIR_IN)
		fd->input = open(copy_in->redir_name, O_RDWR, 00755);	
	// if (fd->input == -1)
	// checker message avec les free et tout le tralala
	else if (copy_in->type == DELIMITER)
	{
		ft_sig_heredoc_setting();
		if (!copy_in->quote)
			ft_here_doc_exp(minishell, copy_in, fd);
		else
			ft_here_doc(minishell, copy_in, fd);
		ft_signals();
	}

}

static void	exec_redirs_out(t_redir *copy_out, t_fds *fd)
{
	if (fd->output != -42)
		close(fd->output);
	if (copy_out->type == APPEND)
		fd->output = open(copy_out->redir_name, O_RDWR | O_CREAT | O_APPEND,
				00755);
	else if (copy_out->type == REDIR_OUT)
		fd->output = open(copy_out->redir_name, O_RDWR | O_CREAT | O_TRUNC,
				00755);
	// if (cmd->fd_out == -1)
	// checker message avec les free et tout le tralala
}

void	exec_redirs(t_shell *minishell, t_fds *fd, int i)
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
		exec_redirs_out(copy_out, fd);
		copy_out = copy_out->next;
	}
}
