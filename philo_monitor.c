/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_moniter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-09 01:20:51 by ryomori           #+#    #+#             */
/*   Updated: 2024-12-09 01:20:51 by ryomori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	moniter_single_philosophers(t_shared *shared)
{
	usleep((shared->time_to_die + 1) * 1000);
	pthread_mutex_lock(&shared->stop_mutex);
	shared->simulation_stop = true;
	pthread_mutex_unlock(&shared->stop_mutex);
}

bool	check_philosophers_dide(t_philo *philo, t_shared *shared, int i)
{
	unsigned int	current_time;

	pthread_mutex_lock(&philo[i].meal_mutex);
	current_time = get_ms_time();
	if(current_time - philo[i].last_meal_time > shared->time_to_die)
	{
		pthread_mutex_lock(&shared->stop_mutex);
		shared->simulation_stop = true;
		pthread_mutex_unlock(&shared->stop_mutex);

		printf("%u %d died\n", get_ms_time(), philo[i].id);
		pthread_mutex_unlock(&philo[i].meal_mutex);
		return true;
	}
	pthread_mutex_unlock(&philo[i].meal_mutex);
	return false;
}

void	monitor_philo(t_philo *philo, t_shared *shared)
{
	unsigned int	i;
	while(!is_simulation_stopped(shared))
	{
		i = 0;
		while(i < shared->philo_num)
		{
			if(check_philosophers_dide(philo, shared, i))
				return;
			i++;
		}
		pthread_mutex_lock(&shared->finish_mutex);
		if (shared->finished_philos == shared->philo_num)
		{
			pthread_mutex_unlock(&shared->stop_mutex);
			return;
		}
		pthread_mutex_unlock(&shared->finish_mutex);
		usleep(1000);
	}
}
void	*monitor_philosophers(void *arg)
{
	t_philo	*philo = (t_philo *)arg;
	t_shared	*shared = philo[0].shared;

	if (shared->philo_num == 1)
	{
		moniter_single_philosophers(shared);
		return NULL;
	}
	monitor_philo(philo, shared);
	return NULL;
}
