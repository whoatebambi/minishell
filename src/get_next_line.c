/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 14:32:17 by fcouserg          #+#    #+#             */
/*   Updated: 2023/12/19 17:09:28 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*clean_save(char *save)
{
	char	*update;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(save);
	while (save[i] && save[i] != '\n')
		i++;
	if (save[i] == '\0')
	{
		free(save);
		return (NULL);
	}
	update = ft_strndup((save + i + 1), (len - i));
	free(save);
	save = NULL;
	return (update);
}

char	*get_line(char *save)
{
	char	*line;
	int		i;

	if (!save)
		return (NULL);
	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	if (save[i] == '\n')
		line = ft_strndup(save, i + 1);
	else
		line = ft_strndup(save, i);
	return (line);
}

char	*read_and_save(char *save, int fd)
{
	char	*buf;
	int		count;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	count = 1;
	if (!save)
	{
		save = malloc(sizeof(char) * 1);
		if (!save)
			return (NULL);
		save[0] = '\0';
	}
	while ((!ft_strchr(save, '\n') && count != 0))
	{
		count = read(fd, buf, BUFFER_SIZE);
		if (count == -1)
			return (free(buf), free(save), NULL);
		buf[count] = '\0';
		save = ft_strjoin(save, buf);
	}
	free(buf);
	return (save);
}

char	*get_next_line(int fd)
{
	static char	*save[4095];
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	save[fd] = read_and_save(save[fd], fd);
	if (!save[fd] || save[fd][0] == 0)
	{
		free(save[fd]);
		save[fd] = NULL;
		return (NULL);
	}
	line = get_line(save[fd]);
	if (!line || line[0] == 0)
		return (free(line), free(save[fd]), NULL);
	save[fd] = clean_save(save[fd]);
	if (save[fd] && save[fd][0] == 0)
	{
		free(save[fd]);
		save[fd] = NULL;
	}
	return (line);
}
