/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_libft.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-09 01:25:24 by ryomori           #+#    #+#             */
/*   Updated: 2024-12-09 01:25:24 by ryomori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}

static int	ft_skipspace(int c)
{
	if (c == ' ' || c == '\n' || c == '\t'
		|| c == '\r' || c == '\f' || c == '\v')
		return (1);
	return (0);
}

static long long int	max_atoi(const char *str)
{
	long long int	result;
	long long int	l_divis;
	char			l_remain;

	result = 0;
	l_divis = LONG_MAX / 10;
	l_remain = LONG_MAX % 10 + '0';
	while (ft_isdigit(*str))
	{
		if (l_divis < result || (l_divis == result && l_remain < *str))
			return (LONG_MAX);
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result);
}

static long long int	min_atoi(const char *str)
{
	long long int	result;
	long long int	l_divis;
	char			l_remain;

	result = 0;
	l_divis = LONG_MIN / 10;
	l_remain = (LONG_MIN % 10 * -1) + '0';
	while (ft_isdigit(*str))
	{
		if (l_divis > result || (l_divis == result && l_remain < *str))
			return (LONG_MIN);
		result = result * 10 - (*str - '0');
		str++;
	}
	return (result);
}

long long int	ft_atoi(const char *str)
{
	int				i;
	int				flag;
	long long int	nbr;

	i = 0;
	flag = 0;
	nbr = 0;
	while (ft_skipspace(*str) == 1)
		str++;
	if (*str == '-')
	{
		str++;
		return ((int)min_atoi(str));
	}
	else if (*str == '+')
		str++;
	return (max_atoi(str));
}
