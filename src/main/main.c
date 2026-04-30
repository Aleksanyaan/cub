/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 15:21:10 by pargev            #+#    #+#             */
/*   Updated: 2026/03/18 00:01:23 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


int	main(int argc, char	**argv)
{
	t_config	config;
	t_game		*game;

	if (argc != 2)
		return (1);
	
	config = parse(argv[1]);
	printf("%s\n", config.north_texture);
	printf("%s\n", config.south_texture);
	printf("%s\n", config.west_texture);
	printf("%s\n", config.east_texture);
	// printf("%d,%d,%d\n", config.floor_color->red, config.floor_color->green,
	// 	config.floor_color->blue);
	// printf("%d,%d,%d\n", config.ceiling_color->red, config.ceiling_color->green,
	// 	config.ceiling_color->blue);
	// i = 0;
	// while (config.map && config.map[i])
	// {
	// 	printf("%s\n", config.map[i]);
	// 	i++;
	// }

	game = malloc(sizeof(t_game));
	if (!game)
		free_and_exit(&config, NULL, "");
	init_game(game, config);
	mlx_hook(game->win, 2, 1L << 0, key_press, game);
	mlx_hook(game->win, 3, 1L << 1, key_release, game);
	// mlx_hook(game->win, 17, 0, close_window, game);
	mlx_loop_hook(game->mlx, draw_loop, game);
	mlx_loop(game->mlx);
	free_all(game);
}

