#include "../includes/cub3d.h"

void	init_doors(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	game->door_count = 0;
	while (game->config.map[y])
	{
		x = 0;
		while (game->config.map[y][x])
		{
			if (game->config.map[y][x] == 'D' && game->door_count < DOOR_MAX)
			{
				game->doors[game->door_count].open = 0;
				game->doors[game->door_count].x = (x + 0.5f) * BLOCK_SIZE;
				game->doors[game->door_count].y = (y + 0.5f) * BLOCK_SIZE;
				game->door_count++;
			}
			x++;
		}
		y++;
	}
}

void	try_open_door(t_game *game)
{
	int		i;
	float	dx;
	float	dy;
	float	dist;
	int		map_x;
	int		map_y;

	i = 0;
	while (i < game->door_count)
	{
		dx = game->doors[i].x - game->player->x;
		dy = game->doors[i].y - game->player->y;
		dist = sqrt(dx * dx + dy * dy);
		if (dist <= DOOR_OPEN_DIST)
		{
			if (game->enemies_alive > 0)
			{
				printf("Kill all enemies first!\n");
				return ;
			}
			game->doors[i].open = 1;
			map_x = (int)(game->doors[i].x / BLOCK_SIZE);
			map_y = (int)(game->doors[i].y / BLOCK_SIZE);
			if (game->config.map[map_y] && game->config.map[map_y][map_x])
				game->config.map[map_y][map_x] = '0';
			return ;
		}
		i++;
	}
}

int	is_door_blocking(double x, double y, t_game *game)
{
	int	i;
	float	dx;
	float	dy;
	float	dist;

	i = 0;
	while (i < game->door_count)
	{
		if (!game->doors[i].open)
		{
			dx = game->doors[i].x - x;
			dy = game->doors[i].y - y;
			dist = sqrt(dx * dx + dy * dy);
			if (dist < BLOCK_SIZE / 2.0f)
				return (1);
		}
		i++;
	}
	return (0);
}

t_door	*get_door_at_map_pos(int map_x, int map_y, t_game *game)
{
	int	i;
	float	door_x;
	float	door_y;

	door_x = (map_x + 0.5f) * BLOCK_SIZE;
	door_y = (map_y + 0.5f) * BLOCK_SIZE;
	i = 0;
	while (i < game->door_count)
	{
		if (fabsf(game->doors[i].x - door_x) < 0.1f && fabsf(game->doors[i].y - door_y) < 0.1f)
			return (&game->doors[i]);
		i++;
	}
	return (NULL);
}
