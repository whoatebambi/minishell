/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 02:14:54 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_write(int fd, const char *str, ...)
{
	va_list		args;
	const char	*current_str;
	int			bytes_written;
	size_t		i;

	va_start(args, str);
	current_str = str;
	while (current_str)
	{
		i = 0;
		while (i < ft_strlen(current_str))
		{
			bytes_written = write(fd, &current_str[i], 1);
			if (bytes_written == -1)
			{
				perror("write");
				va_end(args);
				return ;
			}
			i++;
		}
		current_str = va_arg(args, const char *);
	}
	va_end(args);
}

char	*safe_join_envp(char *key, char *symb, char *value)
{
	char	*str;
	int		i;
	int		len;

	i = 0;
	if (!key || !symb)
		return (NULL);
	len = ft_strlen(key) + ft_strlen(symb) + ft_strlen(value) + 1;
	str = ft_calloc(len, sizeof(char));
	if (!str)
		return (NULL);
	while (*key)
		str[i++] = *key++;
	while (*symb)
		str[i++] = *symb++;
	if (value)
	{
		while (*value)
			str[i++] = *value++;
	}
	str[i] = '\0';
	return (str);
}

void	ft_perror(t_shell *minishell, char *word, char *msg, char *third)
{
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		minishell->excode = 1;
		(perror("dup"), exitmsg(minishell, NULL));
	}
	if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1)
	{
		minishell->excode = 1;
		(close(saved_stdout), perror("dup2"), exitmsg(minishell, NULL));
	}
	if (third)
		printf("minishell: %s: %s: %s\n", word, msg, third);
	else
		printf("minishell: %s: %s\n", word, msg);
	if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1)
	{
		minishell->excode = 1;
		(close(saved_stdout), perror("dup2"), exitmsg(minishell, NULL));
	}
	close(saved_stdout);
}

int	env_size(t_shell *minishell)
{
	t_env	*node;
	int		len;

	len = 0;
	node = minishell->env;
	while (node)
	{
		if (node->key)
			len++;
		node = node->next;
	}
	return (len);
}

void	exitmsg(t_shell *minishell, char *errmsg)
{
	int	ext;

	ext = minishell->excode;
	if (errmsg)
		perror(errmsg);
	free_minishell(minishell);
	exit(ext);
}
