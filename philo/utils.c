/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/25 09:25:10 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/01/31 17:23:26 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	int		n;
	long	i;

	i = 0;
	n = 1;
	while (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n'
		|| *str == '\v' || *str == '\f')
		str++;
	if (*str == '-')
	{
		n = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9' && *str != '\0')
	{
		i *= 10;
		i += *str - '0';
		str++;
	}
	i *= n;
	return ((int)i);
}

void	*ft_calloc(int count, int size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
		return (0);
	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}
	memset(ptr, 0, count * size);
	return (ptr);
}

void	printing(char *str, t_philo *s_philo)
{
	long			timestamp;

	pthread_mutex_lock(&s_philo->arguments->print_lock);
	if (s_philo->arguments->can_print)
	{
		s_philo->arguments->can_print = 0;
		timestamp = get_curr_time();
		if (!s_philo->arguments->stop_sim && !is_dead(s_philo))
			printf(str, timestamp, s_philo->philosopher_id);
		s_philo->arguments->can_print = 1;
	}
	pthread_mutex_unlock(&s_philo->arguments->print_lock);
}

int	ft_error(char *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return (1);
}
