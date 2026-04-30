/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 15:21:05 by pargev            #+#    #+#             */
/*   Updated: 2026/01/28 20:12:47 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	config_lines_count(char *path)
{
	int		fd;
	int		len;
	char	*line;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		exit_with_error(strerror(errno));
	len = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		len++;
		free(line);
	}
	close(fd);
	return (len);
}

char	**read_config(char *path)
{
	int		fd;
	char	**config;
	char	*line;
	int		i;

	config = (char **)malloc(sizeof(char *) * (config_lines_count(path) + 1));
	if (!config)
		exit_with_error(strerror(errno));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		exit_with_error(strerror(errno));
	i = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		config[i++] = line;
	}
	config[i] = 0;
	close(fd);
	return (config);
}
