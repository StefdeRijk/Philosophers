/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   timestamp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 15:19:43 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/01/26 16:37:32 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>

long	get_curr_time(void)
{
	struct timeval	cur_time;
	long			current_time;

	gettimeofday(&cur_time, NULL);
	current_time = (cur_time.tv_sec * 1000) + \
	(cur_time.tv_usec / 1000);
	return (current_time);
}
