/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initialize.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 09:16:06 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/01/31 17:00:23 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>

int	initialize_fork_available(t_arguments *arguments)
{
	int	i;

	arguments->fork_available = malloc((arguments->number_of_philosophers + 1) \
	* sizeof(int));
	if (!arguments->fork_available)
		return (ft_error("Malloc failed\n"));
	i = 0;
	while (i < arguments->number_of_philosophers)
	{
		arguments->fork_available[i] = 0;
		i++;
	}
	arguments->fork_available[arguments->number_of_philosophers] = 2;
	return (0);
}

int	initialize_args(t_arguments *arguments, char **argv)
{
	arguments->number_of_philosophers = ft_atoi(argv[1]);
	arguments->time_to_die = ft_atoi(argv[2]);
	arguments->time_to_eat = ft_atoi(argv[3]);
	arguments->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
	{
		arguments->times_must_eat = ft_atoi(argv[5]);
		if (arguments->times_must_eat == 0)
			return (ft_error("Values must be bigger than zero\n"));
	}
	else
		arguments->times_must_eat = 0;
	arguments->stop_sim = 0;
	arguments->can_print = 1;
	if (arguments->number_of_philosophers <= 0 || arguments->time_to_die <= 0 \
	|| arguments->time_to_eat <= 0 || arguments->time_to_sleep <= 0 || \
	arguments->times_must_eat < 0)
		return (ft_error("Values must be bigger than zero\n"));
	if (arguments->number_of_philosophers > 200)
		return (ft_error("Max number of philosophers is 200\n"));
	return (initialize_fork_available(arguments));
}
