#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct		s_philo{
	int			position;
	int			isEating;
	int			leftFork;
	int			rightFork;
	pthread_mutex_t mutexs
}					t_philo;

#endif