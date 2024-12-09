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

bool	is_simulation_stopped(t_shared *shared)
{
	bool	stopped;

	pthread_mutex_lock(&shared->stop_mutex);
	stopped = shared->simulation_stop;
	pthread_mutex_unlock(&shared->stop_mutex);
	return (stopped);
}

int	args_check(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		ft_putstr_fd("Invalid argument.\n", 2);
		return (1);
	}
	(void) argv;
	return (0);
}

int	parse_argument(const char *arg)
{
	int	value;
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			ft_putstr_fd("Error: Invalid argument\n", 2);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	value = ft_atoi(arg);
	if (value <= 0)
	{
		ft_putstr_fd("Error: Invalid argument.\n", 2);
		exit(EXIT_FAILURE);
	}
	return (value);
}
