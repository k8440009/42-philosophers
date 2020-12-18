#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct	s_philo{
	int position;
	int timeToDie;
	int timeToEat;
	int leftFork;
	int rightFork;
	int timeToSleep;
	int eatCount;

}				t_philo;
#endif

int	ft_strlen(char const *str);
int	ft_atoi(char const *str);