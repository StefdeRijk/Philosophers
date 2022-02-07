/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosopher.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:09:34 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/02/07 11:19:35 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void	sleeping(t_philo *s_philo)
{
	long	start_time;

	printing("%li %i is sleeping\n", s_philo);
	start_time = get_curr_time();
	while (get_curr_time() < start_time + \
	s_philo->arguments->time_to_sleep)
		usleep(500);
	thinking(s_philo);
}

void	thinking(t_philo *s_philo)
{
	printing("%li %i is thinking\n", s_philo);
	usleep((s_philo->arguments->time_to_sleep / 2) * 10);
	try_to_eat(s_philo);
}

void	eating(t_philo *s_philo)
{
	printing("%li %i is eating\n", s_philo);
	pthread_mutex_lock(&s_philo->arguments->start_time_lock);
	s_philo->start_time = get_curr_time();
	pthread_mutex_unlock(&s_philo->arguments->start_time_lock);
	while (get_curr_time() < s_philo->start_time + \
	s_philo->arguments->time_to_eat)
		usleep(500);
	put_down_forks(s_philo);
	pthread_mutex_lock(&s_philo->arguments->check_eaten_lock);
	s_philo->times_eaten += 1;
	pthread_mutex_unlock(&s_philo->arguments->check_eaten_lock);
	sleeping(s_philo);
}

void	try_to_eat(t_philo *s_philo)
{
	int	picked_up_forks;

	picked_up_forks = 0;
	while (!stop_sim(s_philo))
	{
		if (s_philo->philosopher_id == \
		s_philo->arguments->number_of_philosophers)
			picked_up_forks = pick_up_forks_right(s_philo);
		else
			picked_up_forks = pick_up_forks_left(s_philo);
		if (picked_up_forks)
			eating(s_philo);
		usleep(500);
	}
}

void	*ft_philosopher(void *void_philo)
{
	t_philo			*s_philo;

	s_philo = (t_philo *)void_philo;
	s_philo->left_fork = s_philo->philosopher_id - 1;
	s_philo->right_fork = s_philo->philosopher_id;
	if (s_philo->philosopher_id == \
	s_philo->arguments->number_of_philosophers && \
	s_philo->arguments->number_of_philosophers > 1)
		s_philo->right_fork = 0;
	if (s_philo->philosopher_id % 2)
		usleep(500);
	try_to_eat(s_philo);
	return (void_philo);
}
