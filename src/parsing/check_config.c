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

static int	is_walkable_cell(char c)
{
	if (c == '0' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6'
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

static void	find_player_cell(char **map, int *px, int *py)
{
	int		y;
	int		x;
	char	c;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			c = map[y][x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				*px = x;
				*py = y;
				return ;
			}
			x++;
		}
		y++;
	}
	*px = -1;
	*py = -1;
}

static int	max_map_cols(char **map)
{
	int	rows;
	int	len;
	int	max_cols;

	rows = 0;
	max_cols = 0;
	while (map[rows])
	{
		len = ft_strlen(map[rows]);
		if (len > max_cols)
			max_cols = len;
		rows++;
	}
	return (max_cols);
}

static int	enemy_is_reachable(char **map)
{
	int	rows;
	int	cols;
	int	*visited;
	int	*queue;
	int	head;
	int	tail;
	int	px;
	int	py;
	int	idx;
	int	x;
	int	y;
	int	nx;
	int	ny;
	int	nidx;

	rows = 0;
	while (map[rows])
		rows++;
	cols = max_map_cols(map);
	if (rows <= 0 || cols <= 0)
		return (0);
	visited = ft_calloc(rows * cols, sizeof(int));
	queue = ft_calloc(rows * cols, sizeof(int));
	if (!visited || !queue)
	{
		free(visited);
		free(queue);
		return (1);
	}
	find_player_cell(map, &px, &py);
	if (px < 0 || py < 0 || !is_walkable_cell(map[py][px]))
	{
		free(visited);
		free(queue);
		return (1);
	}
	head = 0;
	tail = 0;
	idx = py * cols + px;
	visited[idx] = 1;
	queue[tail++] = idx;
	while (head < tail)
	{
		idx = queue[head++];
		x = idx % cols;
		y = idx / cols;
		nx = x + 1;
		ny = y;
		if (nx >= 0 && ny >= 0 && map[ny] && map[ny][nx]
			&& is_walkable_cell(map[ny][nx]))
		{
			nidx = ny * cols + nx;
			if (!visited[nidx])
			{
				visited[nidx] = 1;
				queue[tail++] = nidx;
			}
		}
		nx = x - 1;
		ny = y;
		if (nx >= 0 && ny >= 0 && map[ny] && map[ny][nx]
			&& is_walkable_cell(map[ny][nx]))
		{
			nidx = ny * cols + nx;
			if (!visited[nidx])
			{
				visited[nidx] = 1;
				queue[tail++] = nidx;
			}
		}
		nx = x;
		ny = y + 1;
		if (nx >= 0 && ny >= 0 && map[ny] && map[ny][nx]
			&& is_walkable_cell(map[ny][nx]))
		{
			nidx = ny * cols + nx;
			if (!visited[nidx])
			{
				visited[nidx] = 1;
				queue[tail++] = nidx;
			}
		}
		nx = x;
		ny = y - 1;
		if (nx >= 0 && ny >= 0 && map[ny] && map[ny][nx]
			&& is_walkable_cell(map[ny][nx]))
		{
			nidx = ny * cols + nx;
			if (!visited[nidx])
			{
				visited[nidx] = 1;
				queue[tail++] = nidx;
			}
		}
	}
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if ((map[y][x] == '2' || map[y][x] == '3' || map[y][x] == '4')
				&& !visited[y * cols + x])
			{
				free(visited);
				free(queue);
				return (1);
			}
			x++;
		}
		y++;
	}
	free(visited);
	free(queue);
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
	if (!config->floor_color && !config->floor_texture)
		free_and_exit(config, NULL, ": <floor> color/texture is missing or invalid");
	if (!config->ceiling_color && !config->ceiling_texture)
		free_and_exit(config, NULL, ": <ceiling> color/texture is missing or invalid");
	if (config->floor_texture && !check_file_extension(config->floor_texture,
			"xpm"))
		free_and_exit(config, NULL, ": <floor texture> must be .xpm");
	if (config->ceiling_texture && !check_file_extension(config->ceiling_texture,
			"xpm"))
		free_and_exit(config, NULL, ": <ceiling texture> must be .xpm");
	if (!config->map || check_map(config->map))
		free_and_exit(config, NULL, ": <map> is missing> or invalid");
	if (enemy_is_reachable(config->map))
		free_and_exit(config, NULL,
			": <map> is unwinnable (enemy unreachable from player)");
}
