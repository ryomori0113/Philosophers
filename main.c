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

    printf("%u %d is eating\n", get_ms_time(), philo->id);
    usleep(philo->shared->time_to_eat * 1000);
	
    if (is_simulation_stopped(philo->shared)) {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        return;
    }

    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal_time = get_ms_time();
    pthread_mutex_unlock(&philo->meal_mutex);

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
	int i;
    t_philo *philo = (t_philo *)arg;
    t_shared *shared = philo[0].shared;

	if (shared->philo_num == 1)
    {
        // time_to_die を超える時間待機
        usleep((shared->time_to_die + 1) * 1000);
        
        // シミュレーション停止
        pthread_mutex_lock(&shared->stop_mutex);
        shared->simulation_stop = true;
        pthread_mutex_unlock(&shared->stop_mutex);
        
        return NULL;
    }
    while (!is_simulation_stopped(shared))
    {
		i = 0;
        while (i < shared->philo_num)
        {
            pthread_mutex_lock(&philo[i].meal_mutex);
            unsigned int current_time = get_ms_time();

            if (current_time - philo[i].last_meal_time > shared->time_to_die)
            {
                pthread_mutex_lock(&shared->stop_mutex);
                shared->simulation_stop = true;
                pthread_mutex_unlock(&shared->stop_mutex);

                printf("%u %d died\n", current_time, philo[i].id);
                pthread_mutex_unlock(&philo[i].meal_mutex);
				// exit(EXIT_FAILURE);//引数のphiloが1のときの場合のみ終わるようにする。＞＞＞＞＞＞＞＞直し必要？＜＜＜＜＜＜＜＜
                return NULL;//↑これをコメントアウトして、exit(EXIT_FAILURE);を追加
            }
            pthread_mutex_unlock(&philo[i].meal_mutex);
			i++;
        }
		pthread_mutex_lock(&shared->finish_mutex);
        if (shared->finished_philos == shared->philo_num)
        {
            pthread_mutex_unlock(&shared->finish_mutex);
            return NULL; // 正常終了
        }
        pthread_mutex_unlock(&shared->finish_mutex);
        usleep(1000);
    }
    return NULL;
}

void *dine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

	//1人の哲学者の場合
    if (philo->shared->philo_num == 1)
    {
        // 片方をロック
        pthread_mutex_lock(philo->left_fork);
        printf("%u %d thinking\n", get_ms_time(), philo->id);
        
        // time_to_die 待機
        usleep((philo->shared->time_to_die + 1) * 1000);
        
        // シミュレーション停止と死亡メッセージ
        pthread_mutex_lock(&philo->shared->stop_mutex);
        philo->shared->simulation_stop = true;
        pthread_mutex_unlock(&philo->shared->stop_mutex);
        
        printf("%u %d died\n", get_ms_time(), philo->id);
        
        pthread_mutex_unlock(philo->left_fork);
        return NULL;
    }
    if (philo->id % 2 == 0) {
        usleep(500);
    }

    while (!is_simulation_stopped(philo->shared) && (philo->count == -1 || philo->count > 0))
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
    return NULL;
}

// コマンドライン引数を検証し、整数に変換
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
	int i;

    if (argc != 5 && argc != 6)
    {
        fprintf(stderr, "Usage: %s [philo_num] [time_to_die] [time_to_eat] [time_to_sleep] [meal_count]\n", argv[0]);
        return 1;
    }
	if(argv[0] == 0 || argv[1] == 0 || argv[2] == 0 || argv[3] == 0 || argv[4] == 0)
	{
		fprintf(stderr, "Error: Invalid argument.\n");
		exit(EXIT_FAILURE);
	}

    int philo_num = parse_argument(argv[1]);
    unsigned int time_to_die = parse_argument(argv[2]);
    unsigned int time_to_eat = parse_argument(argv[3]);
    unsigned int time_to_sleep = parse_argument(argv[4]);
    int meal_count;

	if(argc == 6)
		meal_count = parse_argument(argv[5]);
	else
		meal_count = -1;

    pthread_t *threads = malloc(sizeof(pthread_t) * philo_num);
    pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * philo_num);
    t_philo *philo = malloc(sizeof(t_philo) * philo_num);
    t_shared shared_data;

    if (!threads || !forks || !philo)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 1;
    }

    shared_data.philo_num = philo_num;
    shared_data.time_to_die = time_to_die;
    shared_data.time_to_eat = time_to_eat;
    shared_data.time_to_sleep = time_to_sleep;
    shared_data.simulation_stop = false;
	shared_data.finished_philos = 0;
	pthread_mutex_init(&shared_data.finish_mutex, NULL);
    pthread_mutex_init(&shared_data.stop_mutex, NULL);

	i = 0;
    while (i < philo_num)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	
	i = 0;
    while (i < philo_num)
    {
        philo[i].id = i + 1;
        philo[i].left_fork = &forks[i];
        philo[i].right_fork = &forks[(i + 1) % philo_num];
        philo[i].count = meal_count;
        philo[i].shared = &shared_data;
        philo[i].last_meal_time = get_ms_time();
        pthread_mutex_init(&philo[i].meal_mutex, NULL);
        pthread_create(&threads[i], NULL, dine, &philo[i]);
        usleep(200);
		i++;
    }

    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, monitor_philosophers, philo);

	i = 0;
    while(i < philo_num)
    {
        pthread_join(threads[i], NULL);
		i++;
    }

    pthread_join(monitor_thread, NULL);

	i = 0;
    while(i < philo_num)
    {
        pthread_mutex_destroy(&forks[i]);
        pthread_mutex_destroy(&philo[i].meal_mutex);
		i++;
    }
    pthread_mutex_destroy(&shared_data.stop_mutex);
	pthread_mutex_destroy(&shared_data.finish_mutex);


    free(threads);
    free(forks);
    free(philo);

    return 0;
}
__attribute__((destructor))
static void destructor() {
	system("leaks -q a.out");
}