#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include "philo.h"

unsigned int get_ms_time(void)
{
    unsigned int ms;
    struct timeval time;

    gettimeofday(&time, NULL);
    ms = time.tv_sec * 1000 + time.tv_usec / 1000;
    return (ms);
}

bool is_simulation_stopped(t_shared *shared)
{
    bool stopped;
    pthread_mutex_lock(&shared->stop_mutex);
    stopped = shared->simulation_stop;
    pthread_mutex_unlock(&shared->stop_mutex);
    return stopped;
}

void func_think(t_philo *philo)
{
    if (is_simulation_stopped(philo->shared))
        return;

    printf("%u %d is thinking\n", get_ms_time(), philo->id);
}

void func_eat(t_philo *philo)
{
    if (is_simulation_stopped(philo->shared))
        return;

    if (philo->id % 2 == 0) {
        pthread_mutex_lock(philo->right_fork);
        pthread_mutex_lock(philo->left_fork);
    } else {
        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(philo->right_fork);
    }

    if (is_simulation_stopped(philo->shared)) {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        return;
    }

    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal_time = get_ms_time();
    pthread_mutex_unlock(&philo->meal_mutex);

    printf("%u %d is eating\n", get_ms_time(), philo->id);
    usleep(philo->shared->time_to_eat * 1000);

    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

void func_sleep(t_philo *philo)
{
    if (is_simulation_stopped(philo->shared))
        return;

    printf("%u %d is sleeping\n", get_ms_time(), philo->id);
    usleep(philo->shared->time_to_sleep * 1000);
}

void *monitor_philosophers(void *arg)
{
    t_philo *philosophers = (t_philo *)arg;
    t_shared *shared = philosophers[0].shared;

    while (!is_simulation_stopped(shared))
    {
        for (int i = 0; i < shared->philo_num; i++)
        {
            pthread_mutex_lock(&philosophers[i].meal_mutex);
            unsigned int current_time = get_ms_time();

            if (current_time - philosophers[i].last_meal_time > shared->time_to_die)
            {
                pthread_mutex_lock(&shared->stop_mutex);
                shared->simulation_stop = true;
                pthread_mutex_unlock(&shared->stop_mutex);

                printf("%u %d died\n", current_time, philosophers[i].id);
                pthread_mutex_unlock(&philosophers[i].meal_mutex);
                return NULL;
            }

            pthread_mutex_unlock(&philosophers[i].meal_mutex);
        }
        usleep(1000);
    }
    return NULL;
}

void *dine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    if (philo->id % 2 == 0) {
        usleep(500);
    }

    while (!is_simulation_stopped(philo->shared) && philo->count >= 0)
    {
        func_think(philo);
        func_eat(philo);
        func_sleep(philo);
        philo->count--;
    }
    return NULL;
}

// コマンドライン引数を検証し、整数に変換する
int parse_argument(const char *arg)
{
    int value = atoi(arg);
    if (value <= 0)
    {
        fprintf(stderr, "Error: Invalid argument '%s'. Must be a positive integer.\n", arg);
        exit(EXIT_FAILURE);
    }
    return value;
}

// メイン関数
int main(int argc, char **argv)
{
    if (argc != 6)
    {
        fprintf(stderr, "Usage: %s [philo_num] [time_to_die] [time_to_eat] [time_to_sleep] [meal_count]\n", argv[0]);
        return 1;
    }

    int philo_num = parse_argument(argv[1]);
    unsigned int time_to_die = parse_argument(argv[2]);
    unsigned int time_to_eat = parse_argument(argv[3]);
    unsigned int time_to_sleep = parse_argument(argv[4]);
    int meal_count = parse_argument(argv[5]);

    pthread_t *threads = malloc(sizeof(pthread_t) * philo_num);
    pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * philo_num);
    t_philo *philos = malloc(sizeof(t_philo) * philo_num);
    t_shared shared_data;

    if (!threads || !forks || !philos)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 1;
    }

    shared_data.philo_num = philo_num;
    shared_data.time_to_die = time_to_die;
    shared_data.time_to_eat = time_to_eat;
    shared_data.time_to_sleep = time_to_sleep;
    shared_data.simulation_stop = false;
    pthread_mutex_init(&shared_data.stop_mutex, NULL);

    for (int i = 0; i < philo_num; i++)
    {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < philo_num; i++)
    {
        philos[i].id = i + 1;
        philos[i].left_fork = &forks[i];
        philos[i].right_fork = &forks[(i + 1) % philo_num];
        philos[i].count = meal_count;
        philos[i].shared = &shared_data;
        philos[i].last_meal_time = get_ms_time();
        pthread_mutex_init(&philos[i].meal_mutex, NULL);
        pthread_create(&threads[i], NULL, dine, &philos[i]);
        usleep(20000);
    }

    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, monitor_philosophers, philos);

    for (int i = 0; i < philo_num; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_join(monitor_thread, NULL);

    for (int i = 0; i < philo_num; i++)
    {
        pthread_mutex_destroy(&forks[i]);
        pthread_mutex_destroy(&philos[i].meal_mutex);
    }
    pthread_mutex_destroy(&shared_data.stop_mutex);

    free(threads);
    free(forks);
    free(philos);

    return 0;
}
