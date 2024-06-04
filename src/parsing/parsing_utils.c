/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:46:35 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/03 19:14:18 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_rev_neg_line(t_shell *minishell)
{
	int	i;

	i = 0;
	while (minishell->clean_line[i])
	{
		if (minishell->clean_line[i] < 0)
			minishell->clean_line[i] *= -1;
		i++;
	}
}

// void	ft_reverse_neg_in_word(t_shell *minishell, int i)
// {
// 	int	j;
// 	int	k;
// 	int	len;

// 	j = 0;
// 	k = 0;
// 	len = word_count(minishell->commands[i]->full_command, " ");
// 	minishell->commands[i]->split_command_clean = (char **)malloc(sizeof(char *)
// 			* len);
// 	if (minishell->commands[i]->split_command_clean == NULL)
// 	{
// 		// reset mini
// 	}
// 	while (j < len)
// 	{
// 		minishell->commands[i]->split_command_clean[j] = ft_calloc(ft_strlen(minishell->commands[i]->split_command[j])
// 				+ 1, sizeof(char));
// 	}
// 	if (minishell->commands == NULL)
// 	{
// 		// reset mini
// 	}
// 	while (minishell->line[i] != j && minishell->line[i])
// 	{
// 		minishell->clean_line[k] = minishell->line[i] * -1;
// 		i++;
// 		k++;
// 	}
// }


int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}


char	*ft_strndup(char *str, int n)
{
	char	*dest;
	int		i;

	i = 0;
	dest = malloc(sizeof(char) * (n + 1));
	if (!dest)
		return (NULL);
	while (str[i] && i < n)
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin_no_free(char *s1, char *s2)
{
	char	*join;
	size_t	len_join;

	if (!s1)
		return (NULL);
	len_join = ft_strlen(s1) + ft_strlen(s2) + 1;
	join = malloc(sizeof(char) * len_join);
	if (!join)
		return (NULL);
	ft_strlcpy(join, s1, len_join);
	ft_strlcat(join, s2, len_join);
	return (join);
}

