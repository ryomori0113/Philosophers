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

void	clean_up(t_shared *shared_date)
{
	unsigned int	i;

	i = 0;
	while (i < shared_date->philo_num)
	{
		pthread_mutex_destroy(&shared_date->forks[i]);
		pthread_mutex_destroy(&shared_date->philo[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&shared_date->stop_mutex);
	pthread_mutex_destroy(&shared_date->finish_mutex);
	free(shared_date->threads);
	free(shared_date->forks);
	free(shared_date->philo);
}

int	main(int argc, char **argv)
{
	t_shared		shared_date;

	if (init_resorces(argc, argv, &shared_date))
		return (1);
	run_philosophers(&shared_date);
	clean_up(&shared_date);
	return (0);
}
