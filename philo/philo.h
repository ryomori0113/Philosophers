#ifndef PHILO_H
# define PHILO_H

 #include <pthread.h>

typedef struct s_philo
{
    int     id;
    pthread_mutex_t left_fork;
    pthread_mutex_t right_fork;
    int     count;
    unsigned int  eat_time;
    unsigned int    sleep_time;    

} t_philo;

#endif