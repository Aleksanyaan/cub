/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 15:21:05 by pargev            #+#    #+#             */
/*   Updated: 2026/03/17 23:18:54 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_string_array(char **string)
{
	int	i;

	i = 0;
	if (string)
	{
		while (string[i])
		{
			free(string[i]);
			i++;
		}
		free(string);
	}
}

void	free_config(t_config *config)
{
	if (config->north_texture)
		free(config->north_texture);
	if (config->south_texture)
		free(config->south_texture);
	if (config->west_texture)
		free(config->west_texture);
	if (config->east_texture)
		free(config->east_texture);
	if (config->floor_texture)
		free(config->floor_texture);
	if (config->ceiling_texture)
		free(config->ceiling_texture);
	if (config->floor_color)
		free(config->floor_color);
	if (config->ceiling_color)
		free(config->ceiling_color);
	if (config->map)
		free_string_array(config->map);
}

void	free_textures(t_game *game)
{
	int	i;

	if (game->north_texture.img)
		mlx_destroy_image(game->mlx, game->north_texture.img);
	if (game->south_texture.img)
		mlx_destroy_image(game->mlx, game->south_texture.img);
	if (game->west_texture.img)
		mlx_destroy_image(game->mlx, game->west_texture.img);
	if (game->east_texture.img)
		mlx_destroy_image(game->mlx, game->east_texture.img);
	if (game->floor_texture.img)
		mlx_destroy_image(game->mlx, game->floor_texture.img);
	if (game->ceiling_texture.img)
		mlx_destroy_image(game->mlx, game->ceiling_texture.img);
	if (game->gun_texture.img)
		mlx_destroy_image(game->mlx, game->gun_texture.img);
	i = 0;
	while (i < HEALTH_TEXTURES)
	{
		if (game->health_bars[i])
			mlx_destroy_image(game->mlx, game->health_bars[i]);
		i++;
	}
	if (game->win_img)
		mlx_destroy_image(game->mlx, game->win_img);
	if (game->lose_img)
		mlx_destroy_image(game->mlx, game->lose_img);
	if (game->door_texture.img)
		mlx_destroy_image(game->mlx, game->door_texture.img);
	if (game->zombie1_texture.img)
		mlx_destroy_image(game->mlx, game->zombie1_texture.img);
	if (game->zombie2_texture.img)
		mlx_destroy_image(game->mlx, game->zombie2_texture.img);
	if (game->zombie3_texture.img)
		mlx_destroy_image(game->mlx, game->zombie3_texture.img);
	if (game->zombie4_texture.img)
		mlx_destroy_image(game->mlx, game->zombie4_texture.img);
	if (game->fit_texture.img)
		mlx_destroy_image(game->mlx, game->fit_texture.img);
	if (game->kit_texture.img)
		mlx_destroy_image(game->mlx, game->kit_texture.img);
	if (game->front_texture.img)
		mlx_destroy_image(game->mlx, game->front_texture.img);
	if (game->back_texture.img)
		mlx_destroy_image(game->mlx, game->back_texture.img);
	if (game->left_texture.img)
		mlx_destroy_image(game->mlx, game->left_texture.img);
	if (game->right_texture.img)
		mlx_destroy_image(game->mlx, game->right_texture.img);
}

void	free_all(t_game *game)
{
	if (!game)
		return ;
	free_textures(game);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
		free(game->mlx);
	if (game->player)
		free(game->player);
	free_config(&game->config);
	free(game);
}