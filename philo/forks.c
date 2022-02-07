/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   forks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:25:19 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/02/07 10:08:20 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>
#include <stdio.h>

void	put_down_forks(t_philo *s_philo)
{
	pthread_mutex_lock(&s_philo->arguments->forks[s_philo->left_fork]);
	s_philo->arguments->fork_available[s_philo->left_fork] = 0;
	pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->left_fork]);
	pthread_mutex_lock(&s_philo->arguments->forks[s_philo->right_fork]);
	s_philo->arguments->fork_available[s_philo->right_fork] = 0;
	pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->right_fork]);
}

int	pick_up_forks_left(t_philo *s_philo)
{
	pthread_mutex_lock(&s_philo->arguments->forks[s_philo->left_fork]);
	pthread_mutex_lock(&s_philo->arguments->forks[s_philo->right_fork]);
	if (!s_philo->arguments->fork_available[s_philo->left_fork] && \
	!s_philo->arguments->fork_available[s_philo->right_fork])
	{
		s_philo->arguments->fork_available[s_philo->left_fork] = 1;
		printing("%li %i has taken a fork\n", s_philo);
		s_philo->arguments->fork_available[s_philo->right_fork] = 1;
		printing("%li %i has taken a fork\n", s_philo);
	}
	else
	{
		pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->left_fork]);
		pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->right_fork]);
		return (0);
	}
	pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->left_fork]);
	pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->right_fork]);
	return (1);
}

int	pick_up_forks_right(t_philo *s_philo)
{
	pthread_mutex_lock(&s_philo->arguments->forks[s_philo->right_fork]);
	pthread_mutex_lock(&s_philo->arguments->forks[s_philo->left_fork]);
	if (!s_philo->arguments->fork_available[s_philo->right_fork] && \
	!s_philo->arguments->fork_available[s_philo->left_fork])
	{
		s_philo->arguments->fork_available[s_philo->right_fork] = 1;
		printing("%li %i has taken a fork\n", s_philo);
		s_philo->arguments->fork_available[s_philo->left_fork] = 1;
		printing("%li %i has taken a fork\n", s_philo);
	}
	else
	{
		pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->right_fork]);
		pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->left_fork]);
		return (0);
	}
	pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->right_fork]);
	pthread_mutex_unlock(&s_philo->arguments->forks[s_philo->left_fork]);
	return (1);
}
