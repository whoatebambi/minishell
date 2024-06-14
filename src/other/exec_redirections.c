/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/14 16:38:11 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
