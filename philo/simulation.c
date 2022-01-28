/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   simulation.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 10:32:31 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/01/28 09:41:24 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>

#include <stdio.h>

void	set_s_philo(t_philo *s_philo, int i, t_arguments *arguments)
{
	s_philo->arguments = arguments;
	s_philo->philosopher_id = i + 1;
	s_philo->times_eaten = 0;
	s_philo->start_time = s_philo->arguments->time_at_start;
}

void	join_threads(pthread_t *philo_thread, t_arguments *arguments)
{
	int	i;

	i = 0;
	while (i < arguments->number_of_philosophers)
	{
		pthread_join(philo_thread[i], NULL);
		i++;
	}
}

void	check_times_eaten(t_philo *s_philo)
{
	int	philos_done;
	int	i;

	philos_done = 0;
	while (philos_done < s_philo->arguments->number_of_philosophers)
	{
		i = 0;
		philos_done = 0;
		while (i < s_philo->arguments->number_of_philosophers)
		{
			if (s_philo[i].times_eaten == s_philo->arguments->times_must_eat)
				philos_done++;
			i++;
		}
	}
	s_philo->arguments->stop_sim = 1;
}

int	get_philosophers(t_arguments *arguments)
{
	pthread_t		philo_thread[200];
	t_philo			*s_philo;
	int				i;

	s_philo = ft_calloc(arguments->number_of_philosophers + 1, \
	sizeof(*s_philo));
	if (!s_philo)
		return (ft_error("Malloc failed\n"));
	pthread_mutex_init(&arguments->waiter, NULL);
	pthread_mutex_init(&arguments->print_lock, NULL);
	i = 0;
	while (i < arguments->number_of_philosophers)
	{
		set_s_philo(&s_philo[i], i, arguments);
		pthread_mutex_init(&s_philo->arguments->forks[i], NULL);
		pthread_create(&philo_thread[i], NULL, ft_philosopher, \
		(void *)&s_philo[i]);
		i++;
	}
	if (s_philo->arguments->times_must_eat > 0)
		check_times_eaten(s_philo);
	join_threads(philo_thread, arguments);
	free(s_philo);
	return (0);
}

int	start_simulation(t_arguments *arguments)
{
	struct timeval	cur_time;
	int				ret;

	if (gettimeofday(&cur_time, NULL) == -1)
		return (ft_error("Gettimeofday failed\n"));
	arguments->time_at_start = (cur_time.tv_sec * 1000) + \
	(cur_time.tv_usec / 1000);
	ret = get_philosophers(arguments);
	return (ret);
}
