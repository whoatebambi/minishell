/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/28 23:42:10 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// This is a singleton. The concept is to create an object once
// and then be able to access it any time while not using global variable
// thanks to a stored pointer. (from Ophelie)
t_shell	*get_minishell(t_shell *minishell)
{
	static t_shell	*pointer_to_minishell = NULL;

	if (!pointer_to_minishell && minishell)
		pointer_to_minishell = minishell;
	return (pointer_to_minishell);
}

void safe_write(int fd, const char *str, ...)
{
    va_list     args;
    const char *current_str;
    int         bytes_written;
    size_t      i;
    
    va_start(args, str);
    current_str = str;
    while (current_str)
    {
        i = 0;
        while (i < strlen(current_str))
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

int	safe_strcmp(char *s1, char *s2)
{
	// int	i;

	// i = 0;
	// if (ft_strlen(s1) != ft_strlen(s2))
	// 	return (1);
	// while (s1[i] != '\0' || s2[i] != '\0')
	// {
	// 	if (s1[i] != s2[i])
	// 		return (1);
	// 	i++;
	// }
	// return (0);
	if (!s1 || !s2)
		return (1);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (1);
		s1++;
		s2++;
	}
	if (*s1 || *s2)
		return (1);
	return (0);
}



void swap_env(t_list *a, t_list *b)
{
    void *temp_content;

    temp_content = a->content;
    a->content = b->content;
    b->content = temp_content;
}

void	ft_exit_msg(t_shell *minishell, char *errmsg)
{
	int	ext;

	ext = minishell->excode;
	if (errmsg)
		perror(errmsg);
	free_minishell(minishell);
	exit(ext);
}

char	*safe_join_envp(char *key, char *symb, char *value)
{
	char	*str;
	int i;
	int len;

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

void	ft_perror(t_shell *shell, char *word, char *msg, char *third)
{
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		shell->excode = 1;
		(perror("dup"), exitmsg(shell, NULL));
	}
	if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1)
	{
		shell->excode = 1;
		(close(saved_stdout), perror("dup2"), exitmsg(shell, NULL));
	}
	if (third)
		printf("minishell: %s: %s: %s\n", word, msg, third);
	else
		printf("minishell: %s: %s\n", word, msg);
	if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1)
	{
		shell->excode = 1;
		(close(saved_stdout), perror("dup2"), exitmsg(shell, NULL));
	}
	close(saved_stdout);
}