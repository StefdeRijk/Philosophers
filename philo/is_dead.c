/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   is_dead.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 15:34:27 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/02/07 09:28:17 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int	stop_sim(t_philo *s_philo)
{
	pthread_mutex_lock(&s_philo->arguments->sim_lock);
	if (s_philo->arguments->stop_sim)
	{
		pthread_mutex_unlock(&s_philo->arguments->sim_lock);
		return (1);
	}
	pthread_mutex_unlock(&s_philo->arguments->sim_lock);
	return (0);
}

int	is_dead(t_philo *s_philo)
{
	struct timeval	s_cur_time;
	long			cur_time;

	gettimeofday(&s_cur_time, NULL);
	cur_time = (s_cur_time.tv_sec * 1000) + (s_cur_time.tv_usec / 1000);
	pthread_mutex_lock(&s_philo->arguments->start_time_lock);
	while (cur_time - s_philo->start_time > s_philo->arguments->time_to_die \
	&& !stop_sim(s_philo))
	{
		pthread_mutex_lock(&s_philo->arguments->print_lock);
		pthread_mutex_lock(&s_philo->arguments->sim_lock);
		if (s_philo->arguments->can_print)
		{
			pthread_mutex_unlock(&s_philo->arguments->start_time_lock);
			s_philo->arguments->can_print = 0;
			s_philo->arguments->stop_sim = 1;
			printf("%li %i died\n", cur_time, s_philo->philosopher_id);
			pthread_mutex_unlock(&s_philo->arguments->print_lock);
			pthread_mutex_unlock(&s_philo->arguments->sim_lock);
			return (1);
		}
		pthread_mutex_unlock(&s_philo->arguments->print_lock);
	}
	pthread_mutex_unlock(&s_philo->arguments->start_time_lock);
	return (0);
}
