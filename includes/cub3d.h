/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 15:21:14 by pargev            #+#    #+#             */
/*   Updated: 2026/05/06 00:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libs/minilibx_opengl/mlx.h"
# include "libft.h"
# include <errno.h>
# include <math.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

# define WIDTH 1280
# define HEIGHT 720
# define ESC 53
# define W 13
# define A 0
# define S 1
# define D 2
# define PI 3.14159265358979
# define BLOCK_SIZE 64
# define LEFT 123
# define RIGHT 124
# define SPACE 49
# define ENTER 36
# define E 14
# define PLAYER_RADIUS 4
# define MOVE_SPEED 5
# define CARTRIDGE_MAX 8
# define BULLET_HOLE_MAX 64
# define ENEMY_MAX 128
# define DOOR_MAX 64
# define DOOR_OPEN_DIST 64.0f
# define HEALTH_TEXTURES 11

typedef struct s_color
{
	int	red;
	int	green;
	int	blue;
} 		 t_color;

typedef struct s_config
{
	char		*north_texture;
	char		*south_texture;
	char		*west_texture;
	char		*east_texture;
	char		*floor_texture;
	char		*ceiling_texture;
	t_color		*floor_color;
	t_color		*ceiling_color;
	char		**map;
} 		 t_config;

typedef struct s_texture
{
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	int			width;
	int			height;
} 		t_texture;

typedef struct s_cartridge
{
	int		active;
	float		x;
	float		y;
	float		angle;
} 		t_cartridge;

typedef struct s_bullet_hole
{
	int		active;
	float		x;
	float		y;
} 		t_bullet_hole;

typedef struct s_enemy
{
	int		active;
	int		type;
	float		x;
	float		y;
	float		angle;
	float		last_dist_to_player;
	long		last_progress_time;
	int		ghost_mode;
} 		t_enemy;

typedef enum e_state
{
	STATE_PLAYING,
	STATE_WIN,
	STATE_LOSE
} 	t_state;

typedef struct s_door
{
	int		open;
	float		x;
	float		y;
} 		t_door;

typedef struct s_player
{
	float		x;
	float		y;
	float		angle;

	int			key_up;
	int			key_down;
	int			key_left;
	int			key_right;

	int			left_rotate;
	int			right_rotate;
	t_cartridge	cartridges[CARTRIDGE_MAX];
} 		t_player;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	t_player	*player;

	char		*data;
	int			bpp;
	int			size_line;
	int			endian;

	t_config	config;
	t_texture	north_texture;
	t_texture	south_texture;
	t_texture	door_texture;
	t_texture	west_texture;
	t_texture	east_texture;
	t_texture	floor_texture;
	t_texture	ceiling_texture;
	t_texture	gun_texture;
	void		*health_bars[HEALTH_TEXTURES];
	void		*win_img;
	void		*lose_img;
	int			win_w;
	int			win_h;
	int			lose_w;
	int			lose_h;
	t_state		current_state;
	t_texture	zombie1_texture;
	t_texture	zombie2_texture;
	t_texture	zombie3_texture;
	t_texture	zombie4_texture;
	t_texture	fit_texture;
	t_texture	kit_texture;
	t_texture	front_texture;
	t_texture	back_texture;
	t_texture	left_texture;
	t_texture	right_texture;
	t_bullet_hole	bullet_holes[BULLET_HOLE_MAX];
	t_door		doors[DOOR_MAX];
	int		door_count;
	int		bullet_hole_index;
	t_enemy		enemies[ENEMY_MAX];
	int			enemy_count;
	int		enemies_alive;
	float		wall_dist[WIDTH];
	double		frame_scale;
	int		life;
	long		last_damage_time;
} 		t_game;

typedef enum direction
{
	North,
	South,
	West,
	East
} 	t_direction;

// ========== Parsing ==========
t_config		parse(char *config_path);
void			cheack_config(t_config *config);
int				check_file_extension(char *path, char *extension);
t_color			*parse_color(char *color_text);
char			**read_config(char *path);

// ========= utils =========
void			exit_with_error(char *error_message);
void			free_and_exit(t_config *config, char **string,
					char *error_message);
void			free_string_array(char **string);
void			free_config(t_config *config);
int				ft_str_only(const char *s, const char *allowed);
char			*ft_strdup_free(char *s1);
void			free_all(t_game *game);

// ========= render =========
void			init_player(t_player *player);
int				key_press(int keycode, t_game *game);
int				key_release(int keycode, t_game *game);
void			move_player(t_player *player, t_game *game);
void			shoot_player(t_player *player);
void			update_shoot(t_player *player, t_game *game);
void			draw_shoot(t_game *game);
void			draw_bullet_holes(t_game *game);
void			draw_pickups(t_game *game);
void			init_enemies(t_game *game);
void		init_doors(t_game *game);
void		update_doors(t_game *game);
void			update_enemies(t_game *game);
void			draw_enemies(t_game *game);
int				hit_enemy(float x, float y, t_game *game);
int			close_window(t_game *game);
void			check_player_death(t_game *game);
int				touch(float px, float py, t_game *game);
float			fixed_dist(float x1, float y1, float x2, float y2,
						 t_game *game);
long			current_time_ms(void);
void			put_pixel(int x, int y, t_color color, t_game *game);
void			draw_square(int x, int y, int size, t_color color, t_game *game);
void			draw_map(t_game *game);
void			render_floor_and_ceiling(t_game *game);
void			draw_line(t_player *player, t_game *game, float start_x, int i);
void			draw_minimap(t_game *game);
void			load_health_bars(t_game *game);
void			draw_health_bar(t_game *game, int health);
void			load_ui_assets(t_game *game);
void			check_victory(t_game *game);
void			draw_end_screen(t_game *game);
int				draw_loop(t_game *game);
void			init_game(t_game *game, t_config config);
char			**get_map(void);
int				is_wall(double x, double y, t_game *game);
int				is_circle_colliding_with_wall(double x, double y, double radius,
						 t_game *game);
int			is_door_blocking(double x, double y, t_game *game);
t_door		*get_door_at_map_pos(int map_x, int map_y, t_game *game);
void		try_open_door(t_game *game);

#endif