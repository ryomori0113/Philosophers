/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-09 08:40:05 by ryomori           #+#    #+#             */
/*   Updated: 2024-12-09 08:40:05 by ryomori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	sharse_memory(t_shared *shared_date)
{
	shared_date->threads = malloc(sizeof(pthread_t) * shared_date->philo_num);
	if (!shared_date->threads)
	{
		ft_putstr_fd("Error: Memory allocation failed.\n", 2);
		return (1);
	}
	shared_date->forks = malloc(sizeof(pthread_mutex_t) * \
				shared_date->philo_num);
	if (!shared_date->forks)
	{
		free(shared_date->threads);
		ft_putstr_fd("Error: Memory allocation failed.\n", 2);
		return (1);
	}
	shared_date->philo = malloc(sizeof(t_philo) * shared_date->philo_num);
	if (!shared_date->philo)
	{
		free(shared_date->threads);
		free(shared_date->forks);
		ft_putstr_fd("Error: Memory allocation failed.\n", 2);
		return (1);
	}
	return (0);
}

void	ft_usleep(unsigned int ms)
{
	unsigned int	start_ms;
	unsigned int	now_ms;

	start_ms = get_ms_time();
	while (1)
	{
		now_ms = get_ms_time();
		if (ms <= now_ms - start_ms)
			break ;
		usleep(10);
	}
}