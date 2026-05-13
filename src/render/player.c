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

#define PLAYER_MOVE_FACTOR 0.35

static void	collect_pickup(t_game *game)
{
	int		map_x;
	int		map_y;
	char	cell;

	map_x = (int)(game->player->x / BLOCK_SIZE);
	map_y = (int)(game->player->y / BLOCK_SIZE);
	if (map_y < 0 || !game->config.map[map_y] || map_x < 0
		|| !game->config.map[map_y][map_x])
		return ;
	cell = game->config.map[map_y][map_x];
	if (cell == '5')
		game->life += 10;
	else if (cell == '6')
		game->life += 15;
	else
		return ;
	if (game->life > 100)
		game->life = 100;
	game->config.map[map_y][map_x] = '0';
}

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

static void	clear_player_input(t_player *player)
{
	player->key_up = 0;
	player->key_down = 0;
	player->key_left = 0;
	player->key_right = 0;
	player->left_rotate = 0;
	player->right_rotate = 0;
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == ESC)
		close_window(game);
	if (game->current_state != STATE_PLAYING)
	{
		clear_player_input(game->player);
		return (0);
	}
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
	if (keycode == ENTER)
		try_open_door(game);
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (game->current_state != STATE_PLAYING)
	{
		clear_player_input(game->player);
		return (0);
	}
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

void	rotate_palyer(t_player *player, t_game *game)
{
	float	angle_speed;

	angle_speed = 0.03f * game->frame_scale;
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
	double	move_x;
	double	move_y;
	double	move_len;
	double	step_x;
	double	step_y;
	double	next_x;
	double	next_y;
	double	move_speed;
	int		steps;
	int		i;

	rotate_palyer(player, game);
	move_x = 0;
	move_y = 0;
	move_speed = MOVE_SPEED * game->frame_scale * PLAYER_MOVE_FACTOR;

	if (player->key_up)
	{
		move_x += cos(player->angle) * move_speed;
		move_y += sin(player->angle) * move_speed;
	}
	if (player->key_down)
	{
		move_x -= cos(player->angle) * move_speed;
		move_y -= sin(player->angle) * move_speed;
	}
	if (player->key_right)
	{
		move_x += -sin(player->angle) * move_speed;
		move_y += cos(player->angle) * move_speed;
	}
	if (player->key_left)
	{
		move_x += sin(player->angle) * move_speed;
		move_y += -cos(player->angle) * move_speed;
	}

	move_len = sqrt(move_x * move_x + move_y * move_y);
	if (move_len <= 0)
		return ;
	if (move_len > move_speed)
	{
		move_x = (move_x / move_len) * move_speed;
		move_y = (move_y / move_len) * move_speed;
	}
	steps = (int)ceil(fmax(fabs(move_x), fabs(move_y)));
	if (steps < 1)
		steps = 1;
	step_x = move_x / steps;
	step_y = move_y / steps;
	i = 0;
	while (i < steps)
	{
		next_x = player->x + step_x;
		next_y = player->y + step_y;
		if (!is_circle_colliding_with_wall(next_x, next_y, PLAYER_RADIUS, game))
		{
			player->x = next_x;
			player->y = next_y;
		}
		else
		{
			if (!is_circle_colliding_with_wall(next_x, player->y,
					PLAYER_RADIUS, game))
				player->x = next_x;
			if (!is_circle_colliding_with_wall(player->x, next_y,
					PLAYER_RADIUS, game))
				player->y = next_y;
		}
		i++;
	}
	collect_pickup(game);
}
