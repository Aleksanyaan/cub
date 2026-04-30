/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 15:21:05 by pargev            #+#    #+#             */
/*   Updated: 2026/01/28 20:14:16 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_file_extension(char *path, char *extension)
{
	int	path_len;
	int	extension_len;

	path_len = ft_strlen(path);
	extension_len = ft_strlen(extension);
	if (path_len < extension_len + 2)
		return (0);
	if (!ft_strncmp(path + path_len - extension_len, extension, extension_len))
	{
		if (path[path_len - extension_len - 1] == '.'
			&& !ft_strrchr("\\/:*?\"<>|", path[path_len - extension_len - 2]))
			return (1);
	}
	return (0);
}

int	check_symbol(char **map, int i, int j)
{
	if (i >= 0 && j >= 0 && map[i] && map[i][j])
	{
		if (map[i][j] != ' ' && map[i][j] != '1')
			return (1);
	}
	return (0);
}

int	check_border(char **map, int i, int j)
{
	if (check_symbol(map, i - 1, j)
		|| check_symbol(map, i - 1, j - 1)
		|| check_symbol(map, i - 1, j + 1)
		|| check_symbol(map, i, j - 1)
		|| check_symbol(map, i, j + 1)
		|| check_symbol(map, i + 1, j)
		|| check_symbol(map, i + 1, j - 1)
		|| check_symbol(map, i + 1, j + 1))
		return (1);
	return (0);
}

int	check_map(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == ' ')
			{
				if (check_border(map, i, j))
					return (1);
			}
			else if ((i == 0 || j == 0 || !map[i + 1] || !map[i][j + 1]))
				if (map[i][j] != '1')
					return (1);
			j++;
		}
		i++;
	}
	return (0);
}

void	cheack_config(t_config *config)
{
	if (!config->north_texture)
		free_and_exit(config, NULL, ": <north texture> is missing or invalid");
	if (!config->south_texture)
		free_and_exit(config, NULL, ": <south texture> is missing or invalid");
	if (!config->west_texture)
		free_and_exit(config, NULL, ": <west texture> is missing or invalid");
	if (!config->east_texture)
		free_and_exit(config, NULL, ": <east texture> is missing or invalid");
	if (!config->floor_color)
		free_and_exit(config, NULL, ": <floor color> is missing or invalid");
	if (!config->ceiling_color)
		free_and_exit(config, NULL, ": <ceiling color> is missing or invalid");
	if (!config->map || check_map(config->map))
		free_and_exit(config, NULL, ": <map> is missing> or invalid");
}
