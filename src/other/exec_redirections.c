/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/29 15:52:14 by fcouserg         ###   ########.fr       */
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

void	ft_init_fds(t_fds *fd)
{
	fd->input = -42;
	fd->output = -42;
	fd->pipes[0] = -42;
	fd->pipes[1] = -42;
	fd->redir[1] = -42;
	fd->in = -42;
	fd->savedpipe = -42;
}

void	set_redirs(t_fds *fd)
{
	if (fd->pipes[1] != -42)
		fd->redir[1] = fd->pipes[1];
	if (fd->input != -42)
	{
		if (fd->redir[0] != -42 && !fd->prevpipe)
			close(fd->redir[0]);
		if (fd->prevpipe)
			fd->savedpipe = fd->redir[0];
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
	if (fd->input != -42  && fd->input != -1)
		close(fd->input);
	if (copy_in->type == REDIR_IN)
	{
		fd->input = open(copy_in->redir_name, O_RDONLY);	
		if (fd->input == -1)
		{
			safe_write(2, "minishell: ", copy_in->redir_name, ": No such file or directory\n", NULL);
			minishell->excode = 1;
		}
	// checker message avec les free et tout le tralala	
	}
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

static void	exec_redirs_out(t_shell *minishell, t_redir *copy_out, t_fds *fd)
{
	if (fd->output != -42 && fd->output != -1)
		close(fd->output);
	if (copy_out->type == APPEND)
		fd->output = open(copy_out->redir_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (copy_out->type == REDIR_OUT)
		fd->output = open(copy_out->redir_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd->output == -1)
	{
		safe_write(2, "minishell: ", copy_out->redir_name, ": Permission denied\n", NULL);
		minishell->excode = 1;
	}
}

void	handle_redirs(t_shell *minishell, t_fds *fd, int i)
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

void	ft_pipes(t_shell *minishell, t_fds *fd, int i)
{		
	if (i > 0 && minishell->cmd_table[i - 1] != NULL) // j'ai ajoute i > 0 
		fd->prevpipe = true;
	else
		fd->prevpipe = false;
	if (i < minishell->count_pipes - 1)
	{
		if (pipe(fd->pipes) == -1)
			exitmsg(minishell, "pipe");
	}
}
