/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 19:13:21 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/04 20:01:44 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_is_numerique(char *str, int j)
{
	char	*copy;

	copy = ft_strndup(str, j - 1);
	if (!copy)
		return (NULL);
	if (str[j] == '0')
		return (ft_joinfree(copy, ft_jnf("bash", str + j + 1)));
	return (ft_strjoin(copy, str + j + 1));
}

static char	*is_not_alnumm(t_shell *shell, char *str, int j)
{
	char	*ext;
	char	*copy;

	ext = ft_itoa(shell->excode);
	if (!ext)
		return (NULL);
	copy = ft_strndup(str, j - 1);
	if (!copy)
		return (free(ext), NULL);
	if (str[j] == '?')
		return (ft_joinfree(copy, ft_strjoin(ext, str + j + 1)));
	else if (str[j] == '\'')
		return (free(ext), ft_strjoin(copy, str + j));
	else
		return (free(ext), ft_strjoin(copy, str + j - 1));
}

static char	*ft_dollar_hd(t_shell *shell, char *str, int *i, t_env *node)
{
	char	*key;
	int		j;
	char	*copy;

	j = (*i) + 1;
	if (ft_isdigit(str[j]))
		return (ft_is_numerique(str, j));
	if (!ft_isalnum(str[j]) && str[j] != '_')
	{
		str[(*i)] *= -1;
		return (is_not_alnumm(shell, str, j));
	}
	while (ft_isalnum(str[j]) || str[j] == '_')
		j++;
	key = ft_strndup(str + *i + 1, j - *i - 1);
	if (!key)
		return (NULL);
	copy = ft_strndup(str, *i);
	while (node)
	{
		if (!safe_strcmp(node->key, key))
			return (free(key), ft_joinfree(copy, ft_jnf(node->value, str + j)));
		node = node->next;
	}
	return (free(key), ft_strjoin(copy, str + j));
}

static char	*ft_expand_heredoc(t_shell *shell, char *str, int i)
{
	char	*tmp;
	char	*copy;

	tmp = ft_strdup(str);
	if (!tmp)
		(free(str), exitmsg(shell, MERROR));
	copy = ft_strdup(str);
	if (!copy)
		(free(str), free(tmp), exitmsg(shell, MERROR));
	while (tmp[++i])
	{
		if (tmp[i] == '$')
		{
			free(copy);
			copy = ft_strdupfree(ft_dollar_hd(shell, tmp, &i, shell->env));
			if (!copy)
				return (free(tmp), NULL);
			free(tmp);
			tmp = ft_strdup(copy);
			if (!tmp)
				exitmsg(shell, MERROR);
			i = -1;
		}
	}
	return (free(str), free(tmp), ft_quote_neg(copy), ft_strdupfree(copy));
}

int	ft_here_doc_exp(t_shell *minishell, t_redir *copy_in, t_fds *fd)
{
	int		tmp;
	char	*str;
	char	*exp;
	char	*heredoc;

	heredoc = ft_strdup("/tmp/.here_doc_a");
	if (!heredoc)
		ft_exit_msg(minishell, MERROR);
	heredoc = ft_find_tmp_heredoc(minishell, heredoc);
	tmp = open(heredoc, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	fd->input = open(heredoc, O_RDONLY);
	fd->in = dup(STDIN_FILENO);
	str = readline("> ");
	while (str && safe_strcmp(copy_in->redir_name, str) == 1)
	{
		exp = ft_expand_heredoc(minishell, str, -1);
		if (!exp)
			(free(str), free(heredoc), close(tmp));
		write(tmp, exp, ft_strlen(exp));
		write(tmp, "\n", 1);
		free(exp);
		str = readline("> ");
	}
	ft_ctrlc(minishell, fd, heredoc);
	return (close(tmp), free(str), free(heredoc), close(fd->in), 130);
}
