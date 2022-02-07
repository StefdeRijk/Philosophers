/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   simulation.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 10:32:31 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/02/07 11:33:13 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>

void	create_s_philo(t_philo *s_philo, int i, \
t_arguments *arguments, pthread_t *philo_thread)
{
	s_philo[i].arguments = arguments;
	s_philo[i].philosopher_id = i + 1;
	s_philo[i].times_eaten = 0;
	s_philo[i].start_time = get_curr_time();
	pthread_mutex_init(&s_philo->arguments->forks[i], NULL);
	pthread_create(philo_thread, NULL, ft_philosopher, \
	(void *)&s_philo[i]);
}

void	join_threads(pthread_t *philo_thread, t_arguments *arguments, \
pthread_t check_eaten_enough, pthread_t check_stop_simulation)
{
	int	i;

	i = 0;
	while (i < arguments->number_of_philosophers)
	{
		pthread_join(philo_thread[i], NULL);
		i++;
	}
	if (arguments->times_must_eat)
		pthread_join(check_eaten_enough, NULL);
	pthread_join(check_stop_simulation, NULL);
}

void	initiate_mutexes(t_arguments *arguments)
{
	pthread_mutex_init(&arguments->start_time_lock, NULL);
	pthread_mutex_init(&arguments->print_lock, NULL);
	pthread_mutex_init(&arguments->sim_lock, NULL);
	pthread_mutex_init(&arguments->check_eaten_lock, NULL);
}

void	get_philosophers(t_arguments *arguments, t_philo *s_philo)
{
	pthread_t		philo_thread[200];
	pthread_t		check_eaten_enough;
	pthread_t		check_stop_simulation;
	int				i;

	i = 0;
	while (i < arguments->number_of_philosophers)
	{
		create_s_philo(s_philo, i, arguments, &philo_thread[i]);
		i++;
	}
	if (s_philo->arguments->times_must_eat > 0)
		pthread_create(&check_eaten_enough, NULL, check_times_eaten, \
		(void *)s_philo);
	else
		check_eaten_enough = NULL;
	pthread_create(&check_stop_simulation, NULL, check_deaths, (void *)s_philo);
	join_threads(philo_thread, arguments, \
	check_eaten_enough, check_stop_simulation);
	free(s_philo);
}

int	start_simulation(t_arguments *arguments)
{
	struct timeval	cur_time;
	t_philo			*s_philo;

	gettimeofday(&cur_time, NULL);
	arguments->time_at_start = (cur_time.tv_sec * 1000) + \
	(cur_time.tv_usec / 1000);
	initiate_mutexes(arguments);
	s_philo = ft_calloc(arguments->number_of_philosophers, sizeof(*s_philo));
	if (!s_philo)
	{
		free(arguments->fork_available);
		return (ft_error("Malloc failed\n"));
	}
	get_philosophers(arguments, s_philo);
	return (0);
}
