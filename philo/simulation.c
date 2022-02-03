/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   simulation.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 10:32:31 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/02/03 13:42:46 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void	set_s_philo(t_philo *s_philo, int i, t_arguments *arguments)
{
	s_philo->arguments = arguments;
	s_philo->philosopher_id = i + 1;
	s_philo->times_eaten = 0;
	s_philo->start_time = get_curr_time();
	pthread_mutex_init(&s_philo->arguments->forks[i], NULL);
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

void	*check_times_eaten(void *void_philo)
{
	t_philo	*s_philo;
	int		*philos;
	int		done;
	int		i;

	s_philo = (t_philo *)void_philo;
	philos = malloc(s_philo->arguments->number_of_philosophers * sizeof(int));
	if (!philos)
		return (void_philo); // needs proper protection
	i = 0;
	while (i < s_philo->arguments->number_of_philosophers)
		philos[i++] = 0;
	done = 0;
	while (done < s_philo->arguments->number_of_philosophers)
	{
		i = 0;
		while (i < s_philo->arguments->number_of_philosophers)
		{
			if (philos[i] != 1)
			{
				pthread_mutex_lock(&s_philo->arguments->check_eaten_lock);
				if (s_philo[i].times_eaten == \
				s_philo->arguments->times_must_eat + 1)
					philos[i] = 1;
				pthread_mutex_unlock(&s_philo->arguments->check_eaten_lock);
			}
			if (philos[i] == 1)
				done++;
			i++;
		}
	}
	free(philos);
	s_philo->arguments->stop_sim = 1;
	return (void_philo);
}

int	get_philosophers(t_arguments *arguments)
{
	pthread_t		philo_thread[200];
	pthread_t		check_eaten_enough;
	t_philo			*s_philo;
	int				i;

	s_philo = ft_calloc(arguments->number_of_philosophers, sizeof(*s_philo));
	if (!s_philo)
	{
		free(arguments->fork_available);
		return (ft_error("Malloc failed\n"));
	}
	pthread_mutex_init(&arguments->waiter, NULL);
	pthread_mutex_init(&arguments->print_lock, NULL);
	pthread_mutex_init(&arguments->sim_lock, NULL);
	pthread_mutex_init(&arguments->check_forks_lock, NULL);
	pthread_mutex_init(&arguments->check_eaten_lock, NULL);
	i = 0;
	while (i < arguments->number_of_philosophers)
	{
		set_s_philo(&s_philo[i], i, arguments);
		pthread_create(&philo_thread[i], NULL, ft_philosopher, \
		(void *)&s_philo[i]);
		i++;
	}
	if (s_philo->arguments->times_must_eat > 0)
		pthread_create(&check_eaten_enough, NULL, check_times_eaten, \
		(void *)s_philo);
	join_threads(philo_thread, arguments);
	free(s_philo);
	return (0);
}

int	start_simulation(t_arguments *arguments)
{
	struct timeval	cur_time;
	int				ret;

	gettimeofday(&cur_time, NULL);
	arguments->time_at_start = (cur_time.tv_sec * 1000) + \
	(cur_time.tv_usec / 1000);
	ret = get_philosophers(arguments);
	return (ret);
}
