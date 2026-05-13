#include "../includes/cub3d.h"

#define MINIMAP_DEFAULT_SIZE 200
#define MINIMAP_DEFAULT_SCALE 0.25f
#define MINIMAP_Y 10

/* minimap startup size and scale: constants that do not change at runtime */
static const int	minimap_size = MINIMAP_DEFAULT_SIZE;
static const float	minimap_scale = MINIMAP_DEFAULT_SCALE;

static int	minimap_x(void)
{
	return (WIDTH - minimap_size - 10);
}

static void	draw_minimap_cell(int map_x, int map_y, t_color color, t_game *game, float camera_offset_x, float camera_offset_y)
{
	int	x;
	int	y;
	int	cell_size;
	int	pixel_x;
	int	pixel_y;

	cell_size = (int)(BLOCK_SIZE * minimap_scale);
	if (cell_size < 1)
		cell_size = 1;
	y = 0;
	while (y < cell_size)
	{
		x = 0;
		while (x < cell_size)
		{
			pixel_x = minimap_x() + (int)((map_x * BLOCK_SIZE - camera_offset_x) * minimap_scale) + x;
			pixel_y = MINIMAP_Y + (int)((map_y * BLOCK_SIZE - camera_offset_y) * minimap_scale) + y;
			if (pixel_x >= minimap_x() && pixel_x < minimap_x() + minimap_size
				&& pixel_y >= MINIMAP_Y && pixel_y < MINIMAP_Y + minimap_size)
				put_pixel(pixel_x, pixel_y, color, game);
			x++;
		}
		y++;
	}
}

static void	draw_minimap_border(t_game *game)
{
	t_color	border_color;
	int		i;

	border_color = (t_color){200, 200, 200};
	i = 0;
	while (i < minimap_size)
	{
		put_pixel(minimap_x() + i, MINIMAP_Y, border_color, game);
		put_pixel(minimap_x() + i, MINIMAP_Y + minimap_size, border_color, game);
		put_pixel(minimap_x(), MINIMAP_Y + i, border_color, game);
		put_pixel(minimap_x() + minimap_size, MINIMAP_Y + i, border_color, game);
		i++;
	}
}

static void	draw_minimap_player(t_game *game, float camera_offset_x, float camera_offset_y)
{
	int		player_x;
	int		player_y;
	int		i;
	int		j;
	t_color	player_color;
	t_color	direction_color;
	int		pixel_x;
	int		pixel_y;

	player_color = (t_color){0, 255, 0};
	direction_color = (t_color){255, 255, 0};
	
	// Compute player position relative to camera offset
	player_x = minimap_x() + (int)((game->player->x - camera_offset_x) * minimap_scale);
	player_y = MINIMAP_Y + (int)((game->player->y - camera_offset_y) * minimap_scale);
	
	// Draw player as a small circle (5x5)
	i = -2;
	while (i <= 2)
	{
		j = -2;
		while (j <= 2)
		{
			if (i * i + j * j <= 4)
			{
				pixel_x = player_x + i;
				pixel_y = player_y + j;
				if (pixel_x >= minimap_x() && pixel_x < minimap_x() + minimap_size
					&& pixel_y >= MINIMAP_Y && pixel_y < MINIMAP_Y + minimap_size)
					put_pixel(pixel_x, pixel_y, player_color, game);
			}
			j++;
		}
		i++;
	}
	
	// Draw direction indicator (line from player in looking direction)
	i = 0;
	while (i <= 15)
	{
		pixel_x = player_x + (int)(cos(game->player->angle) * i);
		pixel_y = player_y + (int)(sin(game->player->angle) * i);
		if (pixel_x >= minimap_x() && pixel_x < minimap_x() + minimap_size
			&& pixel_y >= MINIMAP_Y && pixel_y < MINIMAP_Y + minimap_size)
			put_pixel(pixel_x, pixel_y, direction_color, game);
		i++;
	}
}

static void	draw_minimap_enemies(t_game *game, float camera_offset_x, float camera_offset_y)
{
	int		i;
	int		enemy_x;
	int		enemy_y;
	t_color	enemy_color;
	int		j;
	int		pixel_x;
	int		pixel_y;

	enemy_color = (t_color){255, 0, 0};
	i = 0;
	while (i < game->enemy_count)
	{
		if (game->enemies[i].active)
		{
			enemy_x = minimap_x() + (int)((game->enemies[i].x - camera_offset_x) * minimap_scale);
			enemy_y = MINIMAP_Y + (int)((game->enemies[i].y - camera_offset_y) * minimap_scale);
			
			// Draw enemy as a small square (3x3)
			j = 0;
			while (j < 3)
			{
				pixel_x = enemy_x + j;
				pixel_y = enemy_y;
				if (pixel_x >= minimap_x() && pixel_x < minimap_x() + minimap_size
					&& pixel_y >= MINIMAP_Y && pixel_y < MINIMAP_Y + minimap_size)
					put_pixel(pixel_x, pixel_y, enemy_color, game);
				
				pixel_x = enemy_x + j;
				pixel_y = enemy_y + 2;
				if (pixel_x >= minimap_x() && pixel_x < minimap_x() + minimap_size
					&& pixel_y >= MINIMAP_Y && pixel_y < MINIMAP_Y + minimap_size)
					put_pixel(pixel_x, pixel_y, enemy_color, game);
				
				pixel_x = enemy_x;
				pixel_y = enemy_y + j;
				if (pixel_x >= minimap_x() && pixel_x < minimap_x() + minimap_size
					&& pixel_y >= MINIMAP_Y && pixel_y < MINIMAP_Y + minimap_size)
					put_pixel(pixel_x, pixel_y, enemy_color, game);
				
				pixel_x = enemy_x + 2;
				pixel_y = enemy_y + j;
				if (pixel_x >= minimap_x() && pixel_x < minimap_x() + minimap_size
					&& pixel_y >= MINIMAP_Y && pixel_y < MINIMAP_Y + minimap_size)
					put_pixel(pixel_x, pixel_y, enemy_color, game);
				j++;
			}
		}
		i++;
	}
}

void	draw_minimap(t_game *game)
{
	char	**map;
	t_color	wall_color;
	t_color	floor_color;
	int		y;
	int		x;
	float	camera_offset_x;
	float	camera_offset_y;

	map = game->config.map;
	wall_color = (t_color){50, 50, 50};
	floor_color = (t_color){100, 100, 100};
	
	// Calculate camera offset to keep player centered, then clamp to map bounds
	camera_offset_x = game->player->x - (minimap_size / 2.0f) / minimap_scale;
	camera_offset_y = game->player->y - (minimap_size / 2.0f) / minimap_scale;

	/* compute map pixel dimensions */
	int map_rows = 0;
	int map_cols_max = 0;
	while (map[map_rows])
	{
		int len = 0;
		while (map[map_rows][len])
			len++;
		if (len > map_cols_max)
			map_cols_max = len;
		map_rows++;
	}
	float map_w = map_cols_max * BLOCK_SIZE;
	float map_h = map_rows * BLOCK_SIZE;

	float view_w = (minimap_size / minimap_scale);
	float view_h = (minimap_size / minimap_scale);

	if (camera_offset_x < 0.0f)
		camera_offset_x = 0.0f;
	if (camera_offset_y < 0.0f)
		camera_offset_y = 0.0f;
	if (map_w > view_w)
	{
		float max_off_x = map_w - view_w;
		if (camera_offset_x > max_off_x)
			camera_offset_x = max_off_x;
	}
	else
	{
		/* center map if smaller than view */
		camera_offset_x = (map_w - view_w) / 2.0f;
	}
	if (map_h > view_h)
	{
		float max_off_y = map_h - view_h;
		if (camera_offset_y > max_off_y)
			camera_offset_y = max_off_y;
	}
	else
	{
		/* center map if smaller than view */
		camera_offset_y = (map_h - view_h) / 2.0f;
	}
	
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == '1')
				draw_minimap_cell(x, y, wall_color, game, camera_offset_x, camera_offset_y);
			else if (map[y][x] == 'D')
				draw_minimap_cell(x, y, (t_color){120, 70, 30}, game, camera_offset_x, camera_offset_y);
			else
				draw_minimap_cell(x, y, floor_color, game, camera_offset_x, camera_offset_y);
			x++;
		}
		y++;
	}
	
	draw_minimap_border(game);
	draw_minimap_enemies(game, camera_offset_x, camera_offset_y);
	draw_minimap_player(game, camera_offset_x, camera_offset_y);
}

void setup_hooks(t_game *game)
{
    mlx_hook(game->win, 17, 0, handle_close, game);
}
