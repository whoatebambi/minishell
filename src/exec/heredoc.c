/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:31:47 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/05 01:37:21 by codespace        ###   ########.fr       */
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

char	*ft_find_tmp_heredoc(t_shell *minishell, char *heredoc)
{
	while (access(heredoc, F_OK) == 0)
		heredoc = ft_strjoin(heredoc, "a");
	if (!heredoc)
		exitmsg(minishell, "Malloc error");
	return (heredoc);
}

void	prep_heredoc(t_shell *minishell, t_fds *fd, char **heredoc, int *tmp)
{
	*heredoc = ft_strdup("/tmp/.tmp_heredoc");
	if (!(*heredoc))
		exitmsg(minishell, "Malloc error");
	*heredoc = ft_find_tmp_heredoc(minishell, *heredoc);
	*tmp = open(*heredoc, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	fd->input = open(*heredoc, O_RDONLY);
	fd->in = dup(STDIN_FILENO);
}

int	exec_here_doc(t_shell *minishell, t_redir *copy_in, t_fds *fd)
{
	int		tmp;
	char	*line;
	char	*heredoc;

	prep_heredoc(minishell, fd, &heredoc, &tmp);
	line = readline("> ");
	while (line && safe_strcmp(copy_in->redir_name, line) == 1)
	{
		write(tmp, line, ft_strlen(line));
		write(tmp, "\n", 1);
		free(line);
		line = readline("> ");
	}
	ft_ctrlc(minishell, fd, heredoc);
	return (close(tmp), free(line), free(heredoc), close(fd->in), 130);
}
