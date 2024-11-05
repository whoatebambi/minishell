/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exp_helpeur.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 19:13:21 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/05 00:43:20 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_quote_neg(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] < 0)
			str[i] *= -1;
	return (str);
}

char	*ft_joinfree(char *s1, char *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	free(s1);
	free(s2);
	return (str);
}

char	*ft_strdupfree(char *s)
{
	size_t	i;
	char	*str;

	if (!s)
	{
		str = malloc(sizeof(char));
		if (!str)
			return (NULL);
		str[0] = '\0';
		return (str);
	}
	else if (!s[0])
		str = malloc(sizeof(char));
	else
		str = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (free(s), str);
}

char	*num_true(char *line, int i)
{
	char	*copy;

	copy = ft_strndup(line, i - 1);
	if (!copy)
		return (NULL);
	if (line[i] == '0')
		return (ft_joinfree(copy, ft_jnf("bash", line + i + 1)));
	return (ft_strjoin(copy, line + i + 1));
}

char	*all_num_false(t_shell *minishell, char *line, int i)
{
	char	*ext;
	char	*copy;

	ext = ft_itoa(minishell->excode);
	if (!ext)
		return (NULL);
	copy = ft_strndup(line, i - 1);
	if (!copy)
		return (free(ext), NULL);
	if (line[i] == '?')
		return (ft_joinfree(copy, ft_strjoin(ext, line + i + 1)));
	else if (line[i] == '\'')
		return (free(ext), ft_strjoin(copy, line + i));
	else
		return (free(ext), ft_strjoin(copy, line + i - 1));
}
