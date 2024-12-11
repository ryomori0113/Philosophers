/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eatint.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-09 01:21:51 by ryomori           #+#    #+#             */
/*   Updated: 2024-12-09 01:21:51 by ryomori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

bool	lock_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		printf("%u %d has taken a fork\n", get_ms_time(), philo->id);
		if (is_simulation_stopped(philo->shared))
		{
			pthread_mutex_unlock(philo->right_fork);
			return (false);
		}
		pthread_mutex_lock(philo->left_fork);
		printf("%u %d is has taken a fork\n", get_ms_time(), philo->id);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		printf("%u %d has taken a fork\n", get_ms_time(), philo->id);
		if (is_simulation_stopped(philo->shared))
		{
			pthread_mutex_unlock(philo->left_fork);
			return (false);
		}
		pthread_mutex_lock(philo->right_fork);
		printf("%u %d has taken a fork\n", get_ms_time(), philo->id);
	}
	return (true);
}

void	func_eat(t_philo *philo)
{
	if (is_simulation_stopped(philo->shared))
		return ;
	if (!lock_forks(philo))
		return ;
	if (is_simulation_stopped(philo->shared))
	{
		unlock_forks(philo);
		return ;
	}
	printf("%u %d is eating\n", get_ms_time(), philo->id);
	ft_usleep (philo->shared->time_to_eat);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_ms_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	unlock_forks(philo);
}
