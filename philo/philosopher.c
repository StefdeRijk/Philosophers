/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosopher.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:09:34 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/02/03 13:31:07 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>

void	put_down_forks(t_philo *s_philo)
{
	pthread_mutex_lock(&s_philo->arguments->check_forks_lock);
	s_philo->arguments->fork_available[s_philo->left_fork] = 0;
	pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->left_fork]);
	s_philo->arguments->fork_available[s_philo->right_fork] = 0;
	pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->right_fork]);
	pthread_mutex_unlock(&s_philo->arguments->check_forks_lock);
}

int	pick_up_forks(t_philo *s_philo)
{
	pthread_mutex_lock(&s_philo->arguments->check_forks_lock);
	if (!s_philo->arguments->fork_available[s_philo->left_fork] && \
	!s_philo->arguments->fork_available[s_philo->right_fork])
	{
		pthread_mutex_lock(&s_philo->arguments->forks[s_philo->left_fork]);
		s_philo->arguments->fork_available[s_philo->left_fork] = 1;
		printing("%li %i has taken a fork\n", s_philo);
		pthread_mutex_lock(&s_philo->arguments->forks[s_philo->right_fork]);
		s_philo->arguments->fork_available[s_philo->right_fork] = 1;
		printing("%li %i has taken a fork\n", s_philo);
		pthread_mutex_unlock(&s_philo->arguments->check_forks_lock);
		return (1);
	}
	pthread_mutex_unlock(&s_philo->arguments->check_forks_lock);
	return (0);
}

int	forks_available(t_philo *s_philo)
{
	pthread_mutex_lock(&s_philo->arguments->check_forks_lock);
	if (!s_philo->arguments->fork_available[s_philo->left_fork] && \
	!s_philo->arguments->fork_available[s_philo->right_fork])
	{
		pthread_mutex_unlock(&s_philo->arguments->check_forks_lock);
		return (1);
	}
	pthread_mutex_unlock(&s_philo->arguments->check_forks_lock);
	return (0);
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
	try_to_eat(s_philo);
	return (void_philo);
}
