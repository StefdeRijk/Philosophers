/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   states.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:25:19 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/01/28 14:01:41 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

void	sleeping(t_philo *s_philo)
{
	int	i;

	printing("%li %i is sleeping\n", s_philo);
	i = 0;
	while (i < s_philo->arguments->time_to_sleep)
	{
		usleep(1000);
		i++;
	}
	thinking(s_philo);
}

void	thinking(t_philo *s_philo)
{
	printing("%li %i is thinking\n", s_philo);
	try_to_eat(s_philo);
}

void	eating(t_philo *s_philo)
{
	int	i;

	i = 0;
	printing("%li %i is eating\n", s_philo);
	s_philo->start_time = get_curr_time();
	while (i < s_philo->arguments->time_to_eat)
	{
		usleep(1000);
		i++;
	}
	s_philo->arguments->fork_available[s_philo->left_fork] = 0;
	pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->left_fork]);
	s_philo->arguments->fork_available[s_philo->right_fork] = 0;
	pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->right_fork]);
	s_philo->times_eaten += 1;
	sleeping(s_philo);
}

int	ask_waiter(t_philo *s_philo)
{
	if (!s_philo->arguments->fork_available[s_philo->left_fork] && \
	!s_philo->arguments->fork_available[s_philo->right_fork])
	{
		pthread_mutex_lock(&s_philo->arguments->waiter);
		if (!s_philo->arguments->fork_available[s_philo->left_fork] && \
		!s_philo->arguments->fork_available[s_philo->right_fork])
		{
			pthread_mutex_lock(&s_philo->arguments->forks[s_philo->left_fork]);
			s_philo->arguments->fork_available[s_philo->left_fork] = 1;
			printing("%li %i has taken a fork\n", s_philo);
			pthread_mutex_lock(&s_philo->arguments->forks[s_philo->right_fork]);
			s_philo->arguments->fork_available[s_philo->right_fork] = 1;
			printing("%li %i has taken a fork\n", s_philo);
			pthread_mutex_unlock(&s_philo->arguments->waiter);
			return (1);
		}
		pthread_mutex_unlock(&s_philo->arguments->waiter);
	}
	return (0);
}

void	try_to_eat(t_philo *s_philo)
{
	while (!s_philo->arguments->stop_sim && !is_dead(s_philo))
	{
		if (ask_waiter(s_philo))
			eating(s_philo);
		usleep((s_philo->arguments->time_to_eat / 2) * 1000);
	}
}
