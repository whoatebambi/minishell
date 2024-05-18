/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:04:06 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/02/20 15:20:10 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static ssize_t	read_buffer(char *buf, int fd)
{
	ssize_t	len;

	if (*buf)
		return (1);
	len = read(fd, buf, BUFFER_SIZE);
	if (len < 0)
		return (0);
	if (!len)
		return (0);
	if (len < BUFFER_SIZE)
		buf[len] = '\0';
	return (1);
}

static size_t	find_new_line(char *buf, size_t *len)
{
	size_t	is_new_line;

	is_new_line = 0;
	while (buf[*len] && buf[*len] != '\n')
		(*len)++;
	if (buf[*len] == '\n')
		is_new_line = 1;
	*len += is_new_line;
	return (is_new_line);
}

static void	memmove_buffer(char *buf, size_t len)
{
	size_t	i;

	i = 0;
	while (len + i < BUFFER_SIZE)
	{
		buf[i] = buf[len + i];
		i++;
	}
	if (i < BUFFER_SIZE)
		buf[i] = '\0';
}

static char	*ft_strnjoin(char *result, char *buf, size_t n)
{
	size_t	i;
	size_t	size_r;
	char	*join;

	size_r = 0;
	if (result)
		size_r = ft_strlen((const char *)result);
	join = malloc((size_r + n + 1) * sizeof(char));
	if (!join)
		return (free(result), NULL);
	i = -1;
	while (++i < size_r)
		join[i] = result[i];
	i = -1;
	while (++i < n)
		join[i + size_r] = buf[i];
	join[i + size_r] = '\0';
	return (free(result), join);
}

char	*get_next_line_bonus(int fd)
{
	static char	buf[BUFFER_SIZE] = {0};
	char		*line;
	size_t		new_line;
	size_t		len;

	if (fd < 0)
		return (NULL);
	line = NULL;
	new_line = 0;
	while (!new_line && read_buffer((char *)buf, fd) > 0)
	{
		len = 0;
		new_line = find_new_line(buf, &len);
		line = ft_strnjoin(line, buf, len);
		if (!line)
			return (NULL);
		memmove_buffer(buf, len);
	}
	return (line);
}
