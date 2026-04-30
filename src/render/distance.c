/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   distance.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:05:21 by zaleksan          #+#    #+#             */
/*   Updated: 2026/02/21 14:15:36 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/cub3d.h"

int	touch(float px, float py, t_game *game)
{
	int	x;
	int	y;

	x = px / BLOCK_SIZE;
	y = py / BLOCK_SIZE;
	if (x < 0 || y < 0 || !game->config.map[y] || !game->config.map[y][x])
		return (1);
	if (game->config.map[y][x] == '1')
		return (1);
	return (0);
}

float	distance(float x, float y)
{
	return (sqrt(x * x + y * y));
}

float	fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
	float delta_x;
	float delta_y;
	float angle;
	float fix_dist;

	delta_x = x2 - x1;
	delta_y = y2 - y1;
	angle = atan2(delta_y, delta_x) - game->player->angle;
	fix_dist = distance(delta_x, delta_y) * cos(angle);
	return (fix_dist);
}