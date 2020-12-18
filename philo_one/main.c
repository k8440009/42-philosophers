#include "philosophers.h"

const int MAX = 5;
t_philo arr[MAX];
int goalEat;

void takeUpForks(){

}
void takeDownForks(){

}

void evenPhilosopher(){

}

void OddPhilosopher(){

}

int main(int argc, char const **argv){
	/*
		argc
		number_of_philosophpers : 철학자의 수와 포크의 수
		time_to_die : 밀리초 단위, 철학자가 마지막 식사를 시작하거나, 
		시뮬레이션을 시작한 후 time_to_die 시간만큼 먹지 않았다면 죽는다.
		time_to_eat : 밀리초 단위, 철학자가 식사하는데 걸리는 시간. 그 시간동안 고는 포크 두개 유지
		time_to_sleep : 처 자는 시간
		[number_of_times_each_philosopher_must_eat] : 각각의 철학자가 반드시 먹는데 드는 횟수?
	*/
	if (argc == 5 || argc == 6)
		return -1;
	// 철학자 배열 초기화
	for (int i = 0; i < MAX; i++){
		arr[i].position = i;
		arr[i].eatCount = 0;
		arr[i].timeToDie = ft_atoi(argv[2]);
		arr[i].timeToEat = ft_atoi(argv[3]);
		arr[i].timeToSleep = ft_atoi(argv[4]);
		if (argc == 6)
			goalEat = ft_atoi(ft_atoi(argv[5]));
	}
	// 시뮬레이션

}