/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 15:58:45 by zaleksan          #+#    #+#             */
/*   Updated: 2026/03/14 22:45:22 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_player(t_player *player)
{
	player->x = WIDTH / 2;
	player->y = HEIGHT / 2;
	player->angle = 0;
	player->key_up = 0;
	player->key_down = 0;
	player->key_left = 0;
	player->key_right = 0;
	player->left_rotate = 0;
	player->right_rotate = 0;
	ft_bzero(player->cartridges, sizeof(player->cartridges));
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == W)
		game->player->key_up = 1;
	if (keycode == A)
		game->player->key_left = 1;
	if (keycode == S)
		game->player->key_down = 1;
	if (keycode == D)
		game->player->key_right = 1;
	if (keycode == LEFT)
		game->player->left_rotate = 1;
	if (keycode == RIGHT)
		game->player->right_rotate = 1;
	if (keycode == SPACE)
		shoot_player(game->player);
	if (keycode == E)
		try_open_door(game);
	if (keycode == ESC)
		close_window(game);
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode == W)
		game->player->key_up = 0;
	if (keycode == A)
		game->player->key_left = 0;
	if (keycode == S)
		game->player->key_down = 0;
	if (keycode == D)
		game->player->key_right = 0;
	if (keycode == LEFT)
		game->player->left_rotate = 0;
	if (keycode == RIGHT)
		game->player->right_rotate = 0;
	return (0);
}

void	rotate_palyer(t_player *player)
{
	float	angle_speed;

	angle_speed = 0.03;
	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;
	if (player->angle > 2 * PI)
		player->angle -= 2 * PI;
	if (player->angle < 0)
		player->angle += 2 * PI;
}

void	move_player(t_player *player, t_game *game)
{
	double	new_x;
	double	new_y;
	double	cos_angle;
	double	sin_angle;

	rotate_palyer(player);
	cos_angle = cos(player->angle);
	sin_angle = sin(player->angle);
	new_x = player->x;
	new_y = player->y;

	if (player->key_up)
	{
		new_x += cos_angle * MOVE_SPEED;
		new_y += sin_angle * MOVE_SPEED;
	}
	if (player->key_down)
	{
		new_x -= cos_angle * MOVE_SPEED;
		new_y -= sin_angle * MOVE_SPEED;
	}
	if (player->key_right)
	{
		new_x += -sin_angle * MOVE_SPEED;
		new_y += cos_angle * MOVE_SPEED;
	}
	if (player->key_left)
	{
		new_x += sin_angle * MOVE_SPEED;
		new_y += -cos_angle * MOVE_SPEED;
	}

	if (!is_wall(new_x + PLAYER_RADIUS, player->y, game)
		&& !is_wall(new_x - PLAYER_RADIUS, player->y, game))
		player->x = new_x;

	if (!is_wall(player->x, new_y + PLAYER_RADIUS, game)
		&& !is_wall(player->x, new_y - PLAYER_RADIUS, game))
		player->y = new_y;
}
