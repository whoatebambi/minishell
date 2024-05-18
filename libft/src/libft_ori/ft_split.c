/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 11:54:24 by guillaumebe       #+#    #+#             */
/*   Updated: 2024/02/19 19:04:58 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static size_t	word_count(char const *s, char c)
{
	size_t	wc;
	size_t	i;

	wc = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			wc++;
		i++;
	}
	return (wc);
}

static void	fill_array(char *new, char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
}

// static void	mem_set(char **tab, char const *s, char c)
// {
// 	size_t	count;
// 	size_t	index;
// 	size_t	i;

// 	index = 0;
// 	i = 0;
// 	while (s[index])
// 	{
// 		count = 0;
// 		while (s[index + count] && s[index + count] != c)
// 			count++;
// 		if (count > 0)
// 		{
// 			tab[i] = malloc(sizeof(char) * (count + 1));
// 			if (!tab[i])
// 				return (free_tab(tab));
// 			fill_array(tab[i], (s + index), c);
// 			i++;
// 			index = index + count;
// 		}
// 		else
// 			index++;
// 	}
// 	tab[i] = NULL;
// }

static char	**mem_set(char **tab, char const *s, char c)
{
	size_t	count;
	size_t	index;
	size_t	i;

	index = 0;
	i = 0;
	while (s[index])
	{
		count = 0;
		while (s[index + count] && s[index + count] != c)
			count++;
		if (count > 0)
		{
			tab[i] = malloc(sizeof(char) * (count + 1));
			if (!tab[i])
				return (free_tab(tab), NULL);
			fill_array(tab[i], (s + index), c);
			i++;
			index = index + count;
		}
		else
			index++;
	}
	tab[i] = NULL;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**tab;

	if (!s)
		return (NULL);
	words = word_count(s, c);
	tab = malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	tab = mem_set(tab, s, c);
	if (!tab)
		return (NULL);
	return (tab);
}
