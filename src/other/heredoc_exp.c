/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 19:13:21 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/10/06 21:49:23 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_find_value_hd(char **to_join, char *str, t_shell *minishell, int i)
{
	int		j;
	char	*key;
	t_env	*env_lst;
	t_env	*cur_content;

	j = 0;
	while (ft_isalnum(str[i + j]) || str[i + j] == '_')
		j++;
	ft_check_strdup((str + i), j, &key, 1);
	env_lst = minishell->env;
	while (env_lst)
	{
		cur_content = env_lst;
		if (!ft_strcmp(key, cur_content->key))
		{
			if (*(str + i + j) != '\n')
				*to_join = ft_strjoin_no_free(cur_content->value, (str + i + j));
			else
				ft_check_strdup(cur_content->value, 0, to_join, 0);
			break ;
		}
		else
		{
			if (str + i + j)
				ft_check_strdup((str + i + j), 0, to_join, 0);
			else
				*to_join = NULL;
		}
		env_lst = env_lst->next;
	}
	free(key);
}

static char	*ft_dollar_option_hd(char *copy, char *str, int i, t_shell *minishell)
{
	char	*to_join;
	// char	*dup;

	// dup = ft_strdup(str);
	if (ft_isdigit(str[i + 1]) && str[i + 1] != '0')
	{
		to_join = ft_strdup(str + (i + 2));
		// if (mini... == NULL)
		// to be free;
	}
	else if (str[i + 1] == '0')
	{
		to_join = ft_strjoin_no_free("miniminishell_Flo_&_G", str + i + 2);
		// if (mini... == NULL)
		// to be free;
	}
	else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
	{
		ft_find_value_hd(&to_join, str, minishell, i + 1);

	}
	else if (str[i + 1] == '?')
	{
		to_join = ft_strjoin(ft_itoa(minishell->excode), str + i + 2);
		// if (mini... == NULL)
		// to be free;
	}
	else if ((str[i + 1] == '\"') || str[i + 1] == '\'')
	{
		to_join = ft_strdup(str + i + 1);
		// if (mini... == NULL)
		// to be free;
	}
	else
	{
		str[i] = str[i] * -1;
		to_join = ft_strdup(str + i);
		// if (mini... == NULL)
		// to be free;
	}
	if (to_join && copy)
	{
			// ft_printf("%s\n", to_join);
			// 		ft_printf("%s\n", copy);

		copy = ft_strjoin(copy, to_join);
		// ft_printf("%s\n", copy);
		// if (mini... == NULL)
		// to be free;
	}
	else
	{
		ft_check_strdup(to_join, 0, &copy, 0);
				// ft_printf("%s\n", copy);

	}
	// free(to_join);		
	free(str);
	return (copy);
}

char	*ft_expand_dollar_hd(char *str, t_shell *minishell)
{
	char	*copy;
	int		i;
	// char	*tmp;

	i = 0;
	copy = NULL;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (i > 0)
			{
				copy = ft_strndup(str, i);
				if (copy == NULL)
				{
					// reset
				}
			}
			copy = ft_dollar_option_hd(copy, str, i, minishell);
			i = 0;
		}
		else
			i++;
	}
	if (copy)
		return (copy);
	else
		return (str);
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
		exp = ft_expand_dollar_hd(str, minishell);
		// ft_printf("%s\n", exp);
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
