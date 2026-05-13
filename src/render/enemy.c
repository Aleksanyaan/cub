#include "../includes/cub3d.h"

#define ENEMY_HIT_RADIUS 28.0f
#define FOV (PI / 3.0f)
#define ENEMY_SPEED_WALKER 0.7f
#define ENEMY_SPEED_RUNNER 2.2f
#define ENEMY_MOVE_FACTOR 0.35f
#define ENEMY_RADIUS 8
#define ENEMY_STOP_DIST 28.0f
#define ENEMY_ANIM_MS 140
#define ENEMY_SCALE 0.5f
#define ENEMY_TOUCH_DAMAGE 5
#define ENEMY_DAMAGE_COOLDOWN_MS 300
/* type constants */
#define ENEMY_WALKER 0
#define ENEMY_RUNNER 1
#define ENEMY_STANDING 2

static float	normalize_angle(float angle)
{
	while (angle <= -PI)
		angle += 2 * PI;
	while (angle > PI)
		angle -= 2 * PI;
	return (angle);
}

static t_texture	*get_enemy_texture(t_game *game, t_enemy *enemy, int enemy_id)
{
	long	frame;
	float	rel;

	if (enemy->type == ENEMY_STANDING)
	{
		rel = normalize_angle(atan2(game->player->y - enemy->y,
				game->player->x - enemy->x) - enemy->angle);
		if (fabs(rel) <= PI / 4.0f)
			return (&game->front_texture);
		if (fabs(rel) >= 3.0f * PI / 4.0f)
			return (&game->back_texture);
		if (rel > 0)
			return (&game->left_texture);
		return (&game->right_texture);
	}
	frame = (current_time_ms() / ENEMY_ANIM_MS + enemy_id) % 4;
	if (frame == 0)
		return (&game->zombie1_texture);
	if (frame == 1)
		return (&game->zombie2_texture);
	if (frame == 2)
		return (&game->zombie3_texture);
	return (&game->zombie4_texture);
}

void	init_enemies(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (game->config.map[y])
	{
		x = 0;
		while (game->config.map[y][x])
		{
			if (game->config.map[y][x] == '2' && game->enemy_count < ENEMY_MAX)
			{
				game->enemies[game->enemy_count].type = ENEMY_WALKER;
				game->enemies[game->enemy_count].active = 1;
				game->enemies[game->enemy_count].x = (x + 0.5f) * BLOCK_SIZE;
				game->enemies[game->enemy_count].y = (y + 0.5f) * BLOCK_SIZE;
				game->enemies[game->enemy_count].angle = 0.0f;
				game->enemies[game->enemy_count].last_dist_to_player = 0.0f;
				game->enemies[game->enemy_count].last_progress_time = 0;
				game->enemies[game->enemy_count].ghost_mode = 0;
				game->enemy_count++;
				game->enemies_alive++;
				game->config.map[y][x] = '0';
			}
			else if (game->config.map[y][x] == '3'
				&& game->enemy_count < ENEMY_MAX)
			{
				game->enemies[game->enemy_count].type = ENEMY_RUNNER;
				game->enemies[game->enemy_count].active = 1;
				game->enemies[game->enemy_count].x = (x + 0.5f) * BLOCK_SIZE;
				game->enemies[game->enemy_count].y = (y + 0.5f) * BLOCK_SIZE;
				game->enemies[game->enemy_count].angle = 0.0f;
				game->enemies[game->enemy_count].last_dist_to_player = 0.0f;
				game->enemies[game->enemy_count].last_progress_time = 0;
				game->enemies[game->enemy_count].ghost_mode = 0;
				game->enemy_count++;
				game->enemies_alive++;
				game->config.map[y][x] = '0';
			}
			else if (game->config.map[y][x] == '4'
				&& game->enemy_count < ENEMY_MAX)
			{
				game->enemies[game->enemy_count].type = ENEMY_STANDING;
				game->enemies[game->enemy_count].active = 1;
				game->enemies[game->enemy_count].x = (x + 0.5f) * BLOCK_SIZE;
				game->enemies[game->enemy_count].y = (y + 0.5f) * BLOCK_SIZE;
				game->enemies[game->enemy_count].angle = 0.0f;
				game->enemies[game->enemy_count].last_dist_to_player = 0.0f;
				game->enemies[game->enemy_count].last_progress_time = 0;
				game->enemies[game->enemy_count].ghost_mode = 0;
				game->enemy_count++;
				game->enemies_alive++;
				game->config.map[y][x] = '0';
			}
			x++;
		}
		y++;
	}
}

static void	move_enemy(t_game *game, t_enemy *enemy)
{
	float	dx;
	float	dy;
	float	dist;
	float	next_x;
	float	next_y;
	float	step_x;
	float	step_y;
	float	step_scale;

	if (enemy->type == ENEMY_STANDING)
		return ;
	dx = game->player->x - enemy->x;
	dy = game->player->y - enemy->y;
	dist = sqrt(dx * dx + dy * dy);
	if (dist <= ENEMY_STOP_DIST)
		return ;
	enemy->angle = atan2(dy, dx);
	if (enemy->type == ENEMY_RUNNER)
		step_scale = ENEMY_SPEED_RUNNER * ENEMY_MOVE_FACTOR * game->frame_scale;
	else
		step_scale = ENEMY_SPEED_WALKER * ENEMY_MOVE_FACTOR * game->frame_scale;
	step_x = cos(enemy->angle) * step_scale;
	step_y = sin(enemy->angle) * step_scale;
	next_x = enemy->x + step_x;
	next_y = enemy->y + step_y;

	/* Try full circular collision test first */
	if (!is_circle_colliding_with_wall(next_x, next_y, ENEMY_RADIUS, game))
	{
		enemy->x = next_x;
		enemy->y = next_y;
		return ;
	}

	/* Fall back to axis-aligned circular checks if full move is blocked */
	if (!is_circle_colliding_with_wall(next_x, enemy->y, ENEMY_RADIUS, game))
		enemy->x = next_x;
	if (!is_circle_colliding_with_wall(enemy->x, next_y, ENEMY_RADIUS, game))
		enemy->y = next_y;
}

void	check_victory(t_game *game)
{
	int	i;
	int	active_count;

	if (game->current_state != STATE_PLAYING)
		return ;
	i = 0;
	active_count = 0;
	while (i < game->enemy_count)
	{
		if (game->enemies[i].active)
			active_count++;
		i++;
	}
	if (active_count == 0)
		game->current_state = STATE_WIN;
}

void	update_enemies(t_game *game)
{
	int	i;
	float	dx;
	float	dy;
	long	now;
	int	player_touched;

	i = 0;
	player_touched = 0;
	now = current_time_ms();
	while (i < game->enemy_count)
	{
		if (game->enemies[i].active)
		{
			move_enemy(game, &game->enemies[i]);
			dx = game->player->x - game->enemies[i].x;
			dy = game->player->y - game->enemies[i].y;
			if (dx * dx + dy * dy <= ENEMY_HIT_RADIUS * ENEMY_HIT_RADIUS)
				player_touched = 1;
		}
		i++;
	}
	if (player_touched && now - game->last_damage_time >= ENEMY_DAMAGE_COOLDOWN_MS)
	{
		game->life -= ENEMY_TOUCH_DAMAGE;
		if (game->life < 0)
			game->life = 0;
		if (game->life <= 0)
			game->current_state = STATE_LOSE;
		game->last_damage_time = now;
	}
}

static void	draw_enemy(t_game *game, t_enemy *enemy, int enemy_id)
{
	float		dx;
	float		dy;
	float		dist;
	float		angle_to_enemy;
	float		delta;
	float		corrected_dist;
	int			sprite_h;
	int			sprite_w;
	int			start_x;
	int			end_x;
	int			start_y;
	int			end_y;
	t_texture	*tex;
	int			x;
	int			y;
	int			tex_x;
	int			tex_y;
	int			color_hex;
	t_color		color;

	dx = enemy->x - game->player->x;
	dy = enemy->y - game->player->y;
	dist = sqrt(dx * dx + dy * dy);
	if (dist <= 1.0f)
		return ;
	angle_to_enemy = atan2(dy, dx);
	delta = normalize_angle(angle_to_enemy - game->player->angle);
	if (fabs(delta) > FOV / 2.0f)
		return ;
	corrected_dist = dist * cos(delta);
	if (corrected_dist <= 0.001f)
		return ;
	tex = get_enemy_texture(game, enemy, enemy_id);
	sprite_h = (int)(((BLOCK_SIZE / corrected_dist) * (WIDTH / 2.0f)) * ENEMY_SCALE);
	if (sprite_h <= 0)
		return ;
	sprite_w = (int)(sprite_h * ((float)tex->width / (float)tex->height));
	if (sprite_w <= 0)
		return ;
	start_x = (int)((WIDTH / 2.0f) + tan(delta) * (WIDTH / 2.0f)
			/ tan(FOV / 2.0f) - sprite_w / 2.0f);
	end_x = start_x + sprite_w;
	start_y = (HEIGHT - sprite_h) / 2;
	end_y = start_y + sprite_h;
	x = start_x;
	while (x < end_x)
	{
		if (x >= 0 && x < WIDTH && corrected_dist < game->wall_dist[x])
		{
			tex_x = (int)(((float)(x - start_x) / sprite_w) * tex->width);
			y = start_y;
			while (y < end_y)
			{
				if (y >= 0 && y < HEIGHT)
				{
					tex_y = (int)(((float)(y - start_y) / sprite_h) * tex->height);
					color_hex = *(int *)(tex->addr + (tex_y * tex->line_len
							+ tex_x * (tex->bpp / 8)));
					if ((color_hex & 0xFF000000) != 0xFF000000)
					{
						color.blue = color_hex & 0xFF;
						color.green = (color_hex >> 8) & 0xFF;
						color.red = (color_hex >> 16) & 0xFF;
						put_pixel(x, y, color, game);
					}
				}
				y++;
			}
		}
		x++;
	}
}

static int	cmp_enemy_dist(t_game *game, int a, int b)
{
	float	dxa;
	float	dya;
	float	dxb;
	float	dyb;

	dxa = game->enemies[a].x - game->player->x;
	dya = game->enemies[a].y - game->player->y;
	dxb = game->enemies[b].x - game->player->x;
	dyb = game->enemies[b].y - game->player->y;
	return ((dxa * dxa + dya * dya) > (dxb * dxb + dyb * dyb));
}

void	draw_enemies(t_game *game)
{
	int		order[ENEMY_MAX];
	int		count;
	int		i;
	int		j;
	int		tmp;

	count = 0;
	i = 0;
	while (i < game->enemy_count)
	{
		if (game->enemies[i].active)
			order[count++] = i;
		i++;
	}
	/* bubble sort: farthest (largest dist²) first */
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (!cmp_enemy_dist(game, order[j], order[j + 1]))
			{
				tmp = order[j];
				order[j] = order[j + 1];
				order[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < count)
	{
		draw_enemy(game, &game->enemies[order[i]], order[i]);
		i++;
	}
}

int	hit_enemy(float x, float y, t_game *game)
{
	int		i;
	float	dx;
	float	dy;

	i = 0;
	while (i < game->enemy_count)
	{
		if (game->enemies[i].active)
		{
			dx = x - game->enemies[i].x;
			dy = y - game->enemies[i].y;
			if (dx * dx + dy * dy <= ENEMY_HIT_RADIUS * ENEMY_HIT_RADIUS)
			{
				game->enemies[i].active = 0;
				if (game->enemies_alive > 0)
					game->enemies_alive--;
				return (1);
			}
		}
		i++;
	}
	return (0);
}
