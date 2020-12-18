#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define EATING_TIME 2

pthread_mutex_t mutexes[NUM_THREADS];
pthread_cond_t conditionVars[NUM_THREADS];

int permits[NUM_THREADS];
pthread_t tids[NUM_THREADS];

void pickup_forks(int philosopher_number){
	pthread_mutex_lock(&mutexes[philosopher_number % NUM_THREADS]);
	while(permits[philosopher_number % NUM_THREADS] == 0){
		pthread_cond_wait(&conditionVars[philosopher_number % NUM_THREADS], &mutexes[philosopher_number % NUM_THREADS]);
	}
	permits[philosopher_number % NUM_THREADS] = 0;
	pthread_mutex_unlock(&mutexes[philosopher_number % NUM_THREADS]);
}

void return_forks(int philosopher_number){
	pthread_mutex_lock(&mutexes[philosopher_number % NUM_THREADS]);
	permits[philosopher_number % NUM_THREADS] = 1;
	pthread_cond_signal(&conditionVars[philosopher_number % NUM_THREADS]);
	pthread_mutex_unlock(&mutexes[philosopher_number % NUM_THREADS]);
}

void *OddPhilosopher(void *arg){
	int philosopher_number;
	philosopher_number = (int)arg;
	// 오른쪽 포크를 집는다.
	pickup_forks(philosopher_number + 1);
	printf("philosophper (%d) picks up the fork (%d).\n", philosopher_number, (philosopher_number + 1) % NUM_THREADS);
	// 왼쪽 포크를 집는다.
	pickup_forks(philosopher_number);
	printf("philosophper (%d) picks up the fork (%d).\n", philosopher_number, philosopher_number);

	printf("philosophper(%d) start eating \n", philosopher_number);
	sleep(2);
	printf("philosophper(%d) end eating \n", philosopher_number);

	// 왼쪽 포크를 내려 놓는다.
	return_forks(philosopher_number);
	printf("philosophper (%d) put down the fork (%d).\n", philosopher_number, philosopher_number);
	// 오른쪽 포크를 내려 놓는다.
	return_forks(philosopher_number + 1);
	printf("philosophper (%d) put down the fork (%d).\n", philosopher_number, (philosopher_number + 1) % NUM_THREADS);
	return NULL;
}

void *Philosopher(void *arg){
	int philosopher_number;
	philosopher_number = (int)arg;

	// 왼쪽 포크를 집는다.
	pickup_forks(philosopher_number);
	printf("philosophper (%d) picks up the fork (%d).\n", philosopher_number, philosopher_number);
	// 오른쪽 포크를 집는다.
	pickup_forks(philosopher_number + 1);
	printf("philosophper (%d) picks up the fork (%d).\n", philosopher_number, (philosopher_number + 1) % NUM_THREADS);

	printf("philosophper(%d) start eating \n", philosopher_number);
	sleep(2);
	printf("philosophper(%d) end eating \n", philosopher_number);

	// 오른쪽 포크를 내려 놓는다.
	return_forks(philosopher_number + 1);
	printf("philosophper (%d) put down the fork (%d).\n", philosopher_number, (philosopher_number + 1) % NUM_THREADS);
	// 왼쪽 포크를 내려 놓는다.
	return_forks(philosopher_number);
	printf("philosophper (%d) put down the fork (%d).\n", philosopher_number, philosopher_number);
	return NULL;
}

int main(){
	for (int i = 0; i < NUM_THREADS; i++){
		pthread_mutex_init(&mutexes[i], NULL);
		pthread_cond_init(&conditionVars[i], NULL);
		permits[i] = 1;
	}

	for (int i = 0; i < NUM_THREADS; i++){
		if (i % 2)
			pthread_create(&tids[i], NULL, OddPhilosopher, (void*)(i));
		else
			pthread_create(&tids[i], NULL, Philosopher, (void*)(i));
	}
	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(tids[i], NULL);
	for (int i = 0; i < NUM_THREADS; i++){
		pthread_mutex_destroy(&mutexes[i]);
		pthread_cond_destroy(&conditionVars[i]);
	}
	return 0;
}