/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:02:01 by zaleksan          #+#    #+#             */
/*   Updated: 2026/03/17 23:14:15 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	check_player_death(t_game *game)
{
	if (game->life > 0)
		return ;
	printf("GAME OVER\n");
	free_all(game);
	exit(0);
}

int	close_window(t_game *game)
{
	if (!game)
		exit(1);
	free_all(game);
	exit(0);
}

void	init_struct(t_game *game)
{
	game->mlx = NULL;
	game->win = NULL;
	game->img = NULL;
	game->data = NULL;
	game->player = NULL;
	game->north_texture.img = NULL;
	game->south_texture.img = NULL;
	game->west_texture.img = NULL;
	game->east_texture.img = NULL;
	game->floor_texture.img = NULL;
	game->ceiling_texture.img = NULL;
	game->gun_texture.img = NULL;
	ft_bzero(&game->door_texture, sizeof(game->door_texture));
	game->door_texture.img = NULL;
	game->zombie1_texture.img = NULL;
	game->zombie2_texture.img = NULL;
	game->zombie3_texture.img = NULL;
	game->zombie4_texture.img = NULL;
	game->fit_texture.img = NULL;
	game->kit_texture.img = NULL;
	game->front_texture.img = NULL;
	game->back_texture.img = NULL;
	game->left_texture.img = NULL;
	game->right_texture.img = NULL;
	ft_bzero(game->bullet_holes, sizeof(game->bullet_holes));
	ft_bzero(game->enemies, sizeof(game->enemies));
	ft_bzero(game->doors, sizeof(game->doors));
	ft_bzero(game->wall_dist, sizeof(game->wall_dist));
	game->bullet_hole_index = 0;
	game->enemy_count = 0;
	game->enemies_alive = 0;
	game->door_count = 0;
	game->frame_scale = 1.0;
	game->life = 100;
	game->last_damage_time = 0;
	game->bpp = 0;
	game->size_line = 0;
	game->endian = 0;
}

static void	load_enemy_texture(t_game *game, t_texture *tex, char *path)
{
	tex->img = mlx_xpm_file_to_image(game->mlx, path, &tex->width, &tex->height);
	if (!tex->img)
	{
		fprintf(stderr, "Failed to load texture: %s\n", path);
		free_all(game);
		exit_with_error(": loading enemy texture\n");
	}
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len,
			&tex->endian);
}

void	read_texture(t_game *game)
{
	t_texture	*north;
	t_texture	*south;
	t_texture	*west;
	t_texture	*east;
	t_texture	*gun;

	north = &game->north_texture;
	south = &game->south_texture;
	west = &game->west_texture;
	east = &game->east_texture;
	north->img = mlx_xpm_file_to_image(game->mlx, game->config.north_texture, &north->width, &north->height);
	south->img = mlx_xpm_file_to_image(game->mlx, game->config.south_texture, &south->width, &south->height);
	west->img = mlx_xpm_file_to_image(game->mlx, game->config.west_texture, &west->width, &west->height);
	east->img = mlx_xpm_file_to_image(game->mlx, game->config.east_texture, &east->width, &east->height);
	gun = &game->gun_texture;
	/* prefer gun1.xpm if present, fallback to gun.xpm */
	gun->img = mlx_xpm_file_to_image(game->mlx, "textures/gun1.xpm", &gun->width, &gun->height);
	if (!gun->img)
		gun->img = mlx_xpm_file_to_image(game->mlx, "textures/gun.xpm", &gun->width, &gun->height);

	/* load door texture if present */
	game->door_texture.img = mlx_xpm_file_to_image(game->mlx, "textures/door.xpm", &game->door_texture.width, &game->door_texture.height);
	if (!north->img)
	{
		fprintf(stderr, "Failed to load texture: %s\n", game->config.north_texture);
		free_all(game);
		exit_with_error(": loading image\n");
	}
	if (!south->img)
	{
		fprintf(stderr, "Failed to load texture: %s\n", game->config.south_texture);
		free_all(game);
		exit_with_error(": loading image\n");
	}
	if (!west->img)
	{
		fprintf(stderr, "Failed to load texture: %s\n", game->config.west_texture);
		free_all(game);
		exit_with_error(": loading image\n");
	}
	if (!east->img)
	{
		fprintf(stderr, "Failed to load texture: %s\n", game->config.east_texture);
		free_all(game);
		exit_with_error(": loading image\n");
	}
	if (!gun->img)
	{
		fprintf(stderr, "Warning: failed to load HUD gun texture: textures/gun.xpm\n");
		/* don't abort for missing HUD gun */
		gun->img = NULL;
	}
	north->addr = mlx_get_data_addr(north->img, &north->bpp, &north->line_len, &north->endian);
	south->addr = mlx_get_data_addr(south->img, &south->bpp, &south->line_len, &south->endian);
	west->addr = mlx_get_data_addr(west->img, &west->bpp, &west->line_len, &west->endian);
	east->addr = mlx_get_data_addr(east->img, &east->bpp, &east->line_len, &east->endian);
	if (game->config.floor_texture)
	{
		game->floor_texture.img = mlx_xpm_file_to_image(game->mlx,
				game->config.floor_texture, &game->floor_texture.width,
				&game->floor_texture.height);
		if (!game->floor_texture.img)
		{
			fprintf(stderr, "Failed to load texture: %s\n",
				game->config.floor_texture);
			free_all(game);
			exit_with_error(": loading floor texture\n");
		}
		game->floor_texture.addr = mlx_get_data_addr(game->floor_texture.img,
				&game->floor_texture.bpp, &game->floor_texture.line_len,
				&game->floor_texture.endian);
	}
	if (game->config.ceiling_texture)
	{
		game->ceiling_texture.img = mlx_xpm_file_to_image(game->mlx,
				game->config.ceiling_texture, &game->ceiling_texture.width,
				&game->ceiling_texture.height);
		if (!game->ceiling_texture.img)
		{
			fprintf(stderr, "Failed to load texture: %s\n",
				game->config.ceiling_texture);
			free_all(game);
			exit_with_error(": loading ceiling texture\n");
		}
		game->ceiling_texture.addr = mlx_get_data_addr(
				game->ceiling_texture.img, &game->ceiling_texture.bpp,
				&game->ceiling_texture.line_len, &game->ceiling_texture.endian);
	}
	if (gun->img)
		gun->addr = mlx_get_data_addr(gun->img, &gun->bpp, &gun->line_len, &gun->endian);
	if (game->door_texture.img)
		game->door_texture.addr = mlx_get_data_addr(game->door_texture.img, &game->door_texture.bpp, &game->door_texture.line_len, &game->door_texture.endian);
	load_enemy_texture(game, &game->zombie1_texture, "textures/zombie1.xpm");
	load_enemy_texture(game, &game->zombie2_texture, "textures/zombie2.xpm");
	load_enemy_texture(game, &game->zombie3_texture, "textures/zombie3.xpm");
	load_enemy_texture(game, &game->zombie4_texture, "textures/zombie4.xpm");
	load_enemy_texture(game, &game->fit_texture, "textures/fit.xpm");
	load_enemy_texture(game, &game->kit_texture, "textures/kit.xpm");
	load_enemy_texture(game, &game->front_texture, "textures/front.xpm");
	load_enemy_texture(game, &game->back_texture, "textures/back.xpm");
	load_enemy_texture(game, &game->left_texture, "textures/left.xpm");
	load_enemy_texture(game, &game->right_texture, "textures/right.xpm");
}

void	init_game(t_game *game, t_config config)
{
	game->config = config;
	init_struct(game);
 	game->player = malloc(sizeof(t_player));
	if (!game->player)
	{
		free_all(game);
		exit(1);
	}
	init_player(game->player);
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		free_all(game);
		exit(1);
	}
	read_texture(game);
	init_enemies(game);
	init_doors(game);
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Cube3D");
	if (!game->win)
	{
		free_all(game);
		exit(1);
	}
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->img)
	{
		free_all(game);
		exit(1);
	}
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
			&game->endian);
	if (!game->data)
	{
		free_all(game);
		exit(1);
	}
}


