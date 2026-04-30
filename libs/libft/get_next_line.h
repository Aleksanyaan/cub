/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 13:11:23 by zaleksan          #+#    #+#             */
/*   Updated: 2025/05/03 19:58:03 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

size_t	ft_strlen_gnl(const char *src);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin_gnl(char *s1, char *s2);
char	*get_next_line(int fd);

#endif