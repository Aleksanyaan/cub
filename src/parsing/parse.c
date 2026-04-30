/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 15:21:05 by pargev            #+#    #+#             */
/*   Updated: 2026/02/21 14:28:43 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	**parse_map(char **config_text, int start)
{
	int		len;
	char	**map;
	int		i;

	len = start;
	while (config_text[len])
		len++;
	map = (char **)ft_calloc(sizeof(char *), len + 1);
	i = 0;
	while (config_text[start + i])
	{
		if (config_text[start + i][0] == '\n'
			|| !ft_str_only(config_text[start + i], " \n102NWESD"))
		{
			free_string_array(map);
			return (NULL);
		}
		map[i] = ft_strdup_free(ft_strtrim(config_text[start + i], "\n"));
		i++;
	}
	return (map);
}

int	parse_textures(char *info, t_config *config)
{
	if (ft_strnstr(info, "NO", 2) == info)
		config->north_texture = ft_strtrim(info + 2, " \n");
	else if (ft_strnstr(info, "SO", 2) == info)
		config->south_texture = ft_strtrim(info + 2, " \n");
	else if (ft_strnstr(info, "WE", 2) == info)
		config->west_texture = ft_strtrim(info + 2, " \n");
	else if (ft_strnstr(info, "EA", 2) == info)
		config->east_texture = ft_strtrim(info + 2, " \n");
	else
		return (0);
	return (1);
}

int	parse_config(char **config_text, t_config *config)
{
	int		i;
	char	*info;

	i = 0;
	while (config_text[i])
	{
		info = config_text[i];
		if (info[0] == '\n' || parse_textures(info, config))
			;
		else if (ft_strnstr(info, "F", 1) == info)
			config->floor_color = parse_color(ft_strtrim(info + 1, " \n"));
		else if (ft_strnstr(info, "C", 1) == info)
			config->ceiling_color = parse_color(ft_strtrim(info + 1, " \n"));
		else if (ft_str_only(info, " \n102NWESD"))
		{
			config->map = parse_map(config_text, i);
			break ;
		}
		else
			return (1);
		i++;
	}
	return (0);
}

t_config	parse(char *path)
{
	char		**config_text;
	t_config	config;

	if (!check_file_extension(path, "cub"))
		exit_with_error(": configuration's extension must be .cub");
	config_text = read_config(path);
	if (parse_config(config_text, &config))
	{
		free_and_exit(&config, config_text,
			": configuration contains an invalid identifier");
	}
	free_string_array(config_text);
	cheack_config(&config);
	return (config);
}
