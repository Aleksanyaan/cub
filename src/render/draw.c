/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:05:16 by zaleksan          #+#    #+#             */
/*   Updated: 2026/03/18 00:05:48 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static t_color	get_texture_pixel_by_pos(t_texture texture, int x, int y)
{
	char	*px;

	if (texture.width <= 0 || texture.height <= 0 || !texture.addr)
		return ((t_color){0, 0, 0});
	if (x < 0)
		x = 0;
	if (x >= texture.width)
		x = texture.width - 1;
	if (y < 0)
		y = 0;
	if (y >= texture.height)
		y = texture.height - 1;
	px = texture.addr + (y * texture.line_len + x * (texture.bpp / 8));
	return ((t_color){(unsigned char)px[2], (unsigned char)px[1],
		(unsigned char)px[0]});
}

void	put_pixel(int x, int y, t_color color, t_game *game)
{
	int	index;

	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	index = y * game->size_line + x * game->bpp / 8;
	game->data[index] = color.blue;
	game->data[index + 1] = color.green;
	game->data[index + 2] = color.red;
		static long	last_time = 0;
		static int	frames = 0;
		float		fraction;
		float		start_x;
		int			i;

		ft_bzero(game->data, HEIGHT * game->size_line);
		move_player(game->player, game);
		update_enemies(game);
		update_shoot(game->player, game);
		render_floor_and_ceiling(game);
		fraction = PI / 3 / WIDTH;
		start_x = game->player->angle - PI / 6;
		i = 0;
		while (i < WIDTH)
		{
			draw_line(game->player, game, start_x, i);
			start_x += fraction;
			i++;
		}
		draw_bullet_holes(game);
		draw_enemies(game);
		draw_shoot(game);
		draw_gun(game);
		draw_minimap(game);
		draw_fps(game, &last_time, &frames);
		mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
		draw_life(game);
		return (0);
		py_coefficent = 1;
	}
	else
	{
		px_coefficent = 0;
		py_coefficent = 1;
	}
	*px = ((int)(end_x / BLOCK_SIZE) + px_coefficent) * BLOCK_SIZE;
	*py = ((int)(end_y / BLOCK_SIZE) + py_coefficent) * BLOCK_SIZE;
	return ((end_x - start_x) * (*py - start_y)
		- (end_y - start_y) * (*px - start_x));
}

t_direction	wall_direction(int quarter, int side)
{
	t_direction directions[2];

	if (quarter == 1)
	{
		directions[0] = East;
		directions[1] = South;
	}
	else if (quarter == 2)
	{
		directions[0] = South;
		directions[1] = West;
	}
	else if (quarter == 3)
	{
		directions[0] = West;
		directions[1] = North;
	}
	else
	{
		directions[0] = North;
		directions[1] = East;
	}

	if (side < 0)
		return (directions[0]);
	return (directions[1]);
}

t_direction	check_adjacent(t_game *game, int x, int y, t_direction direction, int quarter)
{
	t_direction	new_direction;

	if ((direction == West && quarter == 3) || (direction == East && quarter == 4))
		new_direction = North;
	else if ((direction == West && quarter == 2) || (direction == East && quarter == 1))
		new_direction = South;
	else if ((direction == North && quarter == 3) || (direction == South && quarter == 2))
		new_direction = West;
	else if ((direction == North && quarter == 4) || (direction == South && quarter == 1))
		new_direction = East;

	if (direction == North)
		y++;
	else if (direction == South)
		y--;
	else if (direction == West)
		x++;
	else if (direction == East)
		x--;

	if (game->config.map[y][x] == '1')
		return (new_direction);
	return (direction);
}

void	lines_intersection_point(float x1, float y1, float x2, float y2,
								float px, float py, int quarter, int side,
								float *x, float *y)
{
	float	angle;
	float	dx;
	float	dy;
	float	u;
	float	den;

	angle = PI / 2;
	if ((quarter == 1 && side >= 0) || (quarter == 2 && side < 0) || (quarter == 3 && side >= 0) || (quarter == 4 && side < 0))
		angle = 0;

	dx = cos(angle);
	dy = sin(angle);
	den = (x2 - x1) * dy - (y2 - y1) * dx;
	u = ((px - x1) * dy - (py - y1) * dx) / den;

    *x = x1 + u * (x2 - x1);
    *y = y1 + u * (y2 - y1);
}

void    render_floor_and_ceiling(t_game *game)
{
    float   dir_x = cos(game->player->angle);
    float   dir_y = sin(game->player->angle);
    // Plane represents the "width" of the camera sensor
    float   plane_x = -dir_y * tan(PI / 6.0f); 
    float   plane_y = dir_x * tan(PI / 6.0f);
    int     y;

    // We only need to loop through the bottom half of the screen
    y = HEIGHT / 2;
    while (++y < HEIGHT)
    {
        // Ray vectors for the edges of the FOV
        float ray_dir0_x = dir_x - plane_x;
        float ray_dir0_y = dir_y - plane_y;
        float ray_dir1_x = dir_x + plane_x;
        float ray_dir1_y = dir_y + plane_y;

        // Distance from the horizon to the current row
        int p = y - HEIGHT / 2;
        // Camera height (0.5 means middle of the block)
        float pos_z = 0.5 * HEIGHT;
        // Horizontal distance from player to the floor row
        float row_dist = pos_z / p;

        // How much the world coordinate changes for each horizontal pixel
        float step_x = row_dist * (ray_dir1_x - ray_dir0_x) / WIDTH;
        float step_y = row_dist * (ray_dir1_y - ray_dir0_y) / WIDTH;

        // Starting world coordinates at the far left of the current row
        // We divide by BLOCK_SIZE to get the grid-relative position
        float world_x = (game->player->x / (float)BLOCK_SIZE) + row_dist * ray_dir0_x;
        float world_y = (game->player->y / (float)BLOCK_SIZE) + row_dist * ray_dir0_y;

        int x = -1;
        while (++x < WIDTH)
        {
            // Floor casting often creates negative coordinates; 
            // we use floorf to ensure we always get a positive fraction for the texture
            float fx = world_x - floorf(world_x);
            float fy = world_y - floorf(world_y);

            // Map the fraction (0.0 to 1.0) to the texture dimensions
            int tx = (int)(fx * (game->floor_texture.width));
            int ty = (int)(fy * (game->floor_texture.height));

            // Clamp to avoid out-of-bounds due to precision errors
            tx = (tx < 0) ? 0 : (tx >= game->floor_texture.width ? game->floor_texture.width - 1 : tx);
            ty = (ty < 0) ? 0 : (ty >= game->floor_texture.height ? game->floor_texture.height - 1 : ty);

            t_color floor_c;
            t_color ceil_c;

            if (game->floor_texture.img)
                floor_c = get_texture_pixel_by_pos(game->floor_texture, tx, ty);
            else
                floor_c = *game->config.floor_color;

            if (game->ceiling_texture.img)
                ceil_c = get_texture_pixel_by_pos(game->ceiling_texture, tx, ty);
            else
                ceil_c = *game->config.ceiling_color;

            put_pixel(x, y, floor_c, game);
            put_pixel(x, HEIGHT - y - 1, ceil_c, game);

            world_x += step_x;
            world_y += step_y;
        }
    }
}

t_color	get_texture_pixel(t_texture texture, float x_coefficient, float y_coefficient)
{
	t_color	color_rgb;
	int		x;
	int		y;

	/* Clamp coordinates to texture bounds to prevent buffer overflow */
	if (texture.width <= 0 || texture.height <= 0 || !texture.addr)
		return ((t_color){0, 0, 0});

	x = (int)(texture.width * x_coefficient);
	y = (int)(texture.height * y_coefficient);

	if (x < 0)
		x = 0;
	if (x >= texture.width)
		x = texture.width - 1;
	if (y < 0)
		y = 0;
	if (y >= texture.height)
		y = texture.height - 1;
	color_rgb = get_texture_pixel_by_pos(texture, x, y);
	return (color_rgb);
}

void	draw_walls(t_game *game, int x, float height, t_direction direction, float ray_x, float ray_y)
{
	float	x_coefficient;
	float	y_coefficient;
	int		wall_start;
	int		wall_end;
	int		y;
	t_color color;

	if (direction == North || direction == South)
		x_coefficient = fmod(ray_x, BLOCK_SIZE) / BLOCK_SIZE;
	else if (direction == West || direction == East)
		x_coefficient = fmod(ray_y, BLOCK_SIZE) / BLOCK_SIZE;

	wall_start = (HEIGHT - height) / 2;
	wall_end = wall_start + height;
	y = wall_start;
	
	while (y < wall_end)
	{
		color = (t_color){255, 0, 0};
		y_coefficient = (y - wall_start) / height;
		// printf("%f %f\n", x_coefficient, y_coefficient);

		/* decide texture by map cell (doors override) */
		{
			int map_x = (int)(ray_x / BLOCK_SIZE);
			int map_y = (int)(ray_y / BLOCK_SIZE);
			char cell = 0;
			if (map_y >= 0 && game->config.map[map_y] && map_x >= 0 && game->config.map[map_y][map_x])
				cell = game->config.map[map_y][map_x];
			if (cell == 'D' && game->door_texture.img)
				color = get_texture_pixel(game->door_texture, x_coefficient, y_coefficient);
			else if (direction == North)
				color = get_texture_pixel(game->north_texture, x_coefficient, y_coefficient);
			else if (direction == South)
				color = get_texture_pixel(game->south_texture, x_coefficient, y_coefficient);
			else if (direction == West)
				color = get_texture_pixel(game->west_texture, x_coefficient, y_coefficient);
			else if (direction == East)
				color = get_texture_pixel(game->east_texture, x_coefficient, y_coefficient);
		}

		put_pixel(x, y, color, game);
		y++;
	}

}

void	draw_line(t_player *player, t_game *game, float angle, int x)
{
	float		cos_angle;
	float		sin_angle;
	float		ray_x;
	float		ray_y;
	int			angle_side;
	int			quarter;
	float		px;
	float		py;
	float		dist;
	int			height;

	t_direction direction;

	cos_angle = cos(angle);
	sin_angle = sin(angle);
	ray_x = player->x;
	ray_y = player->y;
	while (!touch(ray_x, ray_y, game))
	{
		// put_pixel(ray_x, ray_y, (t_color){255, 0, 0}, game);
		ray_x += cos_angle;
		ray_y += sin_angle;
	}

	quarter = calculate_quarter(angle);
	angle_side = near_angle(quarter, player->x, player->y, ray_x, ray_y, &px, &py);
	direction = wall_direction(quarter, angle_side);
	direction = check_adjacent(game, ray_x / BLOCK_SIZE, ray_y / BLOCK_SIZE, direction, quarter);
	lines_intersection_point(player->x, player->y, ray_x, ray_y, px, py, quarter, angle_side, &ray_x, &ray_y);
	// draw_square(ray_x, ray_y, 10, color, game);
	dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
	game->wall_dist[x] = dist;
	height = (BLOCK_SIZE / dist) * (WIDTH / 2);
	draw_walls(game, x, height, direction, ray_x, ray_y);
}

long	current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void	draw_fps(t_game *game, long *last_time, int *frames)
{
	long	now_time;
	char	*fps_str;

	(void)game;
	now_time = current_time_ms();
	(*frames)++;
	if (now_time - *last_time >= 1000)
	{
		fps_str = ft_itoa(*frames);
		printf("FPS = %s\n", fps_str);
		free(fps_str);
		*frames = 0;
		*last_time = now_time;
	}
}

void	draw_gun(t_game *game)
{
	t_texture	tex;
	int		x_off;
	int		y_off;
	int		x;
	int		y;
	t_color	color;

	tex = game->gun_texture;
	if (!tex.img)
		return ;
	/* draw texture at bottom-center */
	x_off = (WIDTH - tex.width) / 2;
	y_off = HEIGHT - tex.height;
	y = 0;
	while (y < tex.height)
	{
		x = 0;
		while (x < tex.width)
		{
			color = get_texture_pixel_by_pos(tex, x, y);
			/* skip fully transparent pixels if present (None in XPM usually comes as 0) */
			if (!(color.red == 0 && color.green == 0 && color.blue == 0))
				put_pixel(x_off + x, y_off + y, color, game);
			x++;
		}
		y++;
	}
}

static int	bullet_hole_size(float dist)
{
	int	size;

	size = (int)(60 / dist);
	if (size < 1)
		size = 1;
	if (size > 14)
		size = 14;
	return (size);
}

static void	draw_bullet_hole(t_game *game, float hole_x, float hole_y)
{
	float	dist;
	float	angle_to_hole;
	float	delta;
	float	fov;
	float	projected_x;
	int		size;
	int		center_x;
	int		center_y;
	int		dx;
	int		dy;

	dist = fixed_dist(game->player->x, game->player->y, hole_x, hole_y, game);
	if (dist <= 0)
		return ;
	angle_to_hole = atan2(hole_y - game->player->y, hole_x - game->player->x);
	delta = angle_to_hole - game->player->angle;
	while (delta <= -PI)
		delta += 2 * PI;
	while (delta > PI)
		delta -= 2 * PI;
	fov = PI / 3.0f;
	if (fabs(delta) > fov / 2.0f)
		return ;
	projected_x = (WIDTH / 2.0f) + (delta) * (WIDTH / fov);
	center_x = (int)projected_x;
	center_y = HEIGHT / 2;
	size = bullet_hole_size(dist);
	dy = -size;
	while (dy <= size)
	{
		dx = -size;
		while (dx <= size)
		{
			if (dx * dx + dy * dy <= size * size)
				put_pixel(center_x + dx, center_y + dy, (t_color){20, 20, 20}, game);
			dx++;
		}
		dy++;
	}
}

void	draw_bullet_holes(t_game *game)
{
	int	i;

	i = 0;
	while (i < BULLET_HOLE_MAX)
	{
		if (game->bullet_holes[i].active)
			draw_bullet_hole(game, game->bullet_holes[i].x,
				game->bullet_holes[i].y);
		i++;
	}
}

static void	draw_life(t_game *game)
{
	char	life_text[32];

	snprintf(life_text, sizeof(life_text), "LIFE: %d", game->life);
	mlx_string_put(game->mlx, game->win, 20, 20, 0x00FF4444, life_text);
}

int	draw_loop(t_game *game)
{
	float	dir_x;
	float	dir_y;
	float	plane_x;
	float	plane_y;
	int		y;

	dir_x = cos(game->player->angle);
	dir_y = sin(game->player->angle);
	plane_x = -dir_y * tan(PI / 6.0f);
	plane_y = dir_x * tan(PI / 6.0f);
	y = HEIGHT / 2;
	while (++y < HEIGHT)
	{
		float	ray_dir0_x;
		float	ray_dir0_y;
		float	ray_dir1_x;
		float	ray_dir1_y;
		int		p;
		float	pos_z;
		float	row_dist;
		float	step_x;
		float	step_y;
		float	world_x;
		float	world_y;
		int		x;

		ray_dir0_x = dir_x - plane_x;
		ray_dir0_y = dir_y - plane_y;
		ray_dir1_x = dir_x + plane_x;
		ray_dir1_y = dir_y + plane_y;
		p = y - HEIGHT / 2;
		pos_z = 0.5f * HEIGHT;
		row_dist = pos_z / p;
		step_x = row_dist * (ray_dir1_x - ray_dir0_x) / WIDTH;
		step_y = row_dist * (ray_dir1_y - ray_dir0_y) / WIDTH;
		world_x = (game->player->x / (float)BLOCK_SIZE) + row_dist * ray_dir0_x;
		world_y = (game->player->y / (float)BLOCK_SIZE) + row_dist * ray_dir0_y;
		x = -1;
		while (++x < WIDTH)
		{
			float	fx;
			float	fy;
			int		floor_tx;
			int		floor_ty;
			int		ceil_tx;
			int		ceil_ty;
			t_color	floor_c;
			t_color	ceil_c;

			fx = world_x - floorf(world_x);
			fy = world_y - floorf(world_y);
			floor_tx = (int)(fx * game->floor_texture.width);
			floor_ty = (int)(fy * game->floor_texture.height);
			if (floor_tx < 0)
				floor_tx = 0;
			if (floor_ty < 0)
				floor_ty = 0;
			if (game->floor_texture.width > 0
				&& floor_tx >= game->floor_texture.width)
				floor_tx = game->floor_texture.width - 1;
			if (game->floor_texture.height > 0
				&& floor_ty >= game->floor_texture.height)
				floor_ty = game->floor_texture.height - 1;
			ceil_tx = (int)(fx * game->ceiling_texture.width);
			ceil_ty = (int)(fy * game->ceiling_texture.height);
			if (ceil_tx < 0)
				ceil_tx = 0;
			if (ceil_ty < 0)
				ceil_ty = 0;
			if (game->ceiling_texture.width > 0
				&& ceil_tx >= game->ceiling_texture.width)
				ceil_tx = game->ceiling_texture.width - 1;
			if (game->ceiling_texture.height > 0
				&& ceil_ty >= game->ceiling_texture.height)
				ceil_ty = game->ceiling_texture.height - 1;
			if (game->floor_texture.img)
				floor_c = get_texture_pixel_by_pos(game->floor_texture,
					floor_tx, floor_ty);
			else
				floor_c = *game->config.floor_color;
			if (game->ceiling_texture.img)
				ceil_c = get_texture_pixel_by_pos(game->ceiling_texture,
					ceil_tx, ceil_ty);
			else
				ceil_c = *game->config.ceiling_color;
			put_pixel(x, y, floor_c, game);
			put_pixel(x, HEIGHT - y - 1, ceil_c, game);
			world_x += step_x;
			world_y += step_y;
		}
	}
