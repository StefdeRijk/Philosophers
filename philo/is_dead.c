/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   is_dead.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 15:34:27 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/02/03 12:52:44 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int	stop_sim(t_philo *s_philo)
{
	pthread_mutex_lock(&s_philo->arguments->sim_lock);
	if (s_philo->arguments->stop_sim || is_dead(s_philo))
	{
		pthread_mutex_unlock(&s_philo->arguments->sim_lock);
		return (1);
	}
	pthread_mutex_unlock(&s_philo->arguments->sim_lock);
	return (0);
}

int	is_dead(t_philo *s_philo)
{
	struct timeval	cur_time;
	long			current_time;

	gettimeofday(&cur_time, NULL);
	current_time = (cur_time.tv_sec * 1000) + \
	(cur_time.tv_usec / 1000);
	while (current_time - s_philo->start_time > s_philo->arguments->time_to_die \
	&& !s_philo->arguments->stop_sim)
	{
		pthread_mutex_lock(&s_philo->arguments->print_lock);
		if (s_philo->arguments->can_print)
		{
			s_philo->arguments->can_print = 0;
			s_philo->arguments->stop_sim = 1;
			printf("%li %i died\n", current_time, s_philo->philosopher_id);
			pthread_mutex_unlock(&s_philo->arguments->print_lock);
			free(s_philo->arguments->fork_available);
			return (1);
		}
	}
	return (0);
}
