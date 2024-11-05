/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_redirections.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/04 23:54:18 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
