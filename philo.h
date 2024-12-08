#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <stdbool.h>

typedef struct s_shared {
    unsigned int             philo_num;
    unsigned int    time_to_die;
    unsigned int    time_to_eat;
    unsigned int    time_to_sleep;
	unsigned int    meal_count;
    bool            simulation_stop;
    pthread_mutex_t stop_mutex;
    unsigned int             finished_philos; // 完了した哲学者の数
    pthread_mutex_t finish_mutex;    // 完了カウント用のミューテックス
} t_shared;

typedef struct s_philo {
    unsigned int             id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    unsigned int             count;
    pthread_mutex_t  meal_mutex;
    unsigned int    last_meal_time;
    t_shared        *shared;
    pthread_t       thread;
} t_philo;

// プロトタイプ宣言
unsigned int    get_ms_time(void);
void            func_think(t_philo *philo);
void            func_eat(t_philo *philo);
void            func_sleep(t_philo *philo);
void            *dine(void *arg);
void            *monitor_philosophers(void *arg);
bool            is_simulation_stopped(t_shared *shared);

#endif