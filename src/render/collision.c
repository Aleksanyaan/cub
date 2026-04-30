/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:31:08 by zaleksan          #+#    #+#             */
/*   Updated: 2026/02/21 14:33:42 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	is_wall(double x, double y, t_game *game)
{
	int	map_x;
	int	map_y;

	map_x = (int)(x / BLOCK_SIZE);
	map_y = (int)(y / BLOCK_SIZE);
	if (map_y < 0 || !game->config.map[map_y])
		return (1);
	if (map_x < 0 || game->config.map[map_y][map_x] == '\0')
		return (1);
	return (game->config.map[map_y][map_x] == '1');
}

int	is_circle_colliding_with_wall(double x, double y, double radius, t_game *game)
{
	int	min_x;
	int	max_x;
	int	min_y;
	int	max_y;
	int	mx;
	int	my;
	double	cell_x;
	double	cell_y;
	double	px;
	double	py;
	double	dx;
	double	dy;

	min_x = (int)floor((x - radius) / BLOCK_SIZE);
	max_x = (int)floor((x + radius) / BLOCK_SIZE);
	min_y = (int)floor((y - radius) / BLOCK_SIZE);
	max_y = (int)floor((y + radius) / BLOCK_SIZE);
	mx = min_x;
	while (mx <= max_x)
	{
		my = min_y;
		while (my <= max_y)
		{
			if (my >= 0 && game->config.map[my] && mx >= 0 && game->config.map[my][mx])
			{
				if (game->config.map[my][mx] == '1')
				{
					cell_x = mx * BLOCK_SIZE;
					cell_y = my * BLOCK_SIZE;
					/* closest point on rectangle to circle center */
					px = x;
					if (px < cell_x)
						px = cell_x;
					if (px > cell_x + BLOCK_SIZE)
						px = cell_x + BLOCK_SIZE;
					py = y;
					if (py < cell_y)
						py = cell_y;
					if (py > cell_y + BLOCK_SIZE)
						py = cell_y + BLOCK_SIZE;
					dx = x - px;
					dy = y - py;
					if (dx * dx + dy * dy < radius * radius)
						return (1);
				}
			}
			my++;
		}
		mx++;
	}
	if (is_door_blocking(x, y, game))
		return (1);
	return (0);
}
