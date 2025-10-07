/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eates <eates@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 15:17:20 by eates             #+#    #+#             */
/*   Updated: 2023/07/05 15:17:03 by eates            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t				i;
	unsigned char		a;
	unsigned char		*tmp;

	a = c;
	tmp = (unsigned char *)b;
	i = -1;
	while (++i < len)
		tmp[i] = a;
	return (b);
}