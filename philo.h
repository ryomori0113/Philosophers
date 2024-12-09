/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryomori <ryomori@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-09 01:45:58 by ryomori           #+#    #+#             */
/*   Updated: 2024-12-09 01:45:58 by ryomori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>
# include <stdlib.h>
# include <limits.h>

typedef struct s_philo	t_philo;
typedef struct s_shared	t_shared;

typedef struct s_shared
{
	unsigned int	philo_num;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	meal_count;
	bool			simulation_stop;
	pthread_mutex_t	stop_mutex;
	unsigned int	finished_philos;
	pthread_mutex_t	finish_mutex;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	t_philo			*philo;
}	t_shared;

typedef struct s_philo
{
	unsigned int	id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	unsigned int	count;
	pthread_mutex_t	meal_mutex;
	unsigned int	last_meal_time;
	t_shared		*shared;
	pthread_t		thread;
}	t_philo;

// プロトタイプ宣言
unsigned int	get_ms_time(void);
void			func_think(t_philo *philo);
void			func_eat(t_philo *philo);
void			func_sleep(t_philo *philo);
void			*dine(void *arg);
void			*monitor_philosophers(void *arg);
bool			is_simulation_stopped(t_shared *shared);
void			ft_putstr_fd(char *s, int fd);// utils.c
unsigned int	get_ms_time(void);
bool			is_simulation_stopped(t_shared *shared);
int				args_check(int argc, char **argv);
int				parse_argument(const char *arg);
long long int	ft_atoi(const char *str);// utils_ft_atoi.c
int				parse_set(int argc, char **argv, t_shared *shared);
void			philosohers_create(t_shared *shared, t_philo *philo,
					pthread_t *threads, pthread_mutex_t *forks);
int				init_resorces(int argc, char **argv, t_shared *shared_date);
void			run_philosophers(t_shared *shared);//reserve_p
void			func_sleep(t_philo *philo);//philo_sleeping_thinking.c
void			func_think(t_philo *philo);
void			*monitor_philosophers(void *arg);//philo_moniter.c
void			monitor_philo(t_philo *philo, t_shared *shared);
bool			check_philosophers_dide(t_philo *philo,
					t_shared *shared, int i);
void			moniter_single_philosophers(t_shared *shared);
void			func_eat(t_philo *philo);//philo_eating.c
bool			lock_forks(t_philo *philo);
void			unlock_forks(t_philo *philo);
void			*dine(void *arg);//philo_dine.c
void			dine_philosohers(t_philo *philo);
void			dine_single_philosophers(t_philo *philo);
void			clean_up(t_shared *shared_date);//main.c

#endif
