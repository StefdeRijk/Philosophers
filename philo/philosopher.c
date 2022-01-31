/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosopher.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:09:34 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/01/31 10:53:58 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>

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
