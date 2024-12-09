/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reserve_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-09 01:30:52 by ryomori           #+#    #+#             */
/*   Updated: 2024-12-09 01:30:52 by ryomori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_set(int argc, char **argv, t_shared *shared)
{
	if (args_check(argc, argv))
		return (1);
	shared->philo_num = parse_argument(argv[1]);
	shared->time_to_die = parse_argument(argv[2]);
	shared->time_to_eat = parse_argument(argv[3]);
	shared->time_to_sleep = parse_argument(argv[4]);
	shared->simulation_stop = false;
	shared->finished_philos = 0;
	shared->meal_count = -1;
	if (argc == 6)
		shared->meal_count = parse_argument(argv[5]);
	else
		shared->meal_count = -1;
	return (0);
}

void	philosohers_create(t_shared *shared, t_philo *philo,
			pthread_t *threads, pthread_mutex_t *forks)
{
	unsigned int	i;

	i = 0;
	while (i < shared->philo_num)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < shared->philo_num)
	{
		philo[i].id = i + 1;
		philo[i].left_fork = &forks[i];
		philo[i].right_fork = &forks[(i + 1) % shared->philo_num];
		philo[i].count = shared->meal_count;
		philo[i].shared = shared;
		philo[i].last_meal_time = get_ms_time();
		pthread_mutex_init(&philo[i].meal_mutex, NULL);
		pthread_create(&threads[i], NULL, dine, &philo[i]);
		usleep(200);
		i++;
	}
}

int	init_resorces(int argc, char **argv, t_shared *shared_date,
		pthread_t **threads, pthread_mutex_t **forks, t_philo **philo)
{
	if (parse_set(argc, argv, shared_date))
		return (1);
	*threads = malloc(sizeof(pthread_t) * shared_date->philo_num);
	*forks = malloc(sizeof(pthread_mutex_t) * shared_date->philo_num);
	*philo = malloc(sizeof(t_philo) * shared_date->philo_num);
	if (!threads || !forks || !philo)
	{
		fprintf(stderr, "Error: Memory allocation failed.\n");
		return (1);
	}
	pthread_mutex_init(&shared_date->finish_mutex, NULL);
	pthread_mutex_init(&shared_date->stop_mutex, NULL);
	return (0);
}

void	run_philosophers(t_shared *shared, t_philo *philo,
			pthread_t *threads, pthread_mutex_t *forks)
{
	pthread_t		monitor_thread;
	unsigned int	i;

	philosohers_create(shared, philo, threads, forks);
	pthread_create(&monitor_thread, NULL, monitor_philosophers, philo);
	i = 0;
	while (i < shared->philo_num)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
}
