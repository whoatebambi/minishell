/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:31:47 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/10/06 17:29:32 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_ctrlc(t_shell *minishell, t_fds *fd, char *heredoc)
{
	if (g_sig == SIGINT)
	{
		minishell->excode = 130;
		g_sig = 0;
		close(fd->input);
		dup2(fd->in, STDIN_FILENO);
		unlink(heredoc);
		minishell->tmpexcode = 130;
	}
}

char*	ft_find_tmp_heredoc(t_shell *minishell, char* heredoc)
{
	while (access(heredoc, F_OK) == 0)
		heredoc = ft_strjoin(heredoc, "a");
	if (!heredoc)
		ft_exit_msg(minishell, MERROR);
	return(heredoc);
}

int	ft_here_doc(t_shell *minishell, t_redir *copy_in, t_fds *fd)
{
	int		tmp;
	char	*str;
	char *heredoc;

	heredoc = ft_strdup("/tmp/.tmp_heredoc");
	if (!heredoc)
		ft_exit_msg(minishell, MERROR);
	heredoc = ft_find_tmp_heredoc(minishell, heredoc);
	tmp = open(heredoc, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	fd->input = open(heredoc, O_RDONLY);
	fd->in = dup(STDIN_FILENO);
	str = readline("> ");
	while (str && safe_strcmp(copy_in->redir_name, str) == 1)
	{
		write(tmp, str, ft_strlen(str));
		write(tmp, "\n", 1);
		free(str);
		str = readline("> ");
	}
	ft_ctrlc(minishell, fd, heredoc);
	return (close(tmp), free(str), free(heredoc), close(fd->in), 130);
}
