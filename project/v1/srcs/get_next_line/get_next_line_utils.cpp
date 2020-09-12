/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <froussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 14:54:08 by froussel          #+#    #+#             */
/*   Updated: 2020/09/12 17:08:20 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.hpp"

size_t	ft_strlen(const char *s)
{
	size_t i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *str1, char const *str2)
{
	char	*res;
	char	*beg;
	int		len;

	if (!str1 && !str2)
		return (0);
	len = 0;
	if (str1)
		len = ft_strlen(str1);
	if (str2)
		len += ft_strlen(str2);
	if (!(res = static_cast<char *>(malloc(sizeof(*res) * (len + 1)))))
		return (0);
	beg = res;
	if (str1)
		while (*str1)
			*res++ = *str1++;
	if (str2)
		while (*str2)
			*res++ = *str2++;
	*res = '\0';
	return (beg);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ps;
	size_t	i;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		len = 0;
	else
		len -= start;
	if (!(ps = static_cast<char *>(malloc(sizeof(*ps) * (len + 1)))))
		return (0);
	i = 0;
	while (len-- > 0 && s[start + i])
	{
		ps[i] = s[start + i];
		i++;
	}
	ps[i] = '\0';
	return (ps);
}

char	*ft_strdup(const char *s1)
{
	size_t	i;
	char	*ps;

	i = 0;
	while (s1[i])
		i++;
	if (!(ps = static_cast<char *>(malloc(sizeof(*ps) * (i + 1)))))
		return (0);
	i = 0;
	while (s1[i])
	{
		ps[i] = s1[i];
		i++;
	}
	ps[i] = '\0';
	return (ps);
}
