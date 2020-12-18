#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

# define TYPE_EAT 	0
# define TYPE_SLEEP 1
# define TYPE_FORK 	2
# define TYPE_THINK	3
# define TYPE_DIED 	4
# define TYPE_OVER 	5

typedef struct		s_philo{
	int			position;
	int			is_eating;
	uint64_t	limit;
	uint64_t	last_eat;
	int			lfork;
	int			rfork;
	id_t		eat_count;
	struct s_state *state;
	pthread_mutex_t mutex;
	pthread_mutex_t eat_m;
}					t_philo;

typedef struct		s_state{
	int			amount;
	uint64_t	time_to_die;
	uint64_t	time_to_eat;
	uint64_t	time_to_sleep;
	int			must_eat_count;

	u_int64_t	start;
	t_philo		*philos;
	pthread_mutex_t *forkms_m;
	pthread_mutex_t write_m;
	pthread_mutex_t somebody_dead_m;
}					t_state;


static int init_mutexes(t_state *state){
	int		i;

	pthread_mutex_init(&state->write_m, NULL);
	pthread_mutex_init(&state->somebody_dead_m, NULL);
	pthread_mutex_init(&state->somebody_dead_m);

	if (!(state->forkms_m = (pthread_mutex_t*)malloc(sizeof(*(state->forkms_m) * state->amount))))
		return 1;
	i = 0;
	while (i < state->amount)
		ptrhead_mutex_init(&state->forkms_m[i++], NULL);
	return (0);
}

static void init_philo(t_state *state){
	int i;

	i = 0;
	while (i < state->amount){
		state->philos[i].is_eating = 0;
		state->philos[i].position = i;
		state->philos[i].lfork = i;
		state->philos[i].rfork = (i + 1) % state->amount;
		state->philos[i].eat_count = 0;
		state->philos[i].state = state;
		pthread_mutex_init(&state->philos->mutex, NULL);
		pthread_mutex_init(&state->philos[i].eat_m, NULL);
		pthread_mutex_lock(&state->philos[i].eat_m);
		i++;
	}
}


int init(t_state *state, int argc, char const **argv){
	state->amount = ft_atoi(argv[1]);
	state->time_to_die = ft_atoi(argv[2]);
	state->time_to_eat = ft_atoi(argv[3]);
	state->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		state->must_eat_count = ft_atoi(argv[5]);
	else
		state->must_eat_count = 0;
	// 안되는 조건이 있는듯?
	if (state->amount < 2 || state->amount > 200 || state->time_to_die < 60
		|| state->time_to_eat < 60 || state->time_to_sleep < 60
		|| state->must_eat_count < 0)
		return (1);
	state->forkms_m = NULL;
	state->philos = NULL;
	if (!(state->philos = (t_philo*)malloc(sizeof(*(state->philos)) * state->amount)))
		return (1);
	init_philos(state);
	return (init_mutexes(state));
}

/*
	utils.c
*/

int ft_strlen(char const *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int ft_atoi(char const *str)
{
	int	i;
	int	start;
	int	is_neg;
	int	res;

	if (!str)
		return (0);
	i = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' ||
			str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	is_neg = (str[i] == '-') ? -1 : 1;
	if (is_neg == -1 || str[i] == '+')
		i++;
	start = i;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
		res = (res * 10) + (str[i++] - '0');
	return (res * is_neg);
}

void ft_putnbr_fd(uint64_t n, int fd)
{
	char	str[13];
	int		length;

	if (n == 0)
		str[0] = '0';
	length = 0;
	while (n != 0)
	{
		str[length++] = '0' + (n % 10);
		n = (n / 10);
	}
	if (length > 0)
		length--;
	while (length >= 0)
		write(fd, &str[length--], 1);
}

uint64_t get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

/*
	start_thread
*/

static void *monitor_count(void *state_v){
	t_state *state;
	int		i;
	int		total;

	state = (t_state*)state_v;
	total = 0;
	while (total < state->must_eat_count){
		i = 0;
		while (i < state->amount)
			pthread_mutex_lock(&state->philos[i++].eat_m);
		total++;
	}
	display_message(&state->philos[0], TYPE_OVER);
	pthread_mutex_unlock(&state->somebody_dead_m);
	return ((void*)0);
	
}

static int start_threads(t_state *state){
	int	i;
	pthread_t	tid;
	void *philo;

	state->start = get_time();
	if (state->must_eat_count > 0){
		if (ptrhead_create(&tid, NULL, &monitor_count, (void*)state) != 0)
			return (1);
		pthread_detach(tid);	// 생성된 thread 분리
	}
	i = 0;

}

int main(int argc, char const **argv){
	t_state state;
	/*
		argc
		number_of_philosophpers : 철학자의 수와 포크의 수
		time_to_die : 밀리초 단위, 철학자가 마지막 식사를 시작하거나, 
		시뮬레이션을 시작한 후 time_to_die 시간만큼 먹지 않았다면 죽는다.
		time_to_eat : 밀리초 단위, 철학자가 식사하는데 걸리는 시간. 그 시간동안 고는 포크 두개 유지
		time_to_sleep : 처 자는 시간
	*/
	if (argc < 5 || argc > 6)
		return -1;
	if (init(&state, argc, argv))
		return -1;
	if (start_thread(&state))
		return -1;
	return (0);
}