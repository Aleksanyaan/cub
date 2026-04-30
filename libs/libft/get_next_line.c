/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:02:38 by zaleksan          #+#    #+#             */
/*   Updated: 2026/01/27 18:32:39 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

static char	*set_line(char *buffer)
{
	int		i;
	char	*line;

	i = 0;
	if (!buffer)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		line = (char *)malloc((i + 2) * sizeof(char));
	else
		line = (char *)malloc((i + 1) * sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*set_stash(char *buffer)
{
	int		i;
	int		j;
	char	*stash;

	i = 0;
	if (!buffer)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
		return (free(buffer), NULL);
	stash = (char *)malloc((ft_strlen_gnl(buffer) - i) * sizeof(char));
	if (!stash)
		return (NULL);
	j = 0;
	while (buffer[i] && buffer[++i])
		stash[j++] = buffer[i];
	stash[j] = '\0';
	free(buffer);
	return (stash);
}

char	*read_file(int fd, char *buffer)
{
	char		*line_buffer;
	ssize_t		bytesread;

	line_buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!line_buffer)
		return (NULL);
	while (1)
	{
		bytesread = read(fd, line_buffer, BUFFER_SIZE);
		if (bytesread == -1)
		{
			free(line_buffer);
			free(buffer);
			return (NULL);
		}
		line_buffer[bytesread] = '\0';
		if (bytesread == 0)
			break ;
		buffer = ft_strjoin_gnl(buffer, line_buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free(line_buffer);
	return (buffer);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buffer = NULL;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free(buffer), NULL);
	buffer = read_file(fd, buffer);
	if (!buffer)
		return (NULL);
	line = set_line(buffer);
	buffer = set_stash(buffer);
	if (line && !line[0])
	{
		free(line);
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	return (line);
}
