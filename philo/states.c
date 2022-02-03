/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   states.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:25:19 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/02/03 13:43:08 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>
#include <stdio.h>

void	sleeping(t_philo *s_philo)
{
	long	start_time;

	printing("%li %i is sleeping\n", s_philo);
	start_time = get_curr_time();
	while (get_curr_time() < start_time + \
	s_philo->arguments->time_to_sleep && !stop_sim(s_philo))
		usleep(4000);
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
	s_philo->start_time = get_curr_time();
	while (get_curr_time() < s_philo->start_time + \
	s_philo->arguments->time_to_eat && !stop_sim(s_philo))
		usleep(4000);
	put_down_forks(s_philo);
	pthread_mutex_lock(&s_philo->arguments->check_eaten_lock);
	s_philo->times_eaten += 1;
	pthread_mutex_unlock(&s_philo->arguments->check_eaten_lock);
	sleeping(s_philo);
}

int	ask_waiter(t_philo *s_philo)
{
	int	ret;

	pthread_mutex_lock(&s_philo->arguments->waiter);
	ret = pick_up_forks(s_philo);
	pthread_mutex_unlock(&s_philo->arguments->waiter);
	return (ret);
}

void	try_to_eat(t_philo *s_philo)
{
	while (!stop_sim(s_philo))
	{
		if (forks_available(s_philo) && ask_waiter(s_philo))
			eating(s_philo);
		usleep(1500);
	}
}
