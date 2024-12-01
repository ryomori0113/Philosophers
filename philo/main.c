#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "philo.h"
#include <sys/time.h>

unsigned int	get_ms_time(void)
{
	unsigned int	ms;
	struct timeval	time;

	gettimeofday(&time, NULL);
	ms = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (ms);
}

void func_think(t_philo philo)
{
    printf("%u  %u is thinking\n", get_ms_time(), philo.id);

    if(philo.id % 2 == 0)
    {
        pthread_mutex_lock(philo.right_fork);
        pthread_mutex_lock(philo.left_fork);
    }
    else
    {
        pthread_mutex_lock(philo.left_fork);
        pthread_mutex_lock(philo.right_fork);
    }
    // printf("%d  %d is thinking finished!!!  \n", get_ms_time(), philo.id);
}

void func_eat(t_philo philo)
{
    printf("%u  %u is eating\n", get_ms_time(), philo.id);
    usleep(300000);//定数

    pthread_mutex_unlock(philo.left_fork);
    pthread_mutex_unlock(philo.right_fork);
    // printf("%d  %d is eating finished!!! \n", get_ms_time(), philo.id);
}

void func_sleep(t_philo philo)
{
    printf("%u  %u is sleeping\n", get_ms_time(), philo.id);
    usleep(300000);//定数
    // printf("%d  %d is sleeping finished!!! \n", get_ms_time(), philo.id);
}

static void * dine(void *arg)
{
    int i;
    t_philo *philo = (t_philo *)arg;
	// fprintf(stderr, "philo [%d], left: %p, right: %p\n", philo->id, philo->left_fork, philo->right_fork);
    i = 0;
    while(i < philo->count)//?5times??例死ぬまでループの可能性もある(何回食べるかの回数ぶん回すoption有)
    {
        func_think(*philo);
        func_eat(*philo);
        func_sleep(*philo);
        i++;
    }
}




int    main(void)
{
   pthread_t   thread_id[5];
   int         i;
   t_philo     philo[5];
//    pthread_mutex_t fork[5];
   int philo_num = 5;
	static pthread_mutex_t fork[5] = {PTHREAD_MUTEX_INITIALIZER};//init + dedach


   i = 0;
   while(i < philo_num)
   {
        // printf("i: %d\n", i);
       philo[i].id = i + 1;
       philo[i].left_fork = &fork[i];
       philo[i].right_fork = &fork[(i+1)%philo_num];
       philo[i].count = philo_num;
       pthread_create(&thread_id[i], NULL, dine, &(philo[i]));
       usleep(20000);
       i++;
   }
   sleep(10);
}