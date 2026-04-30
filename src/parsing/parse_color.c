/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 15:21:05 by pargev            #+#    #+#             */
/*   Updated: 2026/02/28 23:38:45 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_color2(char *color_text, int *color, int min, int max)
{
	int	i;

	i = 0;
	while (color_text[i])
	{
		if (!ft_isdigit(color_text[i]))
			return (0);
		i++;
	}
	*color = ft_atoi(color_text);
	if (*color < min || *color > max)
		return (0);
	return (1);
}

t_color	*parse_color(char *color_text)
{
	char	**color_matrix;
	int		len;
	t_color	*color;

	color_matrix = ft_split(color_text, ',');
	len = 0;
	while (color_matrix[len])
		len++;
	if (len != 3)
	{
		free_string_array(color_matrix);
		return (free(color_text), NULL);
	}
	color = (t_color *)malloc(sizeof(t_color));
	if (!parse_color2(color_matrix[0], &color->red, 0, 255)
		|| !parse_color2(color_matrix[1], &color->green, 0, 255)
		|| !parse_color2(color_matrix[2], &color->blue, 0, 255))
	{
		free(color);
		free_string_array(color_matrix);
		return (free(color_text), NULL);
	}
	free_string_array(color_matrix);
	return (free(color_text), color);
}
