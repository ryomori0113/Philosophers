/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-09 01:27:27 by ryomori           #+#    #+#             */
/*   Updated: 2024-12-09 01:27:27 by ryomori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	get_ms_time(void)
{
	unsigned int	ms;
	struct timeval	time;

	gettimeofday(&time, NULL);
	ms = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (ms);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (s == NULL)
	{
		write (fd, "\0", 1);
		return ;
	}
	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

bool is_simulation_stopped(t_shared *shared)
{
	bool stopped;
	pthread_mutex_lock(&shared->stop_mutex);
	stopped = shared->simulation_stop;
	pthread_mutex_unlock(&shared->stop_mutex);
	return stopped;
}

int	args_check(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		ft_putstr_fd("Usage: [philo_num] [time_to_die] [time_to_eat] [time_to_sleep] [meal_count]\n", 2);
		return 1;
	}
	if(argv[0] == 0 || argv[1] == 0 || argv[2] == 0 || argv[3] == 0 || argv[4] == 0)
	{
		ft_putstr_fd("Error: Invalid argument.\n", 2);
		exit(EXIT_FAILURE);
	}
	return 0;
}

int	parse_argument(const char *arg)
{
	int	value;

	value = ft_atoi(arg);
	if(value <= 0)
	{
		ft_putstr_fd("Error: Invalid argument.\n", 2);
		exit(EXIT_FAILURE);
	}
	return (value);
}
