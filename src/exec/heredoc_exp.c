/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 19:13:21 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/05 01:40:17 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_dollar_hd(t_shell *minishell, char *line, int *i, t_env *node)
{
	char	*key;
	int		j;
	char	*cp;

	j = (*i) + 1;
	if (ft_isdigit(line[j]))
		return (num_true(line, j));
	if (!ft_isalnum(line[j]) && line[j] != '_')
	{
		line[(*i)] *= -1;
		return (all_num_false(minishell, line, j));
	}
	while (ft_isalnum(line[j]) || line[j] == '_')
		j++;
	key = ft_strndup(line + *i + 1, j - *i - 1);
	if (!key)
		return (NULL);
	cp = ft_strndup(line, *i);
	while (node)
	{
		if (!safe_strcmp(node->key, key))
			return (free(key), ft_joinfree(cp, ft_jnf(node->value, line + j)));
		node = node->next;
	}
	return (free(key), ft_strjoin(cp, line + j));
}

char	*ft_expand_heredoc(t_shell *mn, char *line, int i)
{
	char	*tmp;
	char	*copy;

	tmp = ft_strdup(line);
	if (!tmp)
		(free(line), exitmsg(mn, "Malloc error"));
	copy = ft_strdup(line);
	if (!copy)
		(free(line), free(tmp), exitmsg(mn, "Malloc error"));
	while (tmp[++i])
	{
		if (tmp[i] == '$')
		{
			free(copy);
			copy = ft_strdupfree(ft_dollar_hd(mn, tmp, &i, mn->env));
			if (!copy)
				return (free(tmp), NULL);
			free(tmp);
			tmp = ft_strdup(copy);
			if (!tmp)
				exitmsg(mn, "Malloc error");
			i = -1;
		}
	}
	return (free(line), free(tmp), ft_quote_neg(copy), ft_strdupfree(copy));
}

void	prep_heredoc_exp(t_shell *minishell, t_fds *fd,
	char **heredoc, int *tmp)
{
	*heredoc = ft_strdup("/tmp/.here_doc_a");
	if (!(*heredoc))
		exitmsg(minishell, "Malloc error");
	*heredoc = ft_find_tmp_heredoc(minishell, *heredoc);
	*tmp = open(*heredoc, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	fd->input = open(*heredoc, O_RDONLY);
	fd->in = dup(STDIN_FILENO);
}

int	exec_here_doc_exp(t_shell *minishell, t_redir *copy_in, t_fds *fd)
{
	int		tmp;
	char	*line;
	char	*exp;
	char	*heredoc;

	prep_heredoc_exp(minishell, fd, &heredoc, &tmp);
	line = readline("> ");
	while (line && safe_strcmp(copy_in->redir_name, line) == 1)
	{
		exp = ft_expand_heredoc(minishell, line, -1);
		if (!exp)
			(free(line), free(heredoc), close(tmp));
		write(tmp, exp, ft_strlen(exp));
		write(tmp, "\n", 1);
		free(exp);
		line = readline("> ");
	}
	ft_ctrlc(minishell, fd, heredoc);
	close(tmp);
	free(line);
	free(heredoc);
	close(fd->in);
	return (130);
}
