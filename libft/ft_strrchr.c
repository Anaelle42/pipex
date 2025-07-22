/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:36:07 by ahenault          #+#    #+#             */
/*   Updated: 2024/06/14 16:45:20 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s && s[i])
		i++;
	while (s && i >= 0)
	{
		if (s[i] == (const char)c)
			return ((char *)(&s[i]));
		i--;
	}
	return (NULL);
}
