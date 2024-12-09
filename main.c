/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-09 01:35:51 by ryomori           #+#    #+#             */
/*   Updated: 2024-12-09 01:35:51 by ryomori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_up(t_shared *shared_date, t_philo *philo,
			pthread_t *threads, pthread_mutex_t *forks)
{
	unsigned int	i;

	i = 0;
	while (i < shared_date->philo_num)
	{
		pthread_mutex_destroy(&forks[i]);
		pthread_mutex_destroy(&philo[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&shared_date->stop_mutex);
	pthread_mutex_destroy(&shared_date->finish_mutex);
	free(threads);
	free(forks);
	free(philo);
}

int	main(int argc, char **argv)
{
	t_shared		shared_date;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	t_philo			*philo;

	if (init_resorces(argc, argv, &shared_date, &threads, &forks, &philo))
		return (1);
	run_philosophers(&shared_date, philo, threads, forks);
	clean_up(&shared_date, philo, threads, forks);
	return (0);
}
