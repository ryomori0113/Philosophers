#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "philo.h"

unsigned int get_ms_time(void)
{
	unsigned int ms;
	struct timeval time;

	gettimeofday(&time, NULL);
	ms = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (ms);
}

//==============================================================================
// atoi
//==============================================================================
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}

static int	ft_skipspace(int c)
{
	if (c == ' ' || c == '\n' || c == '\t'
		|| c == '\r' || c == '\f' || c == '\v')
		return (1);
	return (0);
}

static long long int max_atoi(const char *str)
{
	long long int	result;
	long long int	l_divis;
	char		l_remain;

	result = 0;
	l_divis = LONG_MAX / 10;
	l_remain = LONG_MAX % 10 + '0';
	while (ft_isdigit(*str))
	{
		if (l_divis < result || (l_divis == result && l_remain < *str))
			return (LONG_MAX);
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result);
}

static long long int	min_atoi(const char *str)
{
	long long int	result;
	long long int	l_divis;
	char		l_remain;

	result = 0;
	l_divis = LONG_MIN / 10;
	l_remain = (LONG_MIN % 10 * -1) + '0';
	while (ft_isdigit(*str))
	{
		if (l_divis > result || (l_divis == result && l_remain < *str))
			return (LONG_MIN);
		result = result * 10 - (*str - '0');
		str++;
	}
	return (result);
}

long long int	ft_atoi(const char *str)
{
	int		i;
	int		flag;
	long long int	nbr;

	i = 0;
	flag = 0;
	nbr = 0;
	while (ft_skipspace(*str) == 1)
		str++;
	if (*str == '-')
	{
		str++;
		return ((int)min_atoi(str));
	}
	else if (*str == '+')
		str++;
	return (max_atoi(str));
}
//==============================================================================
// isdigit
//==============================================================================
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

//==============================================================================
// is_simulation_stopped
//==============================================================================
bool is_simulation_stopped(t_shared *shared)
{
	bool stopped;
	pthread_mutex_lock(&shared->stop_mutex);
	stopped = shared->simulation_stop;
	pthread_mutex_unlock(&shared->stop_mutex);
	return stopped;
}

//==============================================================================
// func_think
//==============================================================================
void func_think(t_philo *philo)
{
	if (is_simulation_stopped(philo->shared))
		return;
	printf("%u %d is thinking\n", get_ms_time(), philo->id);
	usleep(100);
}
//==============================================================================
// func_eat
//==============================================================================
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
		if (is_simulation_stopped(philo->shared))
		{
			pthread_mutex_unlock(philo->right_fork);
			return false;
		}
		pthread_mutex_lock(philo->left_fork);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		if (is_simulation_stopped(philo->shared))
		{
			pthread_mutex_unlock(philo->left_fork);
			return false;
		}
		pthread_mutex_lock(philo->right_fork);
	}
	return true;
}
void	func_eat(t_philo *philo)
{
	if (is_simulation_stopped(philo->shared))
		return;
	if (!lock_forks(philo))
		return;
	if(is_simulation_stopped(philo->shared))
	{
		unlock_forks(philo);
		return;
	}
	printf("%u %d is eating\n", get_ms_time(), philo->id);
	usleep (philo->shared->time_to_eat * 1000);

	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_ms_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	unlock_forks(philo);
}
//==============================================================================
// func_sleep
//==============================================================================
void func_sleep(t_philo *philo)
{
    if (is_simulation_stopped(philo->shared))
        return;

    printf("%u %d is sleeping\n", get_ms_time(), philo->id);
    usleep(philo->shared->time_to_sleep * 1000);
}
//==============================================================================
// monitor_philosophers
//==============================================================================
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
	int	i;
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
//==============================================================================
// dine
//==============================================================================

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
	while(!is_simulation_stopped(philo->shared) && (philo->count == -1 || philo->count > 0))
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
			break;
		}
		func_sleep(philo);
	}
}

void	*dine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	if (philo->shared->philo_num == 1)
	{
		dine_single_philosophers(philo);
		return NULL;
	}
	dine_philosohers(philo);
	return NULL;
}

//==============================================================================
// parse_argument
//==============================================================================
// コマンドライン引数を検証し、整数に変換
int parse_argument(const char *arg)
{
    int value = ft_atoi(arg);
    if (value <= 0)
    {
        ft_putstr_fd("Error: Invalid argument.\n", 2);
        exit(EXIT_FAILURE);
    }
    return value;
}

//==============================================================================
// error_check
//==============================================================================
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


//==============================================================================
// main
//==============================================================================
// メイン関数

int	parse_set(int argc, char **argv, t_shared *shared)
{
	int	meal_count;
	if (args_check(argc, argv))
		return 1;
	shared->philo_num = parse_argument(argv[1]);
	shared->time_to_die = parse_argument(argv[2]);
	shared->time_to_eat = parse_argument(argv[3]);
	shared->time_to_sleep = parse_argument(argv[4]);
	shared->simulation_stop = false;
	shared->finished_philos = 0;
	shared->meal_count = -1;
	if(argc == 6)
		shared->meal_count = parse_argument(argv[5]);
	else
		shared->meal_count = -1;
	return 0;
}

void	philosohers_create(t_shared *shared, t_philo *philo, pthread_t *threads, pthread_mutex_t *forks)
{
	int i;

	while(i < shared->philo_num)
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
int	init_resorces(int argc, char **argv, t_shared *shared_date, pthread_t **threads, pthread_mutex_t **forks, t_philo **philo)
{
	if (parse_set(argc, argv, shared_date))
		return 1;
	*threads = malloc(sizeof(pthread_t) * shared_date->philo_num);
	*forks = malloc(sizeof(pthread_mutex_t) * shared_date->philo_num);
	*philo = malloc(sizeof(t_philo) * shared_date->philo_num);
	if (!threads || !forks || !philo)
	{
		fprintf(stderr, "Error: Memory allocation failed.\n");
		return 1;
	}
	pthread_mutex_init(&shared_date->finish_mutex, NULL);
	pthread_mutex_init(&shared_date->stop_mutex, NULL);
	return 0;
}

void	run_philosophers(t_shared *shared, t_philo *philo, pthread_t *threads, pthread_mutex_t *forks)
{
	pthread_t monitor_thread;
	philosohers_create(shared, philo, threads, forks);
	pthread_create(&monitor_thread, NULL, monitor_philosophers, philo);

	int i = 0;
	while(i < shared->philo_num)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
}

void	clean_up(t_shared *shared_date, t_philo *philo, pthread_t *threads, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while(i < shared_date->philo_num)
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

int main(int argc, char **argv)
{
	t_shared	shared_date;
	pthread_t	*threads;
	pthread_mutex_t	*forks;
	t_philo	*philo;
	
	if(init_resorces(argc, argv, &shared_date, &threads, &forks, &philo))
		return 1;

	run_philosophers(&shared_date, philo, threads, forks);
	clean_up(&shared_date, philo, threads, forks);
	return 0;
}
