/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sde-rijk <sde-rijk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/25 09:22:33 by sde-rijk      #+#    #+#                 */
/*   Updated: 2022/01/28 14:41:45 by sde-rijk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_arguments	arguments;

	if (argc < 5 || argc > 6)
		return (ft_error("Wrong amount of arguments\n"));
	if (initialize_args(&arguments, argv))
		return (1);
	if (start_simulation(&arguments))
		return (1);
	return (0);
}
