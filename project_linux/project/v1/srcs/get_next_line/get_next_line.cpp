/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 14:53:17 by froussel          #+#    #+#             */
/*   Updated: 2020/09/12 17:07:16 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.hpp"

static int	free_all(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
	return (-1);
}

static int	is_line(char *str)
{
	int		i;

	i = -1;
	while (str[++i])
		if (str[i] == '\n')
			return (i);
	return (-1);
}

static int	give_line(char **str, char **line, int ret)
{
	char	*s;
	int		len;

	s = NULL;
	if (!*str || !**str)
	{
		if (!(*line = ft_strdup("\0")))
			return (free_all(str));
	}
	else if ((len = is_line(*str)) >= 0)
	{
		if (!(*line = ft_substr(*str, 0, len)))
			return (free_all(str));
		if (!(s = ft_substr(*str, len + 1, ft_strlen(*str))))
			return (free_all(str));
		ret = 1;
	}
	else
	{
		if (!(*line = ft_substr(*str, 0, ft_strlen(*str))))
			return (free_all(str));
	}
	free_all(str);
	*str = s;
	return (ret);
}

int			get_next_line(int fd, char **line)
{
	static char	*str = NULL;
	char		buff[BUFFER_SIZE + 1];
	char		*new_str;
	ssize_t		i;

	if (!line || fd < 0)
		return (free_all(&str));
	while ((i = read(fd, buff, BUFFER_SIZE)) > 0)
	{
		buff[i] = '\0';
		if (!(new_str = ft_strjoin(str, buff)))
			return (free_all(&str));
		free_all(&str);
		str = new_str;
		if (is_line(str) >= 0)
			break ;
	}
	if (i < 0)
		return (free_all(&str));
	return (give_line(&str, line, 0));
}
