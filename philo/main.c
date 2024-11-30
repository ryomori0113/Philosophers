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
    printf("%d  %d is thinking\n", get_ms_time(), philo.id);
}

void func_eat(t_philo philo)
{
    printf("%d  %d is eating\n", get_ms_time(), philo.id);
    usleep(3000000);
}

void func_sleep(t_philo philo)
{
    printf("%d  %d is sleeping\n", get_ms_time(), philo.id);
    usleep(3000000);
}

static void * dine(void *arg)
{
    int i;
    t_philo *philo = (t_philo *)arg;
	// fprintf(stderr, "philo [%d], left: %p, right: %p\n", philo->id, philo->left_fork, philo->right_fork);
    i = 0;
    while(i < philo->count)
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
	static pthread_mutex_t fork[5] = {PTHREAD_MUTEX_INITIALIZER};


   i = 0;
   while(i < philo_num)
   {
        printf("i: %d\n", i);
       philo[i].id = i + 1;
       philo[i].left_fork = fork[i];
       philo[i].right_fork = fork[(i+1)%philo_num];
       philo[i].count = philo_num;
       pthread_create(&thread_id[i], NULL, dine, &(philo[i]));
       i++;
   }
   sleep(10);
}
