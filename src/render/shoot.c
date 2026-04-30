
#include "../includes/cub3d.h"

#define CARTRIDGE_SPEED 28.0f
#define CARTRIDGE_SIZE 4

static void	fill_cartridge(t_game *game, int x, int y)
{
	int	i;
	int	j;

	i = 0;
	while (i < CARTRIDGE_SIZE)
	{
		j = 0;
		while (j < CARTRIDGE_SIZE)
		{
			put_pixel(x + i, y + j, (t_color){220, 180, 60}, game);
			j++;
		}
		i++;
	}
}

static int	cartridge_size_from_dist(float dist)
{
	int	size;

	size = (int)(80 / dist);
	if (size < 3)
		size = 3;
	if (size > 10)
		size = 10;
	return (size);
}

static void	add_bullet_hole(t_game *game, float x, float y)
{
	int	slot;

	slot = game->bullet_hole_index % BULLET_HOLE_MAX;
	game->bullet_holes[slot].active = 1;
	game->bullet_holes[slot].x = x;
	game->bullet_holes[slot].y = y;
	game->bullet_hole_index++;
}

static void	draw_single_cartridge(t_game *game, float cartridge_x,
	float cartridge_y)
{
	float	dist;
	int		size;
	int		x;
	int		y;
	t_texture	tex;
	float	muzzle_cx;
	float	muzzle_cy;
	float	angle_to_cartridge;
	float	delta;
	float	fov;
	float	projected_cx;
	float	projected_cy;
	float	travel_ref;
	float	alpha;
	float	final_cx;
	float	final_cy;

	dist = fixed_dist(game->player->x, game->player->y,
		cartridge_x, cartridge_y, game);
	size = cartridge_size_from_dist(dist);
	if (game->gun_texture.img)
	{
		tex = game->gun_texture;
		x = (WIDTH - tex.width) / 2;
		y = HEIGHT - tex.height;
		muzzle_cx = x + tex.width * 0.5f;
		muzzle_cy = y + tex.height * 0.70f;
		angle_to_cartridge = atan2(cartridge_y - game->player->y,
			cartridge_x - game->player->x);
		delta = angle_to_cartridge - game->player->angle;
		while (delta <= -PI)
			delta += 2 * PI;
		while (delta > PI)
			delta -= 2 * PI;
		fov = PI / 3.0f;
		if (fabs(delta) > fov / 2.0f)
			return ;
		projected_cx = (WIDTH / 2.0f) + (delta) * (WIDTH / fov);
		projected_cy = (HEIGHT / 2.0f);
		travel_ref = 400.0f;
		alpha = dist / travel_ref;
		if (alpha > 1.0f)
			alpha = 1.0f;
		final_cx = (1.0f - alpha) * muzzle_cx + alpha * projected_cx;
		final_cy = (1.0f - alpha) * muzzle_cy + alpha * projected_cy;
		x = (int)(final_cx - size / 2);
		y = (int)(final_cy - size / 2);
		fill_cartridge(game, x, y);
	}
	else
	{
		x = WIDTH / 2 - size / 2;
		y = HEIGHT / 2 - size / 2;
		fill_cartridge(game, x, y);
	}
}

void	update_shoot(t_player *player, t_game *game)
{
	int		i;
	float	step_x;
	float	step_y;
	float	next_x;
	float	next_y;

	i = 0;
	while (i < CARTRIDGE_MAX)
	{
		if (player->cartridges[i].active)
		{
			step_x = cos(player->cartridges[i].angle) * CARTRIDGE_SPEED;
			step_y = sin(player->cartridges[i].angle) * CARTRIDGE_SPEED;
			next_x = player->cartridges[i].x + step_x;
			next_y = player->cartridges[i].y + step_y;
			if (hit_enemy(next_x, next_y, game))
			{
				player->cartridges[i].active = 0;
				i++;
				continue ;
			}
			if (touch(next_x, next_y, game))
			{
				add_bullet_hole(game, next_x - step_x * 0.5f,
					next_y - step_y * 0.5f);
				player->cartridges[i].active = 0;
			}
			else
			{
				player->cartridges[i].x = next_x;
				player->cartridges[i].y = next_y;
			}
		}
		i++;
	}
}

void	draw_shoot(t_game *game)
{
	int		i;

	i = 0;
	while (i < CARTRIDGE_MAX)
	{
		if (game->player->cartridges[i].active)
			draw_single_cartridge(game, game->player->cartridges[i].x,
				game->player->cartridges[i].y);
		i++;
	}
}

void	shoot_player(t_player *player)
{
	int		i;
	float	offset;

	i = 0;
	while (i < CARTRIDGE_MAX)
	{
		if (!player->cartridges[i].active)
		{
			offset = PLAYER_RADIUS + 2;
			player->cartridges[i].active = 1;
			player->cartridges[i].x = player->x + cos(player->angle) * offset;
			player->cartridges[i].y = player->y + sin(player->angle) * offset;
			player->cartridges[i].angle = player->angle;
			return ;
		}
		i++;
	}
}