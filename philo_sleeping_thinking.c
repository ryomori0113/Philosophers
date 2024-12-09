/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sleeping_thinking.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-09 01:23:50 by ryomori           #+#    #+#             */
/*   Updated: 2024-12-09 01:23:50 by ryomori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	func_think(t_philo *philo)
{
	if (is_simulation_stopped(philo->shared))
		return ;
	printf("%u %d is thinking\n", get_ms_time(), philo->id);
	usleep(100);
}

void	func_sleep(t_philo *philo)
{
	if (is_simulation_stopped(philo->shared))
		return ;
	printf("%u %d is sleeping\n", get_ms_time(), philo->id);
	usleep(philo->shared->time_to_sleep * 1000);
}
