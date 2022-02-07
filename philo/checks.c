/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   checks.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 13:33:29 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/02/07 11:35:29 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <unistd.h>

int	check_eaten(t_philo *s_philo)
{
	int	done;
	int	i;

	i = 0;
	done = 0;
	while (i < s_philo->arguments->number_of_philosophers && !stop_sim(s_philo))
	{
		if (s_philo->arguments->philos_eaten[i] != 1)
		{
			pthread_mutex_lock(&s_philo->arguments->check_eaten_lock);
			if (s_philo[i].times_eaten == \
			s_philo->arguments->times_must_eat)
				s_philo->arguments->philos_eaten[i] = 1;
			pthread_mutex_unlock(&s_philo->arguments->check_eaten_lock);
		}
		if (s_philo->arguments->philos_eaten[i] == 1)
			done++;
		i++;
	}
	return (done);
}

void	*check_times_eaten(void *void_philo)
{
	t_philo	*s_philo;
	int		done;
	int		i;

	s_philo = (t_philo *)void_philo;
	i = 0;
	while (i < s_philo->arguments->number_of_philosophers)
		s_philo->arguments->philos_eaten[i++] = 0;
	done = 0;
	while (done < s_philo->arguments->number_of_philosophers && \
	!stop_sim(s_philo))
	{
		done = check_eaten(s_philo);
		usleep(2000);
	}
	free(s_philo->arguments->philos_eaten);
	pthread_mutex_lock(&s_philo->arguments->sim_lock);
	s_philo->arguments->stop_sim = 1;
	pthread_mutex_unlock(&s_philo->arguments->sim_lock);
	return (void_philo);
}

void	*check_deaths(void *void_philo)
{
	t_philo	*s_philo;
	int		i;

	s_philo = (t_philo *)void_philo;
	while (1)
	{
		i = 0;
		while (i < s_philo->arguments->number_of_philosophers)
		{
			if (is_dead(&s_philo[i]) || stop_sim(s_philo))
			{
				pthread_mutex_lock(&s_philo->arguments->sim_lock);
				s_philo->arguments->stop_sim = 1;
				pthread_mutex_unlock(&s_philo->arguments->sim_lock);
				return (void_philo);
			}
			usleep(100);
			i++;
		}
		usleep(2000);
	}
	return (void_philo);
}
