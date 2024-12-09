/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_dine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-09 01:21:27 by ryomori           #+#    #+#             */
/*   Updated: 2024-12-09 01:21:27 by ryomori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	dine_single_philosophers(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	printf("%u %d thinking\n", get_ms_time(), philo->id);
	usleep((philo->shared->time_to_die + 1) * 1000);
	pthread_mutex_lock(&philo->shared->stop_mutex);
	philo->shared->simulation_stop = true;
	pthread_mutex_unlock(&philo->shared->stop_mutex);
	printf("%u %d died\n", get_ms_time(), philo->id);
	pthread_mutex_unlock(philo->left_fork);
}

void	dine_philosohers(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(500);
	while (!is_simulation_stopped(philo->shared) && \
			((int)philo->count == -1 || philo->count > 0))
	{
		func_think(philo);
		func_eat(philo);
		if (philo->count > 0)
			philo->count--;
		if (philo->count == 0)
		{
			pthread_mutex_lock(&philo->shared->finish_mutex);
			philo->shared->finished_philos++;
			if (philo->shared->finished_philos == philo->shared->philo_num)
			{
				pthread_mutex_lock(&philo->shared->stop_mutex);
				philo->shared->simulation_stop = true;
				pthread_mutex_unlock(&philo->shared->stop_mutex);
			}
			pthread_mutex_unlock(&philo->shared->finish_mutex);
		}
		func_sleep(philo);
	}
}

void	*dine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->shared->philo_num == 1)
	{
		dine_single_philosophers(philo);
		return (NULL);
	}
	dine_philosohers(philo);
	return (NULL);
}
